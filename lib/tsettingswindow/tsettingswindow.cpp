/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2022 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "tsettingswindow.h"
#include "ui_tsettingswindow.h"

#include "tsettingspane.h"
#include "tsettingswindowdelegate.h"
#include "tsubtitlelabel.h"
#include <QScroller>
#include <QStandardItemModel>

struct tSettingsWindowPrivate {
        QStandardItemModel* leftModel;

        QList<QWidget*> rightSide;
        bool addedFirst = false;
};

tSettingsWindow::tSettingsWindow(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::tSettingsWindow) {
    ui->setupUi(this);

    ui->titleLabel->setBackButtonShown(true);
    this->setWindowTitle(tr("Settings"));

    d = new tSettingsWindowPrivate();

    d->leftModel = new QStandardItemModel();

    ui->contentsView->setModel(d->leftModel);
    ui->contentsView->setItemDelegate(new tSettingsWindowDelegate(this));
    ui->contentsView->setFixedWidth(SC_DPI_W(300, this));
}

tSettingsWindow::~tSettingsWindow() {
    delete d;
    delete ui;
}

void tSettingsWindow::appendSection(QString text) {
    auto item = new QStandardItem(text);
    d->leftModel->appendRow(item);
}

void tSettingsWindow::appendPane(tSettingsPane* pane) {
    if (d->addedFirst) {
        auto line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFixedHeight(1);
        ui->paneLayout->addWidget(line);
    }

    auto item = new QStandardItem(pane->paneName());
    d->leftModel->appendRow(item);

    auto titleContainer = new QWidget(this);
    titleContainer->setFixedWidth(SC_DPI_W(600, this));

    auto titleLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    titleLayout->setContentsMargins(9, 9, 9, 0);
    titleLayout->setAlignment(Qt::AlignLeft);

    auto* title = new tSubtitleLabel(this);
    title->setText(pane->paneName());
    titleLayout->addWidget(title);
    titleContainer->setLayout(titleLayout);
    ui->paneLayout->addWidget(titleContainer);

    pane->setFixedWidth(SC_DPI_W(600, this));
    ui->paneLayout->addWidget(pane);

    item->setData(QVariant::fromValue(titleContainer));

    d->rightSide.append(titleContainer);
    d->rightSide.append(pane);
    d->addedFirst = true;
}

void tSettingsWindow::appendSeparator() {
    this->appendSection("");
}

void tSettingsWindow::on_titleLabel_backButtonClicked() {
    this->close();
}

void tSettingsWindow::resizeEvent(QResizeEvent* event) {
    if (this->width() > SC_DPI_W(900, this)) {
        ui->leftArea->setVisible(true);
        ui->line->setVisible(true);
        auto remainingWidth = this->width() - SC_DPI_W(900, this) - 1;
        ui->leftArea->setFixedWidth(remainingWidth / 2 + SC_DPI_W(300, this));
        ui->rightArea->setFixedWidth(remainingWidth / 2 + SC_DPI_W(600, this));

        for (auto widget : d->rightSide) widget->setFixedWidth(SC_DPI_W(600, this));
    } else if (this->width() > SC_DPI_W(600, this)) {
        ui->leftArea->setVisible(false);
        ui->line->setVisible(false);
        ui->rightArea->setFixedWidth(SC_DPI_W(600, this));

        for (auto widget : d->rightSide) widget->setFixedWidth(SC_DPI_W(600, this));
    } else {
        ui->rightArea->setFixedWidth(this->width());

        for (auto widget : d->rightSide) widget->setFixedWidth(this->width());
    }
}

void tSettingsWindow::on_contentsView_clicked(const QModelIndex& index) {
    if (auto* widget = index.data(Qt::UserRole + 1).value<QWidget*>()) {
        auto origin = widget->mapTo(ui->rightAreaContents, QPoint(0, 0));
        //        ui->rightArea->ensureWidgetVisible(widget);
        QScroller::scroller(ui->rightArea->viewport())->scrollTo(origin);
    }
}
