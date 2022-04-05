const core = require('@actions/core');
const deploy = require('./deploy');

async function run() {
    try {
        const subdir = core.getInput("subdir");
        let options = {
            subdir: subdir
        };

        await deploy(options);
    } catch (error) {
        core.setFailed(error.message);
    }
}

run();
