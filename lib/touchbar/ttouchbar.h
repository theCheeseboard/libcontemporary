#ifndef TTOUCHBAR_H
#define TTOUCHBAR_H

#include <QObject>

class tTouchBarAbstractItem;
struct tTouchBarPrivate;
class tTouchBar : public QObject {
        Q_OBJECT
    public:
        explicit tTouchBar(QObject* parent = nullptr);
        ~tTouchBar();

        void attach(QWidget* widget);

        void addItem(tTouchBarAbstractItem* item);
        QList<tTouchBarAbstractItem*> touchBarItems();

        void addDefaultItem(tTouchBarAbstractItem* item);
        QList<tTouchBarAbstractItem*> defaultItems();

    signals:

    private:
        tTouchBarPrivate* d;

        void init();
        void invalidateTouchBar();
};

#endif // TTOUCHBAR_H
