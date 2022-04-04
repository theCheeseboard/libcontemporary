const core = require('@actions/core');
const builder = require('./builder');

async function run() {
  try { 
    const project = core.getInput("project");
    const commitish = core.getInput("commitish");

    let options = {
      project: project
    };

    if (commitish) options.commitish = commitish;

    builder(options);
  } catch (error) {
    core.setFailed(error.message);
  }
}

run();
