#ifndef CONSUMEDSERVICE_H
#define CONSUMEDSERVICE_H

#include "iconsumedservice.h"
#include <QObject>
#include <dependencyinjection/tinjectedpointer.h>

class IInnerService;
class ConsumedService : public QObject,
                        public IConsumedService {
        Q_OBJECT
        T_INJECTABLE
    public:
        explicit ConsumedService(tInjectedPointer<IInnerService> test);

        QString helloWorld() override;

    signals:

    private:
        tInjectedPointer<IInnerService> innerService;
};

#endif // CONSUMEDSERVICE_H
