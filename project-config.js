const fs = require("fs/promises");
const path = require("path");

class ProjectConfig {
    static async getProjectConfig(projectDir) {
        let config = new ProjectConfig();
        await config.load(projectDir);
        return config;
    }

    async load(projectDir) {
        let dirFile = path.resolve(".", projectDir, ".contemporary.json");
        this.jsonConfig = await fs.readFile(dirFile, {
            encoding: "utf8"
        });
    }

    value(property) {
        return this.jsonConfig[property];
    }
}

module.exports = ProjectConfig;