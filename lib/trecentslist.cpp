#include "trecentslist.h"

#include <QDir>
#include <QFileSystemWatcher>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

struct tRecentsListPrivate {
        QString name;
        QString path;

        QList<QUrl> paths;
        QFileSystemWatcher* watcher;
};

tRecentsList::tRecentsList(QString name, QObject* parent) :
    QObject{parent} {
    d = new tRecentsListPrivate();
    d->name = name;

    QDir appData(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QDir recents(appData.absoluteFilePath("recents"));
    QDir::root().mkpath(recents.absolutePath());
    d->path = recents.absoluteFilePath(name);

    if (!QFile::exists(d->path)) {
        this->save();
    }

    d->watcher = new QFileSystemWatcher({d->path}, this);
    connect(d->watcher, &QFileSystemWatcher::fileChanged, this, [this] {
        this->load();
    });

    this->load();
}

tRecentsList::~tRecentsList() {
    delete d;
}

void tRecentsList::push(QUrl url) {
    // Push the path to the top
    d->paths.removeAll(url);
    d->paths.prepend(url);

    // Ensure we only keep 10 entries
    if (d->paths.length() > 10) {
        d->paths = d->paths.mid(0, 10);
    }

    this->save();
}

QUrl tRecentsList::url(int index) {
    return d->paths.at(index);
}

int tRecentsList::count() {
    return d->paths.count();
}

void tRecentsList::clear() {
    d->paths.clear();
    this->save();
}

void tRecentsList::load() {
    emit beforeUpdated();

    QFile file(d->path);
    file.open(QFile::ReadOnly);
    auto paths = QJsonDocument::fromJson(file.readAll()).array();
    file.close();

    d->paths.clear();
    for (auto path : paths) {
        if (path.toString().isEmpty()) continue;
        d->paths.append(QUrl(path.toString()));
    }

    emit updated();
}

void tRecentsList::save() {
    QJsonArray paths;
    for (auto path : d->paths) {
        paths.append(path.toString());
    }

    QFile file(d->path);
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(paths).toJson());
    file.close();
}
