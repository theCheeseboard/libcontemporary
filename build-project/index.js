const core = require('@actions/core');
const builder = require('./builder');

async function run() {
  try { 
    const project = core.getInput("project");
    const commitish = core.getInput("commitish");
    const extraCmakeArgs = core.getInput("extra-cmake-args");

    let options = {
      project: project
    };

    if (commitish) options.commitish = commitish;
    if (extraCmakeArgs) options.extraCmakeArgs = extraCmakeArgs;

    await builder(options);
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
