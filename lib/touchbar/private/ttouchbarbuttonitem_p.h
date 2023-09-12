#ifndef TTOUCHBARBUTTONITEM_P_H
#define TTOUCHBARBUTTONITEM_P_H

#include "../ttouchbar_forwarddeclares.h"
#include <QIcon>
#include <QString>

T_FORWARD_DECLARE_OBJC_INTERFACE(TTouchBarButtonItemInternalHandler);
T_FORWARD_DECLARE_OBJC_INTERFACE(NSButton);

struct tTouchBarButtonItemPrivate {
        QString text;
        QIcon icon;
        bool enabled = true;
        bool visible = true;

        TTouchBarButtonItemInternalHandler* internalHandler;
        NSButton* touchBarButton;
};

#endif // TTOUCHBARBUTTONITEM_P_H
