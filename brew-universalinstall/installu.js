const core = require('@actions/core');
const exec = require('@actions/exec');
const io = require('@actions/io');
const fs = require('fs/promises');
const legacyFs = require('fs');
const https = require('follow-redirects').https;
const tar = require('tar-fs');
const stream = require('stream');
const gunzip = require('gunzip-maybe');
const path = require('path');
const clone = require('git-clone/promise');

const mergeExts = ["", ".dylib", ".a"];

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
    console.log(`Merging: arm: ${arm}, sys: ${system}`);
    await exec.exec("lipo", ["-create", arm, system, "-output", system], {
        ignoreReturnCode: true,
        silent: true
    });
}

async function walkDirectory(dir) {
    let dirContents = await fs.readdir(dir, {
        withFileTypes: true
    });

    dirContents = await Promise.all(dirContents.map(async dirent => {
        let name = path.resolve(dir, dirent.name);
        if (dirent.isSymbolicLink()) {
            return [];
        } else if (dirent.isDirectory()) {
            return await walkDirectory(name);
        } else {
            return name;
        }
    }));
    return dirContents.flat();
}

module.exports = async function(options) {
    let homebrewPath = "./homebrew";

    let armCellar = path.resolve(homebrewPath);
    await io.mkdirP(armCellar);

    try {
        let bottlePaths = [];
        let bottleNames = [];
        for (let pk of options.packages) {
            console.log(`Processing package ${pk}`);
            //Install x86_64 version
            let x86install = exec.exec("brew", ["install", pk], {
                silent: true
            });
            await x86install;

            let armBrewOutput = "";
            await exec.exec("brew", ["fetch", "--deps", "--bottle-tag=arm64_big_sur", pk], {
                listeners: {
                     stdout: data => {
                         armBrewOutput += data.toString();

                         let currentOutput = armBrewOutput.split("\n");
                         while (currentOutput.length > 1) {
                             let line = currentOutput.shift();
                             if (line.startsWith("Fetching: ")) {
                                 bottleNames = line.substr(10).split(", ");
                             } else if (line.startsWith("Downloaded to:") || (line.startsWith("Already downloaded:") && line.endsWith(".bottle.tar.gz"))) {
                                 let downloadedFile = line.split(" ")[2];
                                 console.log(`Downloaded ${bottleNames[bottlePaths.length]}`);
                                 bottlePaths.push(downloadedFile);
                             }
                         }
                         armBrewOutput = currentOutput[0];
                     }
                },
                silent: true
            });
        }

        bottlePaths = bottlePaths.filter((path, index) => {
            return bottlePaths.indexOf(path) === index;
        })

        await Promise.all(bottlePaths.map(async bottlePath => {
            //Untar all bottles to the cellar
            let extractStream = legacyFs.createReadStream(bottlePath).pipe(gunzip()).pipe(tar.extract(armCellar, {
                //TODO: also detect headers and resources with a framework and filter those out
                ignore: name => !mergeExts.includes(path.extname(name))
            }));
            await new Promise(res => extractStream.on("finish", res));
        }));

        console.log("Merging libraries");

        //Walk the directory and lipo files together
        let brewFiles = await walkDirectory(armCellar);

        for (let library of brewFiles) {
            let rel = path.relative(armCellar, library);
            await lipoIfRequired(library, path.resolve("/usr/local/Cellar", rel));
        }
    } finally {
        console.log(`Removing ARM homebrew`);
        await fs.rm(path.resolve(armCellar), {
            recursive: true
        });
    }
}
