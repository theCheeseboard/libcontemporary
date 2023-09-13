#include "ttouchbarflexiblespaceitem.h"

#ifndef HAVE_TOUCHBAR_IMPL
tTouchBarFlexibleSpaceItem::tTouchBarFlexibleSpaceItem(QObject* parent) :
    tTouchBarAbstractItem{QStringLiteral(""), parent} {
}
#endif

NSTouchBarItem* tTouchBarFlexibleSpaceItem::makeItem() {
    return nullptr;
}
