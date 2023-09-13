#ifndef TTOUCHBAR_H
#define TTOUCHBAR_H

#include "ttouchbar_forwarddeclares.h"
#include <QObject>

T_FORWARD_DECLARE_OBJC_INTERFACE(NSTouchBar);

class tTouchBarAbstractItem;
struct tTouchBarPrivate;
class tTouchBarPopoverItem;
class QAction;
class tTouchBar : public QObject {
        Q_OBJECT
    public:
        explicit tTouchBar(QObject* parent = nullptr);
        ~tTouchBar();

        void attach(QWidget* widget);
        void detach();

        void setCustomizationIdentifier(QString customizationIdentifier);
        QString customizationIdentifier();

        void addItem(tTouchBarAbstractItem* item);
        QList<tTouchBarAbstractItem*> touchBarItems();

        void addDefaultItem(tTouchBarAbstractItem* item);
        QList<tTouchBarAbstractItem*> defaultItems();

    signals:

    protected:
        friend tTouchBarPopoverItem;
        NSTouchBar* makeTouchBar();

    private:
        tTouchBarPrivate* d;

        void init();
        void invalidateTouchBar();
};

#endif // TTOUCHBAR_H
