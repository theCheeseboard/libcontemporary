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

#include "nativeeventfilter.h"

#include <QDebug>

#ifdef Q_OS_WIN
    #include <Windows.h>
#endif

using namespace theLibsPrivate;

NativeEventFilter::NativeEventFilter(QObject* parent) :
    QObject(parent), QAbstractNativeEventFilter() {
}

bool NativeEventFilter::filter(const QByteArray& eventType, void* message) {
#ifdef Q_OS_WIN
    if (eventType == "windows_generic_MSG") {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_POWERBROADCAST) {
            if (msg->wParam == PBT_POWERSETTINGCHANGE) {
                POWERBROADCAST_SETTING* powerSettings = reinterpret_cast<POWERBROADCAST_SETTING*>(msg->lParam);
                if (powerSettings->PowerSetting == GUID_POWER_SAVING_STATUS) {
                    bool powerSaverOn = powerSettings->Data[0];
                    emit powerStretchChanged(powerSaverOn);
                }
            }
        }
    }
#endif
    return false;
}

#ifdef T_QT_5
bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result) {
    return filter(eventType, message);
}
#endif

#ifdef T_QT_6
bool theLibsPrivate::NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    return filter(eventType, message);
}
#endif
