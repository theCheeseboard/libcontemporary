const path = require("path");
const core = require("@actions/core");
const io = require("@actions/io");
const process = require("process");
const exec = require("@actions/exec");
const fs = require("fs/promises")
const ProjectConfig = require("../project-config");

function createDmg(properties) {
    return new Promise((res, rej) => {
        const appdmg = require("appdmg");
        let dmg = appdmg(properties);

        dmg.on('finish', res);
        dmg.on('error', rej);
    });
}

module.exports = async options => {
    let config = ProjectConfig.getProjectConfig(".");
    let outputDir = path.resolve(".", "build", options.subdir);

    let foundFiles = (await fs.readdir(outputDir)).filter(filename => {
        switch (process.platform) {
            case "darwin":
                return filename.endsWith(".app")
            case "win32":
                return filename.endsWith(".exe")
            //Linux?
        }
    });

    if (foundFiles.length !== 1) throw new Error(`Expected one app, found ${foundFiles.length}`);

    let target = foundFiles[0];
    let executableName = path.basename(target);

    switch (process.platform) {
        case "darwin": {
            //Run cntp-macdeploy and then create the disk image
            await exec.exec("cntp-macdeploy", [target]);

            let output = `${process.env["HOME"]}/${executableName}.dmg`;
            await createDmg({
                target: output,
                basepath: path.resolve("."),
                specification: {
                    title: executableName,
                    icon: config.value("dmg")?.icon,
                    background: config.value("dmg")?.background,
                    "icon-size": 48,
                    window: {
                        size: {
                            width: 600,
                            height: 420
                        }
                    },
                    contents: [
                        {
                            x: 125,
                            y: 225,
                            type: "file",
                            path: target
                        },
                        {
                            x: 470,
                            y: 225,
                            type: "link",
                            path: "/Applications"
                        }
                    ]
                }
            });

            core.setOutput("package", output);
            break;
        }
    }

    // await exec.exec(`cmake`, ["--build", buildDir]);
    // await exec.exec(`cmake`, ["--install", buildDir]);

}