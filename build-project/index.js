const core = require('@actions/core');
const builder = require('./builder');

async function run() {
  try { 
    const project = core.getInput("project");
    const commitish = core.getInput("commitish");
    const extraCmakeArgs = core.getInput("extra-cmake-args");
    const arch = core.getInput("arch");
    const forceHostBuild = core.getInput("force-host-build");

    let options = {
      project: project,
      arch: arch,
      forceHostBuild: forceHostBuild === "true"
    };

    if (commitish) options.commitish = commitish;
    if (extraCmakeArgs) options.extraCmakeArgs = extraCmakeArgs;

    await builder(options);
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
