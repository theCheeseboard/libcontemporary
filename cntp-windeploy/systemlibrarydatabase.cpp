#include "systemlibrarydatabase.h"

#include <QMap>
#include <QSettings>
#include <QDir>
#include <QDirIterator>

struct SystemLibraryDatabasePrivate {
    QMap<QString, QString> libraries;
    QStringList ignoreLibraries;
};

SystemLibraryDatabase::SystemLibraryDatabase(QStringList extraSearchPaths, QObject *parent) : QObject(parent) {
    d = new SystemLibraryDatabasePrivate;

    QSettings winSdkSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots", QSettings::NativeFormat);

    //TODO: pass some of these as parameters
    QDir winSdkPath = QDir(winSdkSettings.value("KitsRoot10").toString()).absoluteFilePath("Lib/10.0.22000.0/um/x64");
    d->ignoreLibraries = winSdkPath.entryList(QDir::Files);


    //Find DLL files
    QStringList searchPaths = extraSearchPaths;
    searchPaths.append(qEnvironmentVariable("Path").split(";"));
    searchPaths.removeAll("");
    searchPaths.removeDuplicates();


    for (QString path : searchPaths) {
        QDir dir(path);
        for (QFileInfo libFile: dir.entryInfoList({"*.dll", "*.DLL"}, QDir::Files)) {
            if (d->libraries.contains(libFile.fileName().toLower())) continue;
            if (!libFile.fileName().toLower().startsWith("msvc") && isInDisallowedPath(libFile.absoluteFilePath().toLower())) continue;
            d->libraries.insert(libFile.fileName().toLower(), libFile.absoluteFilePath());
        }
    }

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

    libraryName.replace(".dll", ".lib");
    if (d->ignoreLibraries.contains(libraryName.toLower(), Qt::CaseInsensitive)) return true;

    return false;
}

bool SystemLibraryDatabase::isInDisallowedPath(QString path) {
    QStringList disallowedPaths({"/windows/system32", "/windows/syswow64/"});
    for (QString disallowedPath : disallowedPaths) {
        if (path.contains(disallowedPath)) return true;
    }
    return false;
}
