#include <QtTest>

#include "tst_di1.h"
#include <tlogger.h>

int main(int argc, char* argv[]) {
    tLogWriter::setTestMode(true);

    int status = 0;
    status |= QTest::qExec(new DI1, argc, argv);

    return status;
}
