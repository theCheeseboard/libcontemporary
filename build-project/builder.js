const core = require('@actions/core');
const exec = require('@actions/exec');
const io = require('@actions/io');
const fs = require('fs/promises');
const cache = require('@actions/cache');
const os = require("os");
const path = require('path');
const process = require('process');
const clone = require('git-clone/promise');

module.exports = async options => {
    let buildFolder = "";
    let gitRoot;
    if (options.project === ".") {
        gitRoot = path.resolve(".");
    } else {
        buildFolder = path.basename(options.project);
        gitRoot = path.resolve(".", path.basename(options.project));

        let gitOptions = {};
        if (options.commitish) gitOptions.checkout = options.commitish;
        await clone(`https://github.com/${options.project}.git`, gitRoot, gitOptions);
        await exec.exec("git", ["submodule", "init"], {
            cwd: gitRoot
        });
        await exec.exec("git", ["submodule", "update"], {
            cwd: gitRoot
        });

        let tip = "";
        await exec.exec("git", ["rev-parse", "HEAD"], {
            listeners: {
                stdout: data => {
                    tip += data.toString();
                }
            }
        })

        buildFolder += `/${tip}`;
    }

    try {
        let buildDir = path.resolve(os.tmpdir(), "build-project-action", buildFolder);
        await io.mkdirP(buildDir);

        const cacheKey = `build-project-${buildFolder}`;

        if (options.project !== ".") {
            await cache.restoreCache([buildDir], cacheKey)
        }

        let cmakeArgs = [
            "-S", path.resolve(gitRoot),
            "-B", buildDir,
            "-GNinja"
        ];

        let prefixPath = "/usr/lib";
        if (process.platform === "darwin") {
            cmakeArgs.push("-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64");
            cmakeArgs.push("-DCMAKE_PREFIX_PATH=/usr/local/lib");
            prefixPath = "/usr/local/lib";
        } else if (process.platform === 'win32') {
            cmakeArgs.push("-DCMAKE_BUILD_TYPE=Release");
            cmakeArgs.push(`-DCMAKE_INSTALL_PREFIX=${process.env["RUNNER_WORKSPACE"]}/cmake-install/${options.arch}`);
            prefixPath = `${process.env["RUNNER_WORKSPACE"]}/cmake-install/${options.arch}`;

            if (process.env["QT_HOST_PATH"]) {
                cmakeArgs.push(`-DQT_HOST_PATH=${process.env["QT_HOST_PATH"]}`);
                cmakeArgs.push(`-DCNTP_TOOL_PATH=${process.env["RUNNER_WORKSPACE"]}/cmake-install/x64`);
                cmakeArgs.push(`-DCMAKE_PREFIX_PATH=${process.env["RUNNER_WORKSPACE"]}/cmake-install/${options.arch}`);
            }
        }

        if (options.extraCmakeArgs) {
            for (let arg of options.extraCmakeArgs.split(" ")) {
                if (arg === "") continue;

                if (process.platform === 'win32') {
                    //HACK: Replace path seperators
                    arg = arg.replaceAll("\\", "/");
                }

                cmakeArgs.push(arg);
            }
        }

        await exec.exec(`cmake`, cmakeArgs);
        await exec.exec(`cmake`, ["--build", buildDir]);

        if (process.platform === "linux") {
            //On Linux we need to escalate
            await exec.exec(`sudo`, ["cmake", "--install", buildDir]);
        } else {
            await exec.exec(`cmake`, ["--install", buildDir]);
        }

        if (process.platform === 'win32') {
            //Add the bin dir to the PATH
            let cmakeCacheContents = path.resolve(buildDir, "CMakeCache.txt");
            let cmakeCache = await fs.readFile(cmakeCacheContents, {
                encoding: "utf8"
            });

            let cmakeCacheLines = cmakeCache.split(/\r?\n/);
            let properties = {};
            cmakeCacheLines.forEach(line => {
                if (line.includes(":") && line.includes("=")) {
                    let key = line.substring(0, line.indexOf(":"));
                    properties[key] = line.substring(line.indexOf("=") + 1);
                }
            });

            console.log("Extracted properties from CMakeCache");
            console.log(JSON.stringify(properties, null, 4));

            //Add required variables to the PATH if needed
            if (process.arch === options.arch) {
                if (properties["CMAKE_INSTALL_PREFIX"] && properties["CMAKE_INSTALL_BINDIR"]) {
                    core.addPath(path.resolve(properties["CMAKE_INSTALL_PREFIX"], properties["CMAKE_INSTALL_BINDIR"]));
                }
                if (properties["BIN_INSTALL_DIR"]) {
                    core.addPath(properties["BIN_INSTALL_DIR"]);
                }
            }
        }

        core.setOutput("install-prefix", prefixPath);

        if (options.project !== ".") {
            await cache.saveCache([buildDir], cacheKey);
        }
    } finally {
        if (options.project !== ".") {
            await fs.rm(gitRoot, {
                recursive: true
            });
        }
    }
}
