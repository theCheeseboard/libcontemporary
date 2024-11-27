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
#ifndef TJOB_H
#define TJOB_H

#include "libcontemporary_global.h"
#include <QObject>

class LIBCONTEMPORARY_EXPORT tJob : public QObject {
        Q_OBJECT
        Q_PROPERTY(quint64 progress READ progress NOTIFY progressChanged FINAL)
        Q_PROPERTY(quint64 totalProgress READ totalProgress NOTIFY totalProgressChanged FINAL)
        Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
        Q_PROPERTY(QString title READ titleString NOTIFY titleStringChanged FINAL)
        Q_PROPERTY(QString status READ statusString NOTIFY statusStringChanged FINAL)
        Q_PROPERTY(bool isTransient READ isTransient FINAL)
        Q_PROPERTY(QUrl qmlFile READ qmlFile FINAL)
    public:
        explicit tJob(QObject* parent = nullptr);

        enum State {
            Processing,
            Finished,
            Failed,
            RequiresAttention
        };

        virtual quint64 progress() = 0;
        virtual quint64 totalProgress() = 0;
        virtual State state() = 0;
        virtual QWidget* makeProgressWidget();
        virtual QString titleString();
        virtual QString statusString();
        virtual bool isTransient();
        virtual QUrl qmlFile();

    signals:
        void progressChanged(quint64 progress);
        void totalProgressChanged(quint64 totalProgress);
        void stateChanged(tJob::State state);
        void titleStringChanged(QString titleString);
        void statusStringChanged(QString statusString);
};

#endif // TJOB_H
