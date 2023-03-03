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
#ifndef JSONFILE_H
#define JSONFILE_H

#include <QString>

struct JsonFilePrivate;

class JsonFile {
    public:
        JsonFile();
        JsonFile(QString file);
        JsonFile(const JsonFile& other);
        ~JsonFile();

        void insertString(QString key, QString message, QString description = "");
        QString message(QString key) const;

        QByteArray output();

        JsonFile& operator=(const JsonFile& other);

    private:
        JsonFilePrivate* d;
};

#endif // JSONFILE_H
