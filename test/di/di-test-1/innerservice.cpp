#include "innerservice.h"

InnerService::InnerService() :
    QObject{nullptr} {
}

QString InnerService::helloWorld() {
    return "Hello World!";
}
