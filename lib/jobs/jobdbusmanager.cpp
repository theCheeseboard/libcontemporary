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
#include "jobdbusmanager.h"

#include "jobdbus.h"
#include "tjob.h"
#include "tjobmanager.h"
#include <QDBusConnection>
#include <tapplication.h>

#include "manageradaptor.h"

struct JobDbusManagerPrivate {
        QList<QDBusObjectPath> jobs;
};

JobDbusManager::JobDbusManager(QObject* parent) :
    QObject(parent) {
    d = new JobDbusManagerPrivate();
    new ManagerAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/com/vicr123/libcontemporary/tjob", this);
    QDBusConnection::sessionBus().registerService(QStringLiteral("com.vicr123.libcontemporary.tjob.app%1").arg(qApp->applicationPid()));

    connect(tJobManager::instance(), &tJobManager::jobAdded, this, &JobDbusManager::addJob);
    for (tJob* job : tJobManager::jobs()) {
        addJob(job);
    }
}

JobDbusManager::~JobDbusManager() {
    delete d;
}

QList<QDBusObjectPath> JobDbusManager::Jobs() {
    return d->jobs;
}

QString JobDbusManager::applicationName() {
    return qApp->applicationName();
}

QString JobDbusManager::applicationDesktopEntry() {
    return tApplication::desktopFileName();
}

void JobDbusManager::addJob(tJob* job) {
    QDBusObjectPath path(QStringLiteral("/com/vicr123/libcontemporary/tjob/%1").arg(d->jobs.count() + 1));
    new JobDbus(path.path(), job, this);
    d->jobs.append(path);

    emit JobAdded(path);
}
