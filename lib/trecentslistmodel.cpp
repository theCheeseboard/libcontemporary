#include "trecentslistmodel.h"

#include "trecentslist.h"
#include <QFileIconProvider>
#include <QFileInfo>
#include <QIcon>
#include <QUrl>

struct tRecentsListModelPrivate {
        tRecentsList* list;

        QFileIconProvider iconProvider;
};

tRecentsListModel::tRecentsListModel(QString name, QObject* parent) :
    QAbstractListModel(parent) {
    d = new tRecentsListModelPrivate();
    d->list = new tRecentsList(name, this);
    connect(d->list, &tRecentsList::beforeUpdated, this, &tRecentsListModel::beginResetModel);
    connect(d->list, &tRecentsList::updated, this, &tRecentsListModel::endResetModel);
}

tRecentsListModel::~tRecentsListModel() {
    delete d;
}

int tRecentsListModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return d->list->count();
}

QVariant tRecentsListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    auto url = d->list->url(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return url.fileName();
        case Qt::DecorationRole:
            return d->iconProvider.icon(QFileInfo(url.toLocalFile()));
        case PathRole:
            return url;
    }

    return QVariant();
}
