#include "dsstoreentry.h"

#include "utils.h"
#include <QStringEncoder>
#include <QVariantMap>
#include <QtEndian>

struct DsStoreEntryPrivate {
        QString filename;
        QString structureId;
        QString dataType;
        QByteArray buffer;
};

DsStoreEntry::~DsStoreEntry() {
    delete d;
}

DsStoreEntry& DsStoreEntry::operator=(const DsStoreEntry& other) {
    d->filename = other.d->filename;
    d->structureId = other.d->structureId;
    d->dataType = other.d->dataType;
    d->buffer = other.d->buffer;
    return *this;
}

DsStoreEntry DsStoreEntry::entryFor(QString filename, QString structureId, QVariantMap options) {
    QString dataType;
    QBuffer blob;
    blob.open(QBuffer::WriteOnly);
    if (structureId == "BKGD") {
        dataType = "blob";

        Utils::write32BigEndian(blob, 8);

        if (options.contains("pictureByteLength")) {
            blob.write(QStringLiteral("PctB").toLatin1());

            Utils::write32BigEndian(blob, options.value("pictureByteLength").toInt());
        } else {
            blob.write(QStringLiteral("DefB").toLatin1());
        }
    } else if (structureId == "Iloc") {
        dataType = "blob";

        Utils::write32BigEndian(blob, 12);

        Utils::write32BigEndian(blob, options.value("x").toInt());
        Utils::write32BigEndian(blob, options.value("y").toInt());
        const char* sentinel = "\xFF\xFF\xFF\x00";
        blob.write(sentinel, 4);
    } else if (structureId == "vSrn") {
        dataType = "long";
        Utils::write32BigEndian(blob, options.value("value").toInt());
    }

    blob.close();
    return DsStoreEntry(filename, structureId, dataType, blob.data());
}

quint64 DsStoreEntry::length() {
    return d->buffer.length();
}

QByteArray DsStoreEntry::buffer() {
    return d->buffer;
}

DsStoreEntry::DsStoreEntry(QString filename, QString structureId, QString dataType, QByteArray blob) {
    d = new DsStoreEntryPrivate();
    d->filename = filename;
    d->structureId = structureId;
    d->dataType = dataType;

    for (auto c : qAsConst(filename)) {
        if (c.decompositionTag() == QChar::Canonical) {
            d->filename += c.decomposition();
        } else {
            d->filename += c;
        }
    }

    QStringEncoder converter(QStringEncoder::Utf16BE);

    auto filenameLength = filename.length();

    QBuffer buf(&d->buffer);
    buf.open(QBuffer::WriteOnly);
    Utils::write32BigEndian(buf, filenameLength);
    buf.write(converter.encode(filename));
    buf.write(structureId.toLatin1());
    buf.write(dataType.toLatin1());
    buf.write(blob);
    buf.close();
}

DsStoreEntry::DsStoreEntry(const DsStoreEntry& other) {
    d = new DsStoreEntryPrivate();
    d->filename = other.d->filename;
    d->structureId = other.d->structureId;
    d->dataType = other.d->dataType;
    d->buffer = other.d->buffer;
}

DsStoreEntry::DsStoreEntry(DsStoreEntry&& other) {
    d = other.d;
    other.d = new DsStoreEntryPrivate();
}
