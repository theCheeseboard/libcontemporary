#include <QCoreApplication>
#include <QCommandLineParser>
#include "deployfolder.h"
#include "common.h"
#include "systemlibrarydatabase.h"

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("directory", "Directory containing application prepare for deployment");
    QCommandLineOption helpOption = parser.addHelpOption();
    parser.parse(a.arguments());

    QTextStream output(stdout);
    QTextStream eoutput(stderr);

    if (parser.isSet(helpOption)) {
        output << parser.helpText() << "\n";
        return 0;
    }

    if (parser.positionalArguments().length() != 1) {
        eoutput << "error: incorrect number of arguments.\n";
        return 1;
    }


    QStringList contemporaryPath = Common::findInPaths("contemporary.dll", a.libraryPaths(), true);
    if (contemporaryPath.isEmpty()) {
        eoutput << "error: Contemporary theme not installed.\n";
        return 1;
    }

    output << "Copying Qt plugins\n";
    output.flush();
    DeployFolder deployFolder(parser.positionalArguments().first());
    QFile::copy(contemporaryPath.first(), deployFolder.destinationDir(DeployFolder::DeployFolderDirectories::QtStylePlugins).absoluteFilePath("contemporary.dll"));

    output << "Searching for support libraries...\n";
    output.flush();
    QStringList supportLibraryPaths;
    QDir rootProjectDir = deployFolder.destinationDir().absoluteFilePath("../../../");
    for (QString markerFile: Common::findInPaths(".cntp-is-support-library", {rootProjectDir.absolutePath()}, true)) {
        QFileInfo markerFileInfo(markerFile);
        QDir libDir = markerFileInfo.dir();

        //Search for libraries
        for (QFileInfo libraryInfo: libDir.entryInfoList({"*.dll"}, QDir::Files)) {
            supportLibraryPaths.append(libraryInfo.dir().absolutePath());
        }
    }

    QStringList extraSearchPaths;
    extraSearchPaths.append(supportLibraryPaths);

    output << "Creating System Library Database...\n";
    output.flush();
    SystemLibraryDatabase *libraryDatabase = new SystemLibraryDatabase(supportLibraryPaths);

    output << "Making folder self contained\n";
    output.flush();
    deployFolder.makeSelfContained(libraryDatabase);

    return 0;
}