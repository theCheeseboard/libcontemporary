#include "tst_di1.h"

#include "consumedservice.h"
#include "innerservice.h"
#include <QtTest>

DI1::DI1() {
    diManager = new tDIManager();
}

DI1::~DI1() {
    delete diManager;
}

void DI1::initTestCase() {
    diManager->addSingleton<IConsumedService, ConsumedService>();
    diManager->addSingleton<IInnerService, InnerService>();
}

void DI1::di1_helloWorld() {
    auto consumedService = diManager->requiredService<IConsumedService>();
    auto innerService = diManager->requiredService<IInnerService>();
    QCOMPARE(consumedService->helloWorld(), innerService->helloWorld());
}
