#include "consumedservice.h"

#include "iinnerservice.h"

struct ConsumedServicePrivate {
        T_INJECTED(IInnerService);
};

ConsumedService::ConsumedService(T_INJECTED(IInnerService)) :
    QObject{nullptr} {
    d = new ConsumedServicePrivate();
    T_INJECT_SAVE_D(IInnerService);
}

ConsumedService::~ConsumedService() {
    delete d;
}

QString ConsumedService::helloWorld() {
    return T_INJECTED_SERVICE(IInnerService)->helloWorld();
}
