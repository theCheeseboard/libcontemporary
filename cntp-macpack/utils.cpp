#include "utils.h"

#include <QtEndian>

void Utils::write32BigEndian(QBuffer& buffer, quint32 value) {
    auto be = qToBigEndian(static_cast<quint32>(value));
    buffer.write(reinterpret_cast<char*>(&be), sizeof(be));
}
