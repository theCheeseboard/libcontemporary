#ifndef CONSUMEDSERVICE_H
#define CONSUMEDSERVICE_H

#include "iconsumedservice.h"
#include <QObject>
#include <dependencyinjection/tinjectedpointer.h>

class IInnerService;
struct ConsumedServicePrivate;
class ConsumedService : public QObject,
                        public IConsumedService {
        Q_OBJECT
    public:
        T_DI_CONSTRUCTOR explicit ConsumedService(T_INJECT(IInnerService));
        ~ConsumedService();

        QString helloWorld() override;

    signals:

    private:
        ConsumedServicePrivate* d;
};

#endif // CONSUMEDSERVICE_H
