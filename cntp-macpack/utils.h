#ifndef UTILS_H
#define UTILS_H

#include <QBuffer>
#include <QJsonObject>

namespace Utils {
    void write32BigEndian(QBuffer& buffer, quint32 value);
    void writeBinaryPlist(QBuffer& buffer, QVariantMap data);
    QByteArray toBinaryPlist(QVariantMap data);
}; // namespace Utils

#endif // UTILS_H
