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

void DI1::di1_equality1() {
    auto consumedService = diManager->tBaseDIManager::requiredService<ConsumedService>();
    auto consumedService2 = diManager->tBaseDIManager::requiredService<ConsumedService>();
    QCOMPARE(consumedService, consumedService2);
}

void DI1::di1_equality2() {
    auto innerService = diManager->tBaseDIManager::requiredService<IInnerService>();
    auto innerService2 = diManager->tBaseDIManager::requiredService<IInnerService>();
    QCOMPARE(innerService, innerService2);
}

void DI1::di1_helloWorld() {
    auto consumedService = diManager->tBaseDIManager::requiredService<IConsumedService>();
    auto innerService = diManager->tBaseDIManager::requiredService<IInnerService>();

    auto consumedHelloWorld = consumedService->helloWorld();
    auto innerHelloWorld = innerService->helloWorld();
    QCOMPARE(consumedHelloWorld, innerHelloWorld);
}
