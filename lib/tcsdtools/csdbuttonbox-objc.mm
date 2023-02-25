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

#include "csdbuttonbox.h"
#include "csdbuttonbox_p.h"
#include "ui_csdbuttonbox.h"

#include <QWindow>
#import <AppKit/AppKit.h>

void CsdButtonBoxPrivate::windowHidden() {
    if (closeWidget) {
        box->ui->macWindowControlsLayout->removeWidget(closeWidget);
        box->ui->macWindowControlsLayout->removeWidget(minWidget);
        box->ui->macWindowControlsLayout->removeWidget(fsWidget);

        closeWidget->deleteLater();
        minWidget->deleteLater();
        fsWidget->deleteLater();

        closeWidget = nullptr;
        minWidget = nullptr;
        fsWidget = nullptr;
    }
}

void CsdButtonBoxPrivate::windowShown() {
    if (!closeWidget) {
        NSWindowStyleMask styleMask = NSWindowStyleMaskFullSizeContentView|NSWindowStyleMaskResizable|NSWindowStyleMaskClosable|NSWindowStyleMaskMiniaturizable;

        NSButton* closeButton = [NSWindow standardWindowButton:NSWindowCloseButton forStyleMask:styleMask];
        closeWidget = QWidget::createWindowContainer(QWindow::fromWinId(reinterpret_cast<WId>(closeButton)), box);
        closeWidget->setFixedSize(14, 16);
        box->ui->macWindowControlsLayout->addWidget(closeWidget);

        NSButton* minButton = [NSWindow standardWindowButton:NSWindowMiniaturizeButton forStyleMask:styleMask];
        minWidget = QWidget::createWindowContainer(QWindow::fromWinId(reinterpret_cast<WId>(minButton)), box);
        minWidget->setFixedSize(14, 16);
        box->ui->macWindowControlsLayout->addWidget(minWidget);

        NSButton* fsButton = [NSWindow standardWindowButton:NSWindowZoomButton forStyleMask:styleMask];
        fsWidget = QWidget::createWindowContainer(QWindow::fromWinId(reinterpret_cast<WId>(fsButton)), box);
        fsWidget->setFixedSize(14, 16);
        box->ui->macWindowControlsLayout->addWidget(fsWidget);
    }
}

void CsdButtonBox::setupMacOs() {
    ui->closeButton->setVisible(false);
    ui->maxButton->setVisible(false);
    ui->minButton->setVisible(false);

    ui->macWindowControlsLayout->setContentsMargins(6, 0, 6, 0);
    ui->macWindowControlsLayout->setSpacing(6);

    //BUG: The following line causes the bug
    //windowShown();
}
