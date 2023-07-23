#ifndef TST_DI1_H
#define TST_DI1_H

#include "iinnerservice.h"
#include <QObject>
#include <dependencyinjection/tdimanager.h>
#include <fakeit.hpp>

class DI1 : public QObject {
        Q_OBJECT
    public:
        DI1();
        ~DI1();

    private slots:
        void initTestCase();
        void di1_equality1();
        void di1_equality2();
        void di1_helloWorld();

    private:
        tBaseDIManager* diManager;

        fakeit::Mock<IInnerService> innerServiceMock;
};

#endif // TST_DI1_H
