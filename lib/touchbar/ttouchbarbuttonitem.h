#ifndef TTOUCHBARBUTTONITEM_H
#define TTOUCHBARBUTTONITEM_H

#include "ttouchbarcustomtouchbaritem.h"

struct tTouchBarButtonItemPrivate;
class tTouchBarButtonItem : public tTouchBarCustomTouchBarItem {
        Q_OBJECT
    public:
        explicit tTouchBarButtonItem(QString identifier, QString customizationLabel, QObject* parent = nullptr);
        ~tTouchBarButtonItem();

        void setText(QString text);
        QString text();

        void setIcon(QIcon icon);
        QIcon icon();

        void setEnabled(bool enabled);
        bool enabled();

        void setVisible(bool visible);
        bool visible();

    signals:
        void clicked();

    private:
        tTouchBarButtonItemPrivate* d;

        void init();
        void updateTouchBarItem();

        // tTouchBarCustomTouchBarItem interface
    public:
        NSView* view();
};

#endif // TTOUCHBARBUTTONITEM_H
