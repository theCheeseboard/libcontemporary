const core = require('@actions/core');
const exec = require('@actions/exec');
const io = require('@actions/io');
const fs = require('fs/promises');
const path = require('path');
const process = require('process');
const clone = require('git-clone/promise');
const fg = require("fast-glob");

const enableDebug = process.env["RUNNER_DEBUG"] === "1";

module.exports = async options => {
    let gitRoot;
    if (options.project === ".") {
        gitRoot = path.resolve(".");
    } else {
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
    }

    try {
        let buildDir = path.resolve(gitRoot, "build");
        await io.mkdirP(buildDir);

        let cmakeArgs = [
            "-S", path.resolve(gitRoot),
            "-B", buildDir,
            "-GNinja"
        ];
        if (process.platform === "darwin") {
            cmakeArgs.push("-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64");
            cmakeArgs.push("-DCMAKE_PREFIX_PATH=/usr/local/lib");

            const paths = await fg("/usr/local/Cellar/pkg-config/*/bin/pkg-config", {
                onlyFiles: false
            });

            if (paths.length > 0) {
                cmakeArgs.push(`-DPKG_CONFIG_EXECUTABLE=${paths[0]}`);
            }
        } else if (process.platform === 'win32') {
            cmakeArgs.push("-DCMAKE_BUILD_TYPE=Release");
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
        await exec.exec(`cmake`, ["--install", buildDir]);

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

            if (enableDebug) {
                console.log("Extracted properties from CMakeCache");
                console.log(JSON.stringify(properties, null, 4));
            }

            //Add required variables to the PATH
            if (properties["CMAKE_INSTALL_PREFIX"] && properties["CMAKE_INSTALL_BINDIR"]) {
                core.addPath(path.resolve(properties["CMAKE_INSTALL_PREFIX"], properties["CMAKE_INSTALL_BINDIR"]));
            }
            if (properties["BIN_INSTALL_DIR"]) {
                core.addPath(properties["BIN_INSTALL_DIR"]);
            }
        }
    } finally {
        if (options.project !== ".") {
            await fs.rm(gitRoot, {
                recursive: true
            });
        }
    }
}
