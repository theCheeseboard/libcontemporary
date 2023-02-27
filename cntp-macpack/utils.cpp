#include "utils.h"

#include <QJsonDocument>
#include <QProcess>
#include <QVariantMap>
#include <QXmlStreamWriter>
#include <QtEndian>

void Utils::write32BigEndian(QBuffer& buffer, quint32 value) {
    auto be = qToBigEndian(static_cast<quint32>(value));
    buffer.write(reinterpret_cast<char*>(&be), sizeof(be));
}

QByteArray Utils::toBinaryPlist(QVariantMap data) {
    QBuffer payload;
    payload.open(QBuffer::WriteOnly);
    payload.write(R"(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">)");

    QXmlStreamWriter writer(&payload);
    writer.writeStartElement("plist");
    writer.writeAttribute("version", "1.0");
    writer.writeStartElement("dict");

    for (auto key : data.keys()) {
        auto value = data.value(key);
        writer.writeStartElement("key");
        writer.writeCharacters(key);
        writer.writeEndElement();

        switch (value.typeId()) {
            case QMetaType::Type::QString:
                writer.writeStartElement("string");
                writer.writeCharacters(value.toString());
                writer.writeEndElement();
                break;
            case QMetaType::Type::Int:
                writer.writeStartElement("integer");
                writer.writeCharacters(QString::number(value.toInt()));
                writer.writeEndElement();
                break;
            case QMetaType::Type::Double:
            case QMetaType::Type::Float:
                writer.writeStartElement("real");
                writer.writeCharacters(QString::number(value.toDouble()));
                writer.writeEndElement();
                break;
            case QMetaType::Type::Bool:
                if (value.toBool()) {
                    writer.writeEmptyElement("true");
                } else {
                    writer.writeEmptyElement("false");
                }
                break;
            case QMetaType::Type::QByteArray:
                writer.writeStartElement("data");
                writer.writeCharacters(value.toByteArray().toBase64());
                writer.writeEndElement();
                break;
        }
    }

    writer.writeEndElement(); // dict
    writer.writeEndElement(); // plist

    payload.close();

    auto payloadData = payload.buffer();
    QProcess proc;
    proc.start("plutil", {"-convert", "binary1", "-o", "-", "--", "-"});
    proc.write(payloadData);
    proc.closeWriteChannel();
    proc.waitForFinished(-1);
    return proc.readAll();
}

void Utils::writeBinaryPlist(QBuffer& buffer, QVariantMap data) {
    auto plistContents = toBinaryPlist(data);
    write32BigEndian(buffer, plistContents.length());
    buffer.write(plistContents);
}
