#ifndef INNERSERVICE_H
#define INNERSERVICE_H

#include "iinnerservice.h"
#include <QObject>

class InnerService : public QObject,
                     public IInnerService {
        Q_OBJECT
    public:
        Q_SCRIPTABLE explicit InnerService();

    signals:

        // IInnerService interface
    public:
        QString helloWorld() override;
};

#endif // INNERSERVICE_H
