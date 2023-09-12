#include <AppKit/AppKit.h>

#include "ttouchbar.h"
#include "ttouchbarabstractitem.h"
#include "private/ttouchbar_p.h"
#include "ranges/trange.h"
#include <QWidget>

@interface TTouchBarMainWindowDelegate: NSResponder<NSTouchBarDelegate, NSApplicationDelegate, NSWindowDelegate>
@property QWidget* parent;
@property tTouchBar* parentTouchBar;
@property (strong) NSObject *existingDelegate;
@end

@implementation TTouchBarMainWindowDelegate

- (id)init: (tTouchBar*)parentTouchBar {
    if (self = [super init]) {
        self.parent = nullptr;
        self.parentTouchBar = parentTouchBar;
    }
    return self;
}

- (void)attachToWidget: (QWidget*)parentWidget {
    self.parent = parentWidget;

    auto view = reinterpret_cast<NSView*>(parentWidget->winId());
    auto window = [view window];
    self.existingDelegate = window.delegate; // Save current delegate for forwarding
    window.delegate = self;
}

- (void)invalidateTouchBar {
    if (!self.parent) return;

    auto view = reinterpret_cast<NSView*>(self.parent->winId());
    [[view window] setTouchBar:nil];
    [view setTouchBar:nil];
}

- (NSTouchBar*)makeTouchBar {
    NSTouchBar* bar = [[NSTouchBar alloc] init];
    [bar setDelegate:self];

    auto defaultItemIdentifiers = tRange(self.parentTouchBar->defaultItems()).reduce<NSMutableArray*>([](tTouchBarAbstractItem* item, NSMutableArray* array) {
        [array addObject:item->identifier().toNSString()];
        return array;
    }, [[NSMutableArray alloc] init]);
    [bar setDefaultItemIdentifiers:defaultItemIdentifiers];

    auto customizationAllowedItemIdentifiers = tRange(self.parentTouchBar->touchBarItems()).reduce<NSMutableArray*>([](tTouchBarAbstractItem* item, NSMutableArray* array) {
        [array addObject:item->identifier().toNSString()];
        return array;
    }, [[NSMutableArray alloc] init]);
    [bar setCustomizationAllowedItemIdentifiers:customizationAllowedItemIdentifiers];

    return bar;
}

- (NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier {
    try {
        auto item = tRange(self.parentTouchBar->touchBarItems()).first([identifier](tTouchBarAbstractItem* item) {
            return item->identifier() == QString::fromNSString(identifier);
        });
        return item->makeItem();
    } catch (tRangeException& ex) {
        return nil;
    }
}

- (BOOL)respondsToSelector: (SEL)selector {
    return [self.existingDelegate respondsToSelector:selector] || [super respondsToSelector:selector];
}

- (void)forwardInvocation: (NSInvocation *)invocation {
    [invocation invokeWithTarget:self.existingDelegate];
}

@end

void tTouchBar::init() {
    d->delegate = [[TTouchBarMainWindowDelegate alloc] init:this];
}

void tTouchBar::invalidateTouchBar()
{
    [d->delegate invalidateTouchBar];
}

void tTouchBar::attach(QWidget* widget) {
    [d->delegate attachToWidget:widget->window()];
}
