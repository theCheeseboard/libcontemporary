#ifndef UTILS_H
#define UTILS_H

#include <QBuffer>

namespace Utils {
    void write32BigEndian(QBuffer& buffer, quint32 value);
};

#endif // UTILS_H
