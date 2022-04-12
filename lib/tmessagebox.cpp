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

#include "tmessagebox.h"

#include "tapplication.h"
#include <QPainter>

#ifdef Q_OS_MAC
#include "private/nsalertmac.h"
#endif

struct tMessageBoxPrivate {
#ifdef Q_OS_MAC
    NsAlertMac* nsAlert;
#endif
};

tMessageBox::tMessageBox(QWidget *parent) : QMessageBox(parent)
{
    d = new tMessageBoxPrivate;
#ifdef Q_OS_MAC
    d->nsAlert = new NsAlertMac(this);
    d->nsAlert->setParentWindow(parent);
    connect(d->nsAlert, &NsAlertMac::finished, this, &tMessageBox::finished);
    connect(d->nsAlert, &NsAlertMac::accepted, this, &tMessageBox::accepted);
#endif
}

tMessageBox::~tMessageBox() {
    delete d;
}

void tMessageBox::setWindowTitle(QString windowTitle) {
    QMessageBox::setWindowTitle(windowTitle);
#ifdef Q_OS_MAC
    d->nsAlert->setTitle(windowTitle);
#endif
}

void tMessageBox::setText(QString text) {
    QMessageBox::setText(text);
#ifdef Q_OS_MAC
    d->nsAlert->setText(text);
#endif
}

int tMessageBox::exec() {
#ifdef Q_OS_MAC
//    if (icon() == Warning && !tApplication::applicationIcon().isNull()) {
//        //Overlay the application icon over the warning icon
//        QIcon warningIcon = tApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
//        int iconSizeInt = tApplication::style()->pixelMetric(QStyle::PM_MessageBoxIconSize);
//
//        QSize iconSize(iconSizeInt, iconSizeInt);
//
//        QPixmap newPixmap(iconSize);
//        newPixmap.fill(Qt::transparent);
//
//        QPainter painter(&newPixmap);
//        painter.drawPixmap(0, 0, warningIcon.pixmap(iconSize));
//
//        QSize appIconSize = iconSize / 2;
//        painter.drawPixmap(appIconSize.width(), appIconSize.height(), tApplication::applicationIcon().pixmap(appIconSize));
//
//        painter.end();
//        setIconPixmap(newPixmap);
//    }

    d->nsAlert->setIcon(this->icon());
    for (int button = QMessageBox::FirstButton; button != QMessageBox::LastButton; button <<= 1) {
        if (this->standardButtons() & button) d->nsAlert->addStandardButton(static_cast<QMessageBox::StandardButton>(button));
    }

    for (QAbstractButton* button : this->buttons()) {
        QPushButton* newButton = d->nsAlert->addButton(button->text());
        connect(newButton, &QPushButton::clicked, button, &QPushButton::click);
    }

    return d->nsAlert->exec();
#endif

    return QMessageBox::exec();
}

void tMessageBox::open() {
#ifdef Q_OS_MAC
    d->nsAlert->setIcon(this->icon());
    for (int button = QMessageBox::FirstButton; button != QMessageBox::LastButton; button <<= 1) {
        if (this->standardButtons() & button) d->nsAlert->addStandardButton(static_cast<QMessageBox::StandardButton>(button));
    }

    for (QAbstractButton* button : this->buttons()) {
        QPushButton* newButton = d->nsAlert->addButton(button->text());
        connect(newButton, &QPushButton::clicked, button, &QPushButton::click);
    }

    return d->nsAlert->open();
#endif
    QMessageBox::open();
}

void tMessageBox::setParent(QWidget *widget) {
#ifdef Q_OS_MAC
    d->nsAlert->setParentWindow(widget);
#endif
    QMessageBox::setParent(widget);
}
