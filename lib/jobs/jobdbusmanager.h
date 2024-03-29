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
#ifndef JOBDBUSMANAGER_H
#define JOBDBUSMANAGER_H

#include <QObject>
#include <QDBusObjectPath>

class tJob;
struct JobDbusManagerPrivate;
class JobDbusManager : public QObject {
        Q_OBJECT
    Q_SCRIPTABLE Q_PROPERTY(QString ApplicationName READ applicationName)
                Q_SCRIPTABLE Q_PROPERTY(QString ApplicationDesktopEntry READ applicationDesktopEntry)
        Q_CLASSINFO("D-Bus Interface", "com.vicr123.libcontemporary.tjob.Manager")
    public:
        explicit JobDbusManager(QObject* parent = nullptr);
        ~JobDbusManager();

        QString applicationName();
        QString applicationDesktopEntry();

    public slots:
        Q_SCRIPTABLE QList<QDBusObjectPath> Jobs();

    signals:
        Q_SCRIPTABLE void JobAdded(QDBusObjectPath job);

    private:
        JobDbusManagerPrivate* d;

        void addJob(tJob* job);
};

#endif // JOBDBUSMANAGER_H
