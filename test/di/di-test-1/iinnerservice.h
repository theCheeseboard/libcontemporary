#ifndef IINNERSERVICE_H
#define IINNERSERVICE_H

#include <QMetaObject>
#include <QString>

class IInnerService {
        Q_GADGET
    public:
        virtual QString helloWorld() = 0;
};

#endif // IINNERSERVICE_H
