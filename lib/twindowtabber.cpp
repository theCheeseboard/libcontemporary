//
// Created by Victor Tran on 13/4/2022.
//

#include "twindowtabber.h"
#include <QScrollArea>
#include <QBoxLayout>
#include <QTimer>
#include <QScrollBar>
#include <QToolButton>
#include "tlogger.h"
#include "twindowtabberbutton.h"
#include "tapplication.h"

struct tWindowTabberPrivate {
    QList<tWindowTabberButton*> buttons;
    QBoxLayout* tabLayout;
    QScrollArea* scrollArea;

    QToolButton* newTabButton;

    bool scrollLeft = false;
    QTimer* scrollTimer;
};

tWindowTabber::tWindowTabber(QWidget *parent) {
    d = new tWindowTabberPrivate();

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::transparent);

    d->tabLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    d->tabLayout->setSpacing(0);
    d->tabLayout->setContentsMargins(0, 0, 0, 0);

    QBoxLayout* spacerLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    spacerLayout->setSpacing(0);
    spacerLayout->setContentsMargins(0, 0, 0, 0);
    spacerLayout->addLayout(d->tabLayout);
    spacerLayout->addStretch();

    d->scrollArea = new QScrollArea(this);
    d->scrollArea->setPalette(pal);
    d->scrollArea->setFrameShape(QScrollArea::NoFrame);
    d->scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    d->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    d->scrollArea->setWidget(new QWidget());
    d->scrollArea->setWidgetResizable(true);
    d->scrollArea->setMouseTracking(true);
    d->scrollArea->widget()->setLayout(spacerLayout);
    tApplication::instance()->installEventFilter(this);

    d->newTabButton = new QToolButton(this);
    d->newTabButton->setIcon(QIcon::fromTheme("list-add"));
    d->newTabButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    d->newTabButton->setText(tr("New Tab"));
    d->newTabButton->setToolTip(tr("New Tab"));
    d->newTabButton->setVisible(false);
    connect(d->newTabButton, &QToolButton::clicked, this, &tWindowTabber::newTabRequested);

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->scrollArea);
    layout->addWidget(d->newTabButton);
    this->setLayout(layout);

    libContemporaryCommon::fixateHeight(d->scrollArea, [=]{
#ifdef Q_OS_MAC
        return fontMetrics().height() + SC_DPI_W(12, this);
#else
        return fontMetrics().height() + SC_DPI_W(28, this);
#endif
    });

    d->scrollTimer = new QTimer(this);
    connect(d->scrollTimer, &QTimer::timeout, this, [=] {
        d->scrollArea->horizontalScrollBar()->setValue(d->scrollArea->horizontalScrollBar()->value() + (d->scrollLeft ? -1 : 1));
        if (!d->scrollArea->underMouse()) d->scrollTimer->stop();
    });
}

tWindowTabber::~tWindowTabber() {
    delete d;
}

void tWindowTabber::addButton(tWindowTabberButton *button) {
    button->setParent(this);
    d->buttons.append(button);
    d->tabLayout->addWidget(button);
}

void tWindowTabber::setCurrent(tWindowTabberButton *button) {
    for (tWindowTabberButton* tabberButton : d->buttons) {
        tabberButton->setSelected(button == tabberButton);
    }
}

bool tWindowTabber::eventFilter(QObject *watched, QEvent *event) {
    if (QWidget* widget = qobject_cast<QWidget*>(watched)) {
        if ((d->scrollArea->isAncestorOf(widget) || widget == d->scrollArea) && (event->type() == QEvent::MouseMove || event->type() == QEvent::Leave)) {
            if (!d->scrollArea->underMouse()) {
                d->scrollTimer->stop();
                return QObject::eventFilter(watched, event);
            }

            const int scrollDistance = SC_DPI_W(100, d->scrollArea);
            int x = d->scrollArea->mapFromGlobal(QCursor::pos()).x();
            if (x >= scrollDistance && x <= d->scrollArea->width() - scrollDistance) {
                d->scrollTimer->stop();
                return QObject::eventFilter(watched, event);
            }

            //Scroll!
            d->scrollLeft = x < scrollDistance;
            int distance = x;
            if (x > d->scrollArea->width() / 2) {
                distance = d->scrollArea->width() - x;
            }
            d->scrollTimer->setInterval(distance / 4);
            if (!d->scrollTimer->isActive()) d->scrollTimer->start();
        }
    }
    return QObject::eventFilter(watched, event);
}

void tWindowTabber::setShowNewTabButton(bool showNewTabButton) {
    d->newTabButton->setVisible(showNewTabButton);
}

void tWindowTabber::removeButton(tWindowTabberButton *button) {
    d->tabLayout->removeWidget(button);
    d->buttons.removeOne(button);
    button->deleteLater();
}
