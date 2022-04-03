const process = require('process');
const cp = require('child_process');
const path = require('path');
const exec = require('@actions/exec');
const installu = require('./installu');
const fs = require('fs');

test('links library', async () => {
    jest.setTimeout(300000);

    await installu({
        packages: ["libtag", "libmusicbrainz"]
    });

    let toCheck = [
        "/usr/local/lib/libtag.dylib",
        "/usr/local/lib/libneon.dylib", //from neon, dependency of libmusicbrainz
        "/usr/local/lib/libmusicbrainz5.dylib"
    ];

    for (let item of toCheck) {
        expect(fs.existsSync(item)).toBe(true);

        let lipoOutput = "";
        await exec.exec("lipo", ["-archs", item], {
            listeners: {
                stdout: data => lipoOutput += data.toString()
            }
        });

        let archs = lipoOutput.trim().split(" ");
        expect(archs).toEqual(["x86_64", "arm64"]);
    }
});
