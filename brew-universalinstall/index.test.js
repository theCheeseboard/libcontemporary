const process = require('process');
const exec = require('@actions/exec');
const installu = require('./installu');
const fs = require('fs');

jest.setTimeout(600000);
const testIfRequired = process.platform === "darwin" ? test : test.skip;

testIfRequired('links library', async () => {
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
            },
            silent: true
        });

        let archs = lipoOutput.trim().split(" ");
        expect(archs).toEqual(["x86_64", "arm64"]);
    }
});
