const core = require('@actions/core');
const exec = require('@actions/exec');
const io = require('@actions/io');
const fs = require('fs/promises');
const path = require('path');
const process = require('process');
const clone = require('git-clone/promise');

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
        } else if (process.platform === 'win32') {

        }

        if (options.extraCmakeArgs) {
            for (let arg of options.extraCmakeArgs.split(" ")) {
                if (arg === "") continue;
                cmakeArgs.push(arg);
            }
        }

        await exec.exec(`cmake`, cmakeArgs);
        await exec.exec(`cmake`, ["--build", buildDir]);
        await exec.exec(`cmake`, ["--install", buildDir]);
    } finally {
        if (options.project !== ".") {
            await fs.rm(gitRoot, {
                recursive: true
            });
        }
    }
}
