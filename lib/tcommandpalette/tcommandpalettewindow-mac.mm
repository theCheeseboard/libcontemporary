#include "tcommandpalettewindow.h"
#import <AppKit/AppKit.h>

#include "tcommandpalettecontroller.h"
#include "tcommandpalettescope.h"

@interface CommandPaletteWindowTouchBarProvider: NSResponder <NSTouchBarDelegate, NSApplicationDelegate, NSWindowDelegate>

@property (strong) NSCustomTouchBarItem *firstFrameItem;
@property (strong) NSCustomTouchBarItem *lastFrameItem;
@property (strong) NSButton *firstFrameButton;
@property (strong) NSButton *lastFrameButton;

@property (strong) NSObject *qtDelegate;
@property tCommandPaletteController *commandPaletteController;
@property tCommandPaletteWindow *commandPaletteWindow;
@property NSSegmentedControl *segmentedControl;
@property NSCustomTouchBarItem *segmentedControlItem;

@end

static NSTouchBarItemIdentifier scopeIdentifier = @"com.vicr123.libcontemporary.scope";

@implementation CommandPaletteWindowTouchBarProvider

- (id)init: (tCommandPaletteWindow*)commandPaletteWin withController:(tCommandPaletteController*)controller {
    if (self = [super init]) {
        //Set main window UI to call touch bar handlers
        self.commandPaletteWindow = commandPaletteWin;
        self.commandPaletteController = controller;

        self.segmentedControl = [[NSSegmentedControl alloc] init];
        [self.segmentedControl setTarget:self];
        [self.segmentedControl setAction:@selector(touchBarSegmentedControlChanged:)];

        self.segmentedControlItem = [[NSCustomTouchBarItem alloc] initWithIdentifier:scopeIdentifier];
        [self.segmentedControlItem setView:self.segmentedControl];
    }

    return self;
}

- (NSTouchBar *)makeTouchBar {
    // Create the touch bar with this instance as its delegate
    NSTouchBar *bar = [[NSTouchBar alloc] init];
    bar.delegate = self;

    bar.defaultItemIdentifiers = @[scopeIdentifier];
    bar.customizationRequiredItemIdentifiers = @[scopeIdentifier];
    [bar setCustomizationIdentifier:@"com.vicr123.libcontemporary.commandpalette.touchbar"];

    return bar;
}

- (NSTouchBarItem *)touchBar:(NSTouchBar *)touchBar makeItemForIdentifier:(NSTouchBarItemIdentifier)identifier {
    Q_UNUSED(touchBar);

    if (identifier != scopeIdentifier) {
        return nil;
    }

    return self.segmentedControlItem;
}

- (void)touchBarSegmentedControlChanged: (NSSegmentedControl*) segmentedControl {
    self.commandPaletteWindow->changeScope(self.commandPaletteController->scopes().at(segmentedControl.selectedSegment));
}

- (void)installAsDelegateForWindow: (NSWindow *) window {
    _qtDelegate = window.delegate; // Save current delegate for forwarding
    window.delegate = self;
}

- (BOOL)respondsToSelector: (SEL) aSelector {
    // We want to forward to the qt delegate. Respond to selectors it
    // responds to in addition to selectors this instance resonds to.
    return [_qtDelegate respondsToSelector:aSelector] || [super respondsToSelector:aSelector];
}

- (void)forwardInvocation: (NSInvocation *) anInvocation {
    // Forward to the existing delegate. This function is only called for selectors
    // this instance does not responds to, which means that the Qt delegate
    // must respond to it (due to the respondsToSelector implementation above).
    [anInvocation invokeWithTarget:_qtDelegate];
}

- (void)setScopes:(QList<tCommandPaletteScope*>)scopes withDefaultScope:(tCommandPaletteScope*)defaultScope {
    [self.segmentedControl setSegmentCount:scopes.length()];
    for (auto i = 0; i < scopes.length(); i++) {
        auto scope = scopes.at(i);
        [self.segmentedControl setLabel:scope->displayName().toNSString() forSegment:i];
        if (defaultScope == scope) {
            [self.segmentedControl setSelectedSegment:i];
        }
    }
}

@end

void tCommandPaletteWindow::setupTouchBar() {
    //Install TouchBarProvider as window delegate
    auto view = reinterpret_cast<NSView *>(this->winId());
    CommandPaletteWindowTouchBarProvider *touchBarProvider = [[CommandPaletteWindowTouchBarProvider alloc] init:this withController:controller()];
    [touchBarProvider installAsDelegateForWindow:view.window];

    connect(controller(), &tCommandPaletteController::scopesChanged, this, [this, touchBarProvider] {
        [touchBarProvider setScopes:controller()->scopes() withDefaultScope:this->currentScope()];
    });
    connect(this, &tCommandPaletteWindow::scopeChanged, this, [this, touchBarProvider] {
        [touchBarProvider setScopes:controller()->scopes() withDefaultScope:this->currentScope()];
    });
    [touchBarProvider setScopes:controller()->scopes() withDefaultScope:this->currentScope()];
}
