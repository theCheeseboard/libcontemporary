#include "consumedservice.h"

#include "iinnerservice.h"
#include "innerservice.h"

ConsumedService::ConsumedService(tInjectedPointer<IInnerService> test) :
    QObject{nullptr} {
    auto innerService = test.reinterpretCast<InnerService>();
    auto metaobject = innerService->metaObject();
    auto metaobjectName = QString::fromUtf8(metaobject->className());
    this->innerService = test;
}

QString ConsumedService::helloWorld() {
    return this->innerService->helloWorld();
}
