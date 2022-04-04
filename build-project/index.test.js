const exec = require('@actions/exec');
const builder = require('./builder');
const fs = require('fs');

jest.setTimeout(600000);

beforeAll(async () => {
    //Install required dependencies
    switch (process.platform) {
        case "darwin": {
            const installu = require('../brew-universalinstall/installu');
            await installu({
                packages: ["qt", "cmake", "ninja"]
            });
        }
    }
});

test('builds libcontemporary', async () => {
    await builder({
        project: "vicr123/libcontemporary",
        commitish: "blueprint"
    });

    let toCheck = [];
    switch (process.platform) {
        case "darwin":
            toCheck.push("/usr/local/lib/libcontemporary.framework/libcontemporary");
            break;
    }

    for (let item of toCheck) {
        expect(fs.existsSync(item)).toBe(true);
    }
});

const testIfMac = process.platform === "darwin" ? test : test.skip;

testIfMac('libcontemporary is universal', async () => {
    let lipoOutput = "";
    await exec.exec("lipo", ["-archs", "/usr/local/lib/libcontemporary.framework/libcontemporary"], {
        listeners: {
            stdout: data => lipoOutput += data.toString()
        },
        silent: true
    });

    let archs = lipoOutput.trim().split(" ");
    expect(archs).toEqual(["x86_64", "arm64"]);
});
