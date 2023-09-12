#include "ttouchbarcustomtouchbaritem.h"
#include "private/ttouchbarcustomtouchbaritem_p.h"

NSTouchBarItem *tTouchBarCustomTouchBarItem::makeItem()
{
    auto item = [[NSCustomTouchBarItem alloc] initWithIdentifier:this->identifier().toNSString()];
    [item setCustomizationLabel:d->customizationLabel.toNSString()];
    [item setView:this->view()];
    return item;
}
