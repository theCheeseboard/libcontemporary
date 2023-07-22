#include "innerservice.h"

InnerService::InnerService() :
    QObject{nullptr} {
}

QString InnerService::helloWorld() {
    return "Hello World!";
}

QObject* InnerService::cntp_inject_construct(QList<tInjectedPointer<QObject>>) {
    return new InnerService();
}
