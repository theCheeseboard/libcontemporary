const path = require("path");
const core = require("@actions/core");
const io = require("@actions/io");
const process = require("process");
const exec = require("@actions/exec");
const fs = require("fs/promises")

module.exports = async options => {
    let outputDir = path.resolve(".", "build", options.subdir);

    let foundFiles = (await fs.readdir(outputDir)).filter(filename => {
        switch (process.platform) {
            case "darwin":
                return filename.endsWith(".app")
            case "win32":
                return filename.endsWith(".exe")
            //Linux?
        }
    }).map(filename => path.resolve(outputDir, filename));

    if (foundFiles.length !== 1) throw new Error(`Expected one app, found ${foundFiles.length}`);

    let target = foundFiles[0];
    let executableName = path.basename(target);

    switch (process.platform) {
        case "darwin": {
            // Run cntp-macdeploy and then create the disk image

            // HACK: Due to SSL issues we try removing the OpenSSL backend - we can use Secure Transport anyway
            for (let dir of await fs.readdir("/usr/local/Cellar/qt/")) {
                try {
                    await fs.rm(`/usr/local/Cellar/qt/${dir}/share/qt/plugins/tls/libqopensslbackend.dylib`)
                } catch {
                    console.log(`Unable to remove the file /usr/local/Cellar/qt/${dir}/share/qt/plugins/tls/libqopensslbackend.dylib`)
                }
            }

            await exec.exec("cntp-macdeploy", [target]);

            console.log("Creating disk image...");
            let output = `${process.env["HOME"]}/${executableName}.dmg`;
            await exec.exec("cntp-macpack", [target, output, "-u"]);

            core.setOutput("package", output);
            break;
        }
        case "win32": {
            let deployDir = path.resolve(outputDir, "deploy");
            await io.mkdirP(deployDir);
            await io.cp(target, deployDir + "/");
            await exec.exec("\"C:/Program Files (x86)/libcontemporary/bin/cntp-windeploy.exe\"", [deployDir])

            core.setOutput("package", deployDir);
            break;
        }
    }
}