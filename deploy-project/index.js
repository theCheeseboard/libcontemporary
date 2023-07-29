const core = require('@actions/core');
const deploy = require('./deploy');

async function run() {
    try {
        const subdir = core.getInput("subdir");
        const arch = core.getInput("arch");
        let options = {
            subdir: subdir,
            arch: arch
        };

        await deploy(options);
    } catch (error) {
        core.setFailed(error.message);
    }
}

run();
