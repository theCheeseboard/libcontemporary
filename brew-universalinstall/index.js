const core = require('@actions/core');
const installu = require('./installu');

async function run() {
  try { 
    const packages = core.getInput("packages").split(":");
    await installu({
      packages: packages
    });
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
