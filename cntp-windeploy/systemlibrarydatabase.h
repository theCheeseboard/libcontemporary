#ifndef SYSTEMLIBRARYDATABASE_H
#define SYSTEMLIBRARYDATABASE_H

#include <QObject>
#include <QStringList>

struct SystemLibraryDatabasePrivate;
class SystemLibraryDatabase : public QObject {
	Q_OBJECT

	public:
        explicit SystemLibraryDatabase(QStringList extraSearchPaths, QObject *parent = nullptr);
        ~SystemLibraryDatabase();

        QString library(QString libraryName);
        bool ignore(QString libraryName);

    private:
        SystemLibraryDatabasePrivate *d;

        bool isInDisallowedPath(QString path);
};

#endif