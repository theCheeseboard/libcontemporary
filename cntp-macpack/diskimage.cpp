#include "diskimage.h"
#include <QProcess>
#include <QRegularExpression>
#include <QThread>
#include <math.h>

struct DiskImagePrivate {
        QString diskImagePath;
        QString mountPath;
        bool mounted = false;
        bool valid = true;

        static QRegularExpression mountPathSearch;
};

QRegularExpression DiskImagePrivate::mountPathSearch = QRegularExpression("\\s+(\\/Volumes\\/.+)$", QRegularExpression::MultilineOption);

DiskImage::DiskImage(qulonglong size, QString name, QString output, QString filesystem, QObject* parent) :
    QObject{parent} {
    d = new DiskImagePrivate();
    d->diskImagePath = output;

    QProcess hdiutilProc;
    hdiutilProc.start("hdiutil", {"create", output, "-ov", "-fs", filesystem, "-size", QString::number(size), "-volname", name});
    hdiutilProc.waitForFinished(-1);
    auto out = hdiutilProc.readAllStandardError();
    if (hdiutilProc.exitCode() != 0) {
        d->valid = false;
        return;
    }
}

DiskImage::~DiskImage() {
    if (d->mounted) this->unmount();
    delete d;
}

bool DiskImage::convert(QString input, QString output, QString format) {
    QProcess hdiutilProc;
    hdiutilProc.start("hdiutil", {"convert", input, "-ov", "-format", format, "-imagekey", "zlib-level=9", "-o", output});
    hdiutilProc.waitForFinished(-1);
    return hdiutilProc.exitCode() == 0;
}

bool DiskImage::isValid() {
    return d->valid;
}

bool DiskImage::mount() {
    if (d->mounted) return true;

    QProcess hdiutilProc;
    hdiutilProc.start("hdiutil", {"attach", d->diskImagePath, "-nobrowse", "-noverify", "-noautoopen"});
    hdiutilProc.waitForFinished(-1);
    auto out = hdiutilProc.readAllStandardError();
    if (hdiutilProc.exitCode() != 0) {
        return false;
    }

    auto matches = DiskImagePrivate::mountPathSearch.match(QString(hdiutilProc.readAllStandardOutput()));
    if (!matches.hasMatch()) {
        return false;
    }

    d->mountPath = matches.captured(1);
    d->mounted = true;
    return true;
}

void DiskImage::unmount() {
    if (!d->mounted) return;

    int attempts = 0;

    do {
        QProcess hdiutilProc;
        hdiutilProc.start("hdiutil", {"detach", d->mountPath});
        hdiutilProc.waitForFinished(-1);
        if (hdiutilProc.exitCode() != 16) {
            return;
        }

        QThread::sleep(pow(2, attempts));
        attempts++;
    } while (attempts < 8);
}

QDir DiskImage::mountPath() {
    return d->mountPath;
}
