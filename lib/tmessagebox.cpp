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

tMessageBox::tMessageBox(QWidget *parent) : QMessageBox(parent)
{

}

tMessageBox::~tMessageBox() {

}

void tMessageBox::setWindowTitle(QString windowTitle) {
#ifdef Q_OS_MAC
    QMessageBox::setText(windowTitle);
#else
    QMessageBox::setWindowTitle(windowTitle);
#endif
}

void tMessageBox::setText(QString text) {
#ifdef Q_OS_MAC
    QMessageBox::setInformativeText(text);
#else
    QMessageBox::setText(text);
#endif
}

int tMessageBox::exec() {
#ifdef Q_OS_MAC
    if (icon() == Warning && !tApplication::applicationIcon().isNull()) {
        //Overlay the application icon over the warning icon
        QIcon warningIcon = tApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
        int iconSizeInt = tApplication::style()->pixelMetric(QStyle::PM_MessageBoxIconSize);

        QSize iconSize(iconSizeInt, iconSizeInt);

        QPixmap newPixmap(iconSize);
        newPixmap.fill(Qt::transparent);

        QPainter painter(&newPixmap);
        painter.drawPixmap(0, 0, warningIcon.pixmap(iconSize));

        QSize appIconSize = iconSize / 2;
        painter.drawPixmap(appIconSize.width(), appIconSize.height(), tApplication::applicationIcon().pixmap(appIconSize));

        painter.end();
        setIconPixmap(newPixmap);
    }
#endif

    return QMessageBox::exec();
}
