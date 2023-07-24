#include "tst_di1.h"

#include "consumedservice.h"
#include <QtTest>
#include <fakeit.hpp>

DI1::DI1() {
    diManager = new tDIManager();
}

DI1::~DI1() {
    delete diManager;
}

void DI1::initTestCase() {
    auto diManager = static_cast<tDIManager*>(this->diManager);
    diManager->addSingleton<IConsumedService, ConsumedService>();

    fakeit::When(Method(innerServiceMock, helloWorld)).AlwaysReturn(QStringLiteral("Hello World!"));
    diManager->addSingleton<IInnerService>(innerServiceMock.get());
}

void DI1::di1_equality1() {
    auto consumedService = diManager->requiredService<ConsumedService>();
    auto consumedService2 = diManager->requiredService<ConsumedService>();
    QCOMPARE(consumedService, consumedService2);
}

void DI1::di1_equality2() {
    auto innerService = diManager->requiredService<IInnerService>();
    auto innerService2 = diManager->requiredService<IInnerService>();
    QCOMPARE(innerService, innerService2);
}

void DI1::di1_helloWorld() {
    auto consumedService = diManager->requiredService<IConsumedService>();
    auto innerService = diManager->requiredService<IInnerService>();

    auto consumedHelloWorld = consumedService->helloWorld();
    auto innerHelloWorld = innerService->helloWorld();
    QCOMPARE(consumedHelloWorld, innerHelloWorld);
}

void DI1::di1_construct() {
    auto constructed = diManager->construct<ConsumedService>();
    auto consumedService = diManager->requiredService<IConsumedService>();
    auto innerService = diManager->requiredService<IInnerService>();

    QCOMPARE_NE(constructed.dynamicCast<IConsumedService>(), consumedService);

    auto constructedHelloWorld = constructed->helloWorld();
    auto innerHelloWorld = innerService->helloWorld();
    QCOMPARE(constructedHelloWorld, innerHelloWorld);
}
