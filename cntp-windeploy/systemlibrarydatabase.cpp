#include "systemlibrarydatabase.h"

#include <QMap>
#include <QSettings>
#include <QDir>
#include <QDirIterator>
#include <QProcess>

struct SystemLibraryDatabasePrivate {
    QMap<QString, QString> libraries;
    QSet<QString> ignoreLibraries;
};

SystemLibraryDatabase::SystemLibraryDatabase(QStringList extraSearchPaths, QString sdkVersion, QString arch, QObject* parent) : QObject(parent) {
    d = new SystemLibraryDatabasePrivate;

    QSettings winSdkSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots", QSettings::NativeFormat);

    //TODO: pass some of these as parameters
    QDir winSdkPath = QDir(winSdkSettings.value("KitsRoot10").toString()).absoluteFilePath(QStringLiteral("Lib/%1/um/%2").arg(sdkVersion, arch));

    for (auto& entry : winSdkPath.entryList(QDir::Files)) {
        d->ignoreLibraries.insert(entry.toLower());
    }

    //Find DLL files
    QStringList searchPaths = extraSearchPaths;
    searchPaths.append(qEnvironmentVariable("Path").split(";"));
    searchPaths.removeAll("");
    searchPaths.removeDuplicates();

    for (QString path : searchPaths) {
        QDir dir(path);
        for (QFileInfo libFile : dir.entryInfoList({"*.dll", "*.DLL"}, QDir::Files)) {
            if (d->libraries.contains(libFile.fileName().toLower())) continue;
            if (!libFile.fileName().toLower().startsWith("msvc") && isInDisallowedPath(libFile.absoluteFilePath().toLower())) continue;
            d->libraries.insert(libFile.fileName().toLower(), libFile.absoluteFilePath());
        }
    }

    // Exclude C++ runtime
    QProcess vsWhere;
    auto vsWherePath = QDir(qEnvironmentVariable("ProgramFiles(x86)")).absoluteFilePath("Microsoft Visual Studio/Installer/vswhere.exe");
    vsWhere.start(vsWherePath, {
        "-prerelease",
        "-latest",
        "-products", "*",
        "-requires", "Microsoft.VisualStudio.Component.VC.Tools.*",
        "-property", "installationPath"
    });
    vsWhere.waitForFinished(-1);

    bool readingLibraries = false;
    while (vsWhere.canReadLine()) {
        auto vsPath = vsWhere.readLine().trimmed();
        auto dllRoot = QDir(vsPath).absoluteFilePath("VC/Redist/MSVC/");
        QDirIterator iterator(dllRoot, {"*.dll"}, QDir::Files, QDirIterator::Subdirectories);
        while (iterator.hasNext()) {
            iterator.next();

            d->ignoreLibraries.insert(iterator.fileName().toLower());
        }
    }

    d->ignoreLibraries << "ucrtbase.dll"
        << "ucrtbased.dll";

    QStringList deepSearchPaths({"/Program Files", "/Program Files (x86)"});
    for (QString path : deepSearchPaths) {
        QDirIterator iterator(
            QDir(path).absolutePath(), {"*.dll"}, QDir::Files, QDirIterator::Subdirectories);
        while (iterator.hasNext()) {
            iterator.next();

            if (d->libraries.contains(iterator.fileName().toLower())) continue;
            d->libraries.insert(iterator.fileName().toLower(), iterator.filePath());
        }
    }
}

SystemLibraryDatabase::~SystemLibraryDatabase() {
    delete d;
}

QString SystemLibraryDatabase::library(QString libraryName) {
    return d->libraries.value(libraryName.toLower());
}

bool SystemLibraryDatabase::ignore(QString libraryName) {
    //            if (IsApiSetImplemented(library.toUtf8().constData())) continue;
    if (libraryName.startsWith("api-ms") || libraryName.startsWith("ext-ms")) return true;

    if (d->ignoreLibraries.contains(libraryName.toLower())) return true;
    libraryName.replace(".dll", ".lib");
    if (d->ignoreLibraries.contains(libraryName.toLower())) return true;

    return false;
}

bool SystemLibraryDatabase::isInDisallowedPath(QString path) {
    QStringList disallowedPaths({"/windows/system32", "/windows/syswow64/"});
    for (QString disallowedPath : disallowedPaths) {
        if (path.contains(disallowedPath)) return true;
    }
    return false;
}
