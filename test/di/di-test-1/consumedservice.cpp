#include "consumedservice.h"

#include "iinnerservice.h"

ConsumedService::ConsumedService(T_INJECTED(IInnerService, test)) :
    QObject{nullptr} {
    this->innerService = test;
}

QString ConsumedService::helloWorld() {
    return this->innerService->helloWorld();
}
