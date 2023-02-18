#include "tst_trange.h"

#include <QtTest>
#include <ranges/trange.h>

TRange::TRange() {

}

TRange::~TRange() {

}

void TRange::trange_canIterate() {
    QList<int> ints({1, 2, 3, 4, 5});
    auto range = tRange(ints);

    auto i = 0;
    for (auto item : range) {
        QCOMPARE(item, ints.at(i));
        i++;
    }
}

void TRange::trange_canMap() {
    QList<int> ints({1, 2, 3, 4, 5});
    auto range = tRange(ints).map<int>([](int x) {
        return x * 2;
    });

    auto i = 0;
    for (auto item : range) {
        QCOMPARE(item, ints.at(i) * 2);
        i++;
    }
}
