#include "alias.h"
#include "diskimage.h"
#include "dsstore.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QSvgRenderer>
#include <QTemporaryDir>

void copyDir(QDir dir, QString to) {
    QDir toDir(to);
    QDir::root().mkpath(toDir.absolutePath());

    auto entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const auto& entry : entries) {
        auto destination = toDir.absoluteFilePath(entry.fileName());
        if (entry.isDir()) {
            copyDir(entry.absoluteFilePath(), destination);
        } else if (entry.isFile()) {
            QFile file(entry.absoluteFilePath());
            file.open(QFile::ReadOnly);

            QFile destFile(destination);
            destFile.open(QFile::WriteOnly);

            qulonglong read = 0;
            while (read < file.size()) {
                auto buf = file.read(8192);
                destFile.write(buf);
                read += buf.length();
            }
            destFile.close();
            file.close();

            destFile.setPermissions(file.permissions());
        } else if (entry.isSymLink()) {
            QFile::link(entry.symLinkTarget(), destination);
        }
    }
}

int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("bundle", "Application bundle to package");
    parser.addPositionalArgument("output", "Output path of the final DMG");
    parser.addOption({
        {"c", "contemporary-json"},
        "Path to .contemporary.json file (optional)", "contemporary-json"
    });
    QCommandLineOption helpOption = parser.addHelpOption();
    parser.parse(a.arguments());

    QTextStream output(stdout);
    QTextStream eoutput(stderr);
    if (parser.isSet(helpOption)) {
        output << parser.helpText() << "\n";
        return 0;
    }

    if (parser.positionalArguments().length() != 2) {
        eoutput << "error: incorrect number of arguments.\n";
        return 1;
    }

    auto bundle = QDir(parser.positionalArguments().at(0));
    auto dmgOut = parser.positionalArguments().at(1);

    if (!bundle.exists()) {
        eoutput << "error: application bundle does not exist.\n";
        return 1;
    }

    QString contemporaryJsonFilePath;
    if (parser.isSet("contemporary-json")) {
        contemporaryJsonFilePath = parser.value("contemporary-json");
    } else {
        // Attempt to find .contemporary.json by recursively searching the bundle's parent folders
        auto dir = QFileInfo(bundle.absolutePath()).dir();
        while (!QFile::exists(dir.absoluteFilePath(".contemporary.json")) && !dir.isRoot()) {
            dir = QFileInfo(dir.absolutePath()).dir();
        }
        contemporaryJsonFilePath = dir.absoluteFilePath(".contemporary.json");
    }

    auto contemporaryJsonFile = QFile(contemporaryJsonFilePath);
    if (!contemporaryJsonFile.exists()) {
        eoutput << "error: unable to find .contemporary.json file.\n";
        return 1;
    }

    contemporaryJsonFile.open(QFile::ReadOnly);
    QJsonParseError parseError;
    auto contemporaryJson = QJsonDocument::fromJson(contemporaryJsonFile.readAll(), &parseError).object();
    contemporaryJsonFile.close();
    if (parseError.error != QJsonParseError::NoError) {
        eoutput << "error: unable to parse .contemporary.json file.\n";
        return 1;
    }

    if (!contemporaryJson.contains("dmg")) {
        eoutput << "error: .contemporary.json file does not contain dmg section.\n";
        return 1;
    }
    auto dmg = contemporaryJson.value("dmg").toObject();

    if (!dmg.contains("title")) {
        eoutput << "error: .contemporary.json file does not contain dmg title.\n";
        return 1;
    }

    if (!dmg.contains("background")) {
        eoutput << "error: .contemporary.json file does not contain dmg background file.\n";
        return 1;
    }

    QDir contemporaryJsonDir = QFileInfo(contemporaryJsonFilePath).dir();

    // Count the size of the application bundle
    qulonglong size = 0;
    QDirIterator iterator(bundle, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        auto fileInfo = iterator.nextFileInfo();
        if (fileInfo.isFile()) {
            size += fileInfo.size() * 1.5 + 32;
        }
    }

    QTemporaryDir tempDir;
    {
        auto temporaryImage = DiskImage(size, dmg.value("title").toString(), tempDir.filePath("testdmg.dmg"));
        if (!temporaryImage.isValid()) {
            eoutput << "error: unable to create temporary disk image.\n";
            return 1;
        }

        if (!temporaryImage.mount()) {
            eoutput << "error: unable to mount temporary disk image.\n";
            return 1;
        }

        // Render out the background

        auto backgroundSourceFile = contemporaryJsonDir.absoluteFilePath(dmg.value("background").toString());
        if (!QFile::exists(backgroundSourceFile)) {
            eoutput << "error: background source file does not exist.\n";
            return 1;
        }

        QSvgRenderer renderer(backgroundSourceFile);
        if (!renderer.isValid()) {
            eoutput << "error: unable to load background file.\n";
            return 1;
        }

        if (!renderer.elementExists("applications")) {
            eoutput << "error: background SVG does not contain an element with ID #applications.\n";
            return 1;
        }

        if (!renderer.elementExists("app")) {
            eoutput << "error: background SVG does not contain an element with ID #app.\n";
            return 1;
        }

        temporaryImage.mountPath().mkdir(".background");
        auto backgroundDir = QDir(temporaryImage.mountPath().absoluteFilePath(".background"));

        QSize imageSize = renderer.defaultSize();
        double qualityScale = 2;

        const double dpiToDpm = 39.37;

        auto finderScale = imageSize.width() / renderer.viewBoxF().width();

        auto applicationsLinkLocation = (renderer.transformForElement("applications").mapRect(renderer.boundsOnElement("applications")).center().toPoint() * finderScale) - QPoint(0, 10);
        auto appLocation = (renderer.transformForElement("app").mapRect(renderer.boundsOnElement("app")).center().toPoint() * finderScale) - QPoint(0, 10);

        QImage image(imageSize * qualityScale, QImage::Format_ARGB32);
        image.setDotsPerMeterX(72 * dpiToDpm * qualityScale);
        image.setDotsPerMeterY(72 * dpiToDpm * qualityScale);
        QPainter painter(&image);
        renderer.render(&painter, QRectF(QPointF(0, 0), imageSize * qualityScale));
        painter.end();

        image.save(backgroundDir.absoluteFilePath("background.tiff"), "TIFF");

        // Copy the application bundle
        copyDir(bundle, temporaryImage.mountPath().absoluteFilePath(bundle.dirName()));

        // Create link to Applications folder
        QFile::link("/Applications", temporaryImage.mountPath().absoluteFilePath("Applications"));

        // Create DS_Store file
        DsStore dsStore;
        dsStore.setWindowGeometry(QRect(QPoint(100, 100), imageSize));
        dsStore.setWindowProperties(48, Alias::aliasFor(backgroundDir.absoluteFilePath("background.tiff")));
        dsStore.vSrn(1);
        dsStore.moveIcon("Applications", applicationsLinkLocation.x(), applicationsLinkLocation.y());
        dsStore.moveIcon(bundle.dirName(), appLocation.x(), appLocation.y());
        dsStore.write(temporaryImage.mountPath().absoluteFilePath(".DS_Store"));
    }

    // Finalise the disk image
    if (!DiskImage::convert(tempDir.filePath("testdmg.dmg"), dmgOut)) {
        eoutput << "error: unable to create compressed image at the output.\n";
        return 1;
    }
    output << "Created deployable disk image at " << dmgOut << "\n";
    return 0;
}
