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

#include "taboutdialog.h"
#include "ui_taboutdialog.h"

#include "tapplication.h"
#include "ticon.h"
#include <QScroller>
#include <QToolButton>

tAboutDialog::tAboutDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::tAboutDialog) {
    ui->setupUi(this);
    tIcon::processWidgetLater(this);

    this->setWindowTitle((tr("About %1").arg(tApplication::applicationName())));
    ui->titleLabel->setText(tr("About %1").arg(tApplication::applicationName()));
    ui->applicationTitle->setText(tApplication::applicationName() + " " + tApplication::applicationVersion());
    ui->genericName->setText(tApplication::genericName());
    ui->verticalSpacer->changeSize(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding);

    QPixmap splashGraphic = tApplication::aboutDialogSplashGraphic();
    bool graphicAvailable;
    if (splashGraphic.isNull()) {
        ui->splashGraphicLabel->setVisible(false);
        this->setFixedSize(QSize(430, 340));
        graphicAvailable = false;
    } else {
        ui->splashGraphicLabel->setPixmap(splashGraphic);
        this->setFixedSize(QSize(530, 340));
        graphicAvailable = true;
    }

    QList<QPair<QString, QString>> softwareVersions = tApplication::versions();
    for (int i = 0; i < softwareVersions.count(); i++) {
        QLabel* label1 = new QLabel(this);
        label1->setText(softwareVersions.at(i).first);
        ui->versionsLayout->addWidget(label1, i + 1, 0);
        ui->versionsLayout->setAlignment(label1, Qt::AlignLeft);

        QLabel* label2 = new QLabel(this);
        label2->setText(softwareVersions.at(i).second);
        label2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->versionsLayout->addWidget(label2, i + 1, 1);
        ui->versionsLayout->setAlignment(label2, Qt::AlignLeft);
    }

    QFont copyrightFont = this->font();
    if (copyrightFont.pixelSize() == -1) {
        copyrightFont.setPointSizeF(copyrightFont.pointSizeF() * 0.75);
    } else {
        copyrightFont.setPixelSize(copyrightFont.pixelSize() * 0.75);
    }
    QStringList copyrightLines = tApplication::copyrightLines();
    for (int i = 0; i < copyrightLines.count(); i++) {
        QLabel* label = new QLabel(this);
        label->setText(copyrightLines.at(i));
        label->setWordWrap(true);
        label->setFont(copyrightFont);
        ui->copyrightLayout->addWidget(label);
    }

#ifndef Q_OS_MAC
    QToolButton* backButton = new QToolButton(this);
    backButton->setAutoRaise(true);
    backButton->setIcon(QIcon::fromTheme("go-previous", QIcon(":/the-libs/icons/go-previous.svg")));
    if (graphicAvailable) {
        backButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        backButton->setParent(ui->splashGraphicLabel);
        backButton->setFixedHeight(ui->titleLabel->sizeHint().height());

        if (this->layoutDirection() == Qt::LeftToRight) {
            backButton->move(0, 0);
        } else {
            backButton->move(100 - backButton->width(), 0);
        }
    } else {
        backButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        ui->titleLayout->insertWidget(0, backButton);
    }
    connect(backButton, &QToolButton::clicked, this, &tAboutDialog::on_okButton_clicked);

    ui->macOkPanel->setVisible(false);
#endif

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
}

tAboutDialog::~tAboutDialog() {
    delete ui;
}

void tAboutDialog::on_okButton_clicked() {
    this->close();
}
