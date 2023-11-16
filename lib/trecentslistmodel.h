#ifndef TRECENTSLISTMODEL_H
#define TRECENTSLISTMODEL_H

#include <QAbstractListModel>

struct tRecentsListModelPrivate;
class tRecentsListModel : public QAbstractListModel {
        Q_OBJECT

    public:
        explicit tRecentsListModel(QString name = "default", QObject* parent = nullptr);
        ~tRecentsListModel();

        enum Role {
            PathRole = Qt::UserRole
        };

        // Basic functionality:
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    private:
        tRecentsListModelPrivate* d;
};

#endif // TRECENTSLISTMODEL_H
