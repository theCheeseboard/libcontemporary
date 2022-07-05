/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2022 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#ifndef TASYNC_H
#define TASYNC_H

#include "tlogger.h"
#include <QFuture>
#include <QFutureWatcher>
#include <QTimer>
#include <functional>

#ifndef T_NO_COROUTINES
    #include <coroutine>
#endif

template<typename T> class tAsync {
    public:
        tAsync(QFuture<T> future) {
            this->future = future;
        }

        tAsync(QPromise<T>* promise) {
            this->future = promise->future();
        }

        tAsync<T>& then(QObject* context, std::function<void(T)> callback) {
            auto future = this->future;
            if (future.isFinished()) {
                QTimer::singleShot(0, context, [future, callback] {
                    try {
                        callback(future.result());
                    } catch (QException& exception) {
                        // Ignore
                    }
                });
            } else {
                auto* watcher = new QFutureWatcher<T>(context);
                watcher->setFuture(this->future);
                QObject::connect(watcher, &QFutureWatcher<T>::finished, context, [watcher, future, callback] {
                    watcher->deleteLater();
                    try {
                        callback(future.result());
                    } catch (QException& exception) {
                        // Ignore
                    }
                });
            }
            return *this;
        }

        tAsync<T>& error(QObject* context, std::function<void(QException)> callback) {
            auto future = this->future;
            if (future.isFinished()) {
                QTimer::singleShot(0, context, [future, callback] {
                    try {
                        future.result();
                    } catch (QException& exception) {
                        callback(exception);
                    }
                });
            } else {
                auto* watcher = new QFutureWatcher<T>(context);
                watcher->setFuture(future);
                QObject::connect(watcher, &QFutureWatcher<T>::finished, context, [watcher, future, callback] {
                    watcher->deleteLater();
                    try {
                        future.result();
                    } catch (QException& exception) {
                        callback(exception);
                    }
                });
            }
            return *this;
        }

        T await() {
            this->future.waitForFinished();
            return this->future.result();
        }

    private:
        QFuture<T> future;
};

// template<> class tAsync<void> {
//     public:
//         tAsync(QFuture<void> future) {
//             this->future = future;
//         }

//        tAsync(QPromise<void>* promise) {
//            this->future = promise->future();
//        }

//        tAsync<void>& then(QObject* context, std::function<void()> callback) {
//            auto future = this->future;
//            if (future.isFinished()) {
//                QTimer::singleShot(0, context, [future, callback] {
//                    try {
//                        callback(future.result());
//                    } catch (QException& exception) {
//                        // Ignore
//                    }
//                });
//            } else {
//                auto* watcher = new QFutureWatcher<void>(context);
//                watcher->setFuture(this->future);
//                QObject::connect(watcher, &QFutureWatcher<void>::finished, context, [watcher, future, callback] {
//                    watcher->deleteLater();
//                    try {
//                        callback(future.result());
//                    } catch (QException& exception) {
//                        // Ignore
//                    }
//                });
//            }
//            return *this;
//        }

//        tAsync<void>& error(QObject* context, std::function<void(QException)> callback) {
//            auto future = this->future;
//            if (future.isFinished()) {
//                QTimer::singleShot(0, context, [future, callback] {
//                    try {
//                        future.result();
//                    } catch (QException& exception) {
//                        callback(exception);
//                    }
//                });
//            } else {
//                auto* watcher = new QFutureWatcher<void>(context);
//                watcher->setFuture(future);
//                QObject::connect(watcher, &QFutureWatcher<void>::finished, context, [watcher, future, callback] {
//                    watcher->deleteLater();
//                    try {
//                        future.result();
//                    } catch (QException& exception) {
//                        callback(exception);
//                    }
//                });
//            }
//            return *this;
//        }

//        void await() {
//            this->future.waitForFinished();
//        }

//    private:
//        QFuture<void> future;
//};

#endif // TASYNC_H
