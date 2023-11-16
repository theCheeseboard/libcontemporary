#include "trecentslist.h"

#include <QDir>
#include <QFileIconProvider>
#include <QFileSystemWatcher>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenu>
#include <QStandardPaths>

struct tRecentsListPrivate {
        QString name;
        QString path;

        QList<QUrl> paths;
        QFileSystemWatcher* watcher;

        QMenu* recentsMenu;
        QFileIconProvider iconProvider;
};

tRecentsList::tRecentsList(QString name, QObject* parent) :
    QObject{parent} {
    d = new tRecentsListPrivate();
    d->name = name;

    QDir appData(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QDir recents(appData.absoluteFilePath("recents"));
    QDir::root().mkpath(recents.absolutePath());
    d->path = recents.absoluteFilePath(name);

    d->recentsMenu = new QMenu();
    d->recentsMenu->setTitle(tr("Open Recent"));

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
    d->recentsMenu->deleteLater();
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

QMenu* tRecentsList::recentsMenu() {
    return d->recentsMenu;
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

    d->recentsMenu->clear();
    if (d->paths.isEmpty()) {
        auto emptyAction = d->recentsMenu->addAction(tr("No Recent Items"));
        emptyAction->setEnabled(false);
    } else {
        for (auto path : d->paths) {
            auto recentAction = d->recentsMenu->addAction(d->iconProvider.icon(QFileInfo(path.toLocalFile())), path.fileName(), [path, this] {
                emit openRecent(path);
            });

            // Force the icon to be visible in macOS
            recentAction->setIconVisibleInMenu(true);
        }
    }
    d->recentsMenu->addSeparator();
    auto clearAction = d->recentsMenu->addAction(tr("Clear Menu"), [this] {
        this->clear();
    });
    clearAction->setEnabled(!d->paths.isEmpty());

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
