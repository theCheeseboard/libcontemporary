#include "ttouchbarflexiblespaceitem.h"

tTouchBarFlexibleSpaceItem::tTouchBarFlexibleSpaceItem(QObject* parent) :
    tTouchBarAbstractItem{QString::fromNSString(NSTouchBarItemIdentifierFlexibleSpace), parent} {
}
