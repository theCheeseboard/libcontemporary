const core = require('@actions/core');
const path = require("path");
const fs = require("fs/promises");

async function run() {
    await installMatcher("gcc-matcher.json");
}

async function installMatcher(file) {
    const matcherPath = path.join(__dirname, file)
    try {
        await fs.stat(matcherPath);
        core.info(`::add-matcher::${matcherPath}`)
    } catch {
        return core.warning(`Can't find matcher ${file}`);
    }
}

run();