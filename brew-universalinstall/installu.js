const core = require('@actions/core');
const exec = require('@actions/exec');
const io = require('@actions/io');
const fs = require('fs/promises');
const https = require('follow-redirects').https;
const tar = require('tar-fs');
const stream = require('stream');
const gunzip = require('gunzip-maybe');
const path = require('path');
const clone = require('git-clone/promise');
const dir = require('node-dir');

function getHttps(url) {
    return new Promise((res, rej) => {
        let httpsObject = https.get(url, response => {
            let data = Buffer.alloc(0);
            response.on('data', d => data = Buffer.concat([data, d]));
            response.on('close', () => res(data));
        });
        httpsObject.on('error', e => rej(e));
    });
}

async function lipoIfRequired(arm, system) {
    await exec.exec("lipo", ["-create", arm, system, "-output", system], {
        ignoreReturnCode: true,
        silent: true
    });
}

module.exports = async function(options) {
    let homebrewPath = "./homebrew";
    if (process.env["CI"]) {
        homebrewPath = "/opt/homebrew";
        await exec.exec("sudo", ["chmod", "777", "/opt"], {
            silent: true
        });

        await io.mkdirP('/opt/homebrew');
    }

    //Download brew tarball
    console.log("Downloading Homebrew...");
    let brewTarData = stream.Readable.from(await getHttps("https://github.com/Homebrew/brew/tarball/master"));
    let pipeStream = brewTarData.pipe(gunzip()).pipe(tar.extract(homebrewPath));
    await new Promise(res => pipeStream.on("finish", res));

    try {
        let armBrewRoot = path.resolve(homebrewPath, (await fs.readdir(path.resolve(homebrewPath)))[0]);
        console.log(`ARM Homebrew installed at ${armBrewRoot}`);

        console.log("Shallow tapping homebrew/core");
        await clone("https://github.com/Homebrew/homebrew-core.git", path.resolve(armBrewRoot, "Library/Taps/homebrew/homebrew-core"), {
            shallow: true
        });

        let armBrew = path.resolve(armBrewRoot, "bin/brew");

        let bottlePaths = [];

        for (let pk of options.packages) {
            console.log(`Processing package ${pk}`);
            //Install x86_64 version
            let x86install = exec.exec("brew", ["install", pk], {
                silent: true
            });

            let armBrewOutput = "";
            await exec.exec(armBrew, ["fetch", "--force", "--deps", "--bottle-tag=arm64_big_sur", pk], {
                listeners: {
                    stdout: data => armBrewOutput += data.toString()
                },
                silent: true
            });

            for (let line of armBrewOutput.split("\n")) {
                if (line.startsWith("Downloaded to:")) {
                    bottlePaths.push(line.split(" ")[2]);
                }
            }

            await x86install;
        }

        console.log("Bottles to install: ");
        console.log(bottlePaths);

        await exec.exec(armBrew, ["install", ...bottlePaths], {
            silent: false
        });

        console.log("Merging libraries");

        //Walk the directory and lipo files together
        let brewFiles = dir.files(path.resolve(armBrewRoot, "Cellar"), {
            sync: true
        });

        let promises = [];
        for (let library of brewFiles) {
            let rel = path.relative(armBrewRoot, library);
            promises.push(lipoIfRequired(library, path.resolve("/usr/local", rel)));
        }
        await Promise.all(promises);
    } finally {
        console.log(`Removing ARM homebrew`);
        await fs.rm(path.resolve(homebrewPath), {
            recursive: true
        });
    }
}
