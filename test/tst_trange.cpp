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

void TRange::trange_canMapToDifferentType()
{
    QList<int> ints({1, 2, 3, 4, 5});
    auto range = tRange(ints).map<QString>([](int x) {
        return QString::number(x);
    });

    auto i = 0;
    for (auto item : range) {
        QCOMPARE(item, QString::number(ints.at(i)));
        i++;
    }
}

void TRange::trange_canFilter()
{
    QList<int> ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    auto range = tRange(ints).filter([](int x) {
        return x % 2 == 0;
    });

    for (auto item : range) {
        QVERIFY(item % 2 == 0);
    }
}

void TRange::trange_canCompose()
{
    QList<int> ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    auto range = tRange(ints).filter([](int x) {
        return x % 2 == 0;
    }).map<QString>([](int x) {
        return QString::number(x);
    });

    for (QString item : range) {
        QVERIFY(item.toInt() % 2 == 0);
    }
}

void TRange::trange_canReturn()
{
    auto createRange = [] {
        QList<int> ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        auto range = tRange(ints).filter([](int x) {
            return x % 2 == 0;
        }).map<QString>([](int x) {
            return QString::number(x);
        });
        return range;
    };

    auto range = createRange();
    for (QString item : range) {
        QVERIFY(item.toInt() % 2 == 0);
    }
}
