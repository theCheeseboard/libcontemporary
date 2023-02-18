#include <QtTest>

#include "tst_trange.h"

int main(int argc, char* argv[]) {
    int status = 0;
    status |= QTest::qExec(new TRange, argc, argv);

    return status;
}
