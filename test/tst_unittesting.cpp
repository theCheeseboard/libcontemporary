/*******************************************************************************
 *
 * libcontemporary - Common libraries
 * Copyright (C) 2022 Victor Tran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QRandomGenerator>

#include <tnotification.h>
#include <tsystemsound.h>
#include <tpromise.h>

QString generateRandomString() {
    QString characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNNOPQRSTUVWXYZ1234567890!@#$%^&*()-=_+[]{}\\|;':\",.<>/?`~";
    QRandomGenerator* generator = QRandomGenerator::system();
    QString finalString;
    int length = generator->bounded(128);
    for (int i = 0; i < length; i++) {
        finalString.append(characters.at(generator->bounded(characters.length())));
    }
    return finalString;
}

class UnitTesting : public QObject
{
        Q_OBJECT

    public:
        UnitTesting();

    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();

        void sound();
        void notification();

        void voidPromise();
        void valuePromise();
        void asyncVoidPromise();
        void asyncValuePromise();
        void promiseRejection();
        void asyncPromiseRejection();
};

UnitTesting::UnitTesting()
{
}

void UnitTesting::initTestCase()
{

}

void UnitTesting::cleanupTestCase()
{

}

void UnitTesting::notification()
{
    tNotification* notification = new tNotification("Notification", "This is a test notification");
    notification->insertAction("okay", "With Actions!");
    notification->post(false);

    QVERIFY(true);
}

void UnitTesting::voidPromise()
{
    QDateTime now = QDateTime::currentDateTime();
    (new tPromise<void>([=](QString err) {
        QThread::sleep(1);
    }))->await();

    //Allow some room for error
    QVERIFY(now.msecsTo(QDateTime::currentDateTime()) > 700);
}

void UnitTesting::valuePromise()
{
    QString testString = generateRandomString();
    tPromiseResults<QString> results = (new tPromise<QString>([=](QString err) {
        return testString;
    }))->await();

    QCOMPARE(results.result, testString);
}

void UnitTesting::asyncVoidPromise()
{
    QDateTime now = QDateTime::currentDateTime();
    (new tPromise<void>([=](auto res, auto rej) {
        QTimer::singleShot(1000, res);
    }))->await();

    //Allow some room for error
    QVERIFY(now.msecsTo(QDateTime::currentDateTime()) > 700);
}

void UnitTesting::asyncValuePromise()
{
    QString testString = generateRandomString();
    tPromiseResults<QString> results = (new tPromise<QString>([=](auto res, auto rej) {
        res(testString);
    }))->await();

    QCOMPARE(results.result, testString);
}

void UnitTesting::promiseRejection()
{
    tPromiseResults<void> results = (new tPromise<void>([=](QString& err) {
        err = "An error";
    }))->await();

    QCOMPARE(results.error, "An error");
}

void UnitTesting::asyncPromiseRejection()
{
    tPromiseResults<void> results = (new tPromise<void>([=](auto res, auto rej) {
        rej("An error");
    }))->await();

    QCOMPARE(results.error, "An error");
}


void UnitTesting::sound() {
    tSystemSound* sound = tSystemSound::play("desktop-login");
    if (sound == nullptr) {
        //Didn't work
        QVERIFY(false);
    } else {
        QEventLoop* loop = new QEventLoop();
        connect(sound, &tSystemSound::done, [=] {
            loop->quit();
        });
        loop->exec();
        loop->deleteLater();

        QVERIFY(true);
    }
}

QTEST_MAIN(UnitTesting)

#include "tst_unittesting.moc"
