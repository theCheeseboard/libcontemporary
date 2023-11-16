#ifndef TRECENTSLIST_H
#define TRECENTSLIST_H

#include <QObject>

class QMenu;
struct tRecentsListPrivate;
class tRecentsList : public QObject {
        Q_OBJECT
    public:
        explicit tRecentsList(QString name = "default", QObject* parent = nullptr);
        ~tRecentsList();

        void push(QUrl url);
        QUrl url(int index);
        int count();
        void clear();

        QMenu* recentsMenu();

    signals:
        void beforeUpdated();
        void updated();
        void openRecent(QUrl url);

    private:
        tRecentsListPrivate* d;

        void load();
        void save();
};

#endif // TRECENTSLIST_H
