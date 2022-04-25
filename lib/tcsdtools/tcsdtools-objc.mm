/*******************************************************************************
 *
 * libcontemporary - Common libraries
 * Copyright (C) 2022 Victor Tran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "../tcsdtools.h"

#include <QWidget>
#import <AppKit/AppKit.h>

void tCsdTools::macHandleDrag(QPoint screenPos, QWidget* dragWindow) {
    CGEventRef cgEvent = CGEventCreateMouseEvent(nullptr, kCGEventLeftMouseDown, screenPos.toCGPoint(), kCGMouseButtonLeft);
    NSEvent* nsEvent = [NSEvent eventWithCGEvent:cgEvent];

    NSView* view = reinterpret_cast<NSView*>(dragWindow->winId());
    [view.window performWindowDragWithEvent:nsEvent];
    CFRelease(cgEvent);
}

void tCsdTools::macInstallResizeAction(QWidget* widget) {
    NSView* view = reinterpret_cast<NSView*>(widget->winId());

    NSWindowStyleMask styleMask = NSWindowStyleMaskFullSizeContentView | NSWindowStyleMaskUnifiedTitleAndToolbar | NSWindowStyleMaskResizable | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
    if ([view.window styleMask] & NSWindowStyleMaskFullScreen) styleMask |= NSWindowStyleMaskFullScreen;
    [view.window setStyleMask:styleMask];
}

void tCsdTools::macRemoveResizeAction(QWidget* widget) {
    NSView* view = reinterpret_cast<NSView*>(widget->winId());

    NSWindowStyleMask styleMask = NSWindowStyleMaskResizable | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
    [view.window setStyleMask:styleMask];
}
