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
#include "tscrim.h"

#include "tvariantanimation.h"
#include <QGraphicsEffect>
#include <QMap>
#include <QPainter>
#include <QSet>

struct tScrimPrivate {
        static QMap<QWidget*, tScrim*> scrims;
        static QMap<QWidget*, QWidget*> scrimProxies;
        QGraphicsBlurEffect* blurEffect;
        QSet<QWidget*> ignoreWidgets;
        QWidget* parentWidget;
        double opacity = 1;

        bool showing = false;

        tVariantAnimation* anim;

        const int scrimOverscan = 50;
};

QMap<QWidget*, tScrim*> tScrimPrivate::scrims = QMap<QWidget*, tScrim*>();
QMap<QWidget*, QWidget*> tScrimPrivate::scrimProxies = QMap<QWidget*, QWidget*>();

tScrim::tScrim(QWidget* parent) :
    QWidget(parent) {
    d = new tScrimPrivate();
    d->parentWidget = parent;
    d->scrims.insert(parent, this);
    parent->installEventFilter(this);

    this->setAutoFillBackground(true);

    d->blurEffect = new QGraphicsBlurEffect(this);
    this->setGraphicsEffect(d->blurEffect);

    d->anim = new tVariantAnimation(this);
    d->anim->setStartValue(0.0);
    d->anim->setEndValue(1.0);
    d->anim->setDuration(250);
    d->anim->setEasingCurve(QEasingCurve::OutCubic);
    connect(d->anim, &tVariantAnimation::valueChanged, this, [this, parent](QVariant value) {
        d->blurEffect->setBlurRadius(value.toReal() * 10);
        d->opacity = 1 - value.toReal() * 0.75;
        this->update();

        // Constantly change the size of the blanker because Qt doesn't seem to blur correctly otherwise
        if (this->geometry().size().height() == parent->height() + d->scrimOverscan * 2) {
            this->resize(this->width(), parent->height() + d->scrimOverscan * 2 + 1);
        } else {
            this->resize(this->width(), parent->height() + d->scrimOverscan * 2);
        }
    });
    connect(d->anim, &tVariantAnimation::finished, this, [this] {
        if (d->anim->direction() == tVariantAnimation::Backward) {
            QWidget::hide();
        }
    });

    connect(parent, &QWidget::destroyed, this, [this, parent] {
        d->scrims.remove(parent);
    });

    QWidget::hide();

    updateGeometry();
}

tScrim::~tScrim() {
    delete d;
}

tScrim* tScrim::scrimForWidget(QWidget* widget) {
    auto scrimWidget = tScrim::scrimWidget(widget);
    if (tScrimPrivate::scrims.contains(scrimWidget)) {
        return tScrimPrivate::scrims.value(scrimWidget);
    } else {
        tScrim* scrim = new tScrim(scrimWidget);
        return scrim;
    }
}

void tScrim::setScrimProxy(QWidget* forWidget, QWidget* toWidget) {
    tScrimPrivate::scrimProxies.insert(forWidget, toWidget);
    connect(forWidget, &QWidget::destroyed, forWidget, [forWidget, toWidget] {
        tScrimPrivate::scrimProxies.removeIf([forWidget, toWidget](QPair<QWidget*, QWidget*> x) {
            return x.first == forWidget && x.second == toWidget;
        });
    });
    connect(toWidget, &QWidget::destroyed, toWidget, [forWidget, toWidget] {
        tScrimPrivate::scrimProxies.removeIf([forWidget, toWidget](QPair<QWidget*, QWidget*> x) {
            return x.first == forWidget && x.second == toWidget;
        });
    });
}

QWidget* tScrim::scrimWidget(QWidget* widget) {
    if (tScrimPrivate::scrimProxies.contains(widget)) {
        return tScrimPrivate::scrimProxies.value(widget);
    }
    return widget;
}

void tScrim::addIgnoreWidget(QWidget* ignoreWidget) {
    connect(ignoreWidget, &QWidget::destroyed, this, [this, ignoreWidget] {
        d->ignoreWidgets.remove(ignoreWidget);
    });
    d->ignoreWidgets.insert(ignoreWidget);
}

void tScrim::setBlurEnabled(bool blurEnabled) {
    d->blurEffect->setEnabled(blurEnabled);
    this->setAutoFillBackground(blurEnabled);
}

void tScrim::show() {
    if (d->showing) return;

    d->showing = true;
    QWidget::show();
    QWidget::raise();
    d->anim->setDirection(tVariantAnimation::Forward);
    d->anim->start();
}

void tScrim::hide() {
    if (!d->showing) return;

    d->showing = false;
    d->anim->setDirection(tVariantAnimation::Backward);
    d->anim->start();
}

void tScrim::updateGeometry() {
    this->setGeometry(-d->scrimOverscan, -d->scrimOverscan, d->parentWidget->width() + 2 * d->scrimOverscan, d->parentWidget->height() + 2 * d->scrimOverscan);
}

void tScrim::paintEvent(QPaintEvent* event) {
    if (d->blurEffect->isEnabled()) {
        QPixmap image(d->parentWidget->width(), d->parentWidget->height());
        image.fill(d->parentWidget->palette().color(QPalette::Window));

        QPainter imagePainter(&image);
        QObjectList children = d->parentWidget->children();
        for (QObject* child : children) {
            if (child == this || d->ignoreWidgets.contains(qobject_cast<QWidget*>(child))) continue;
            if (QWidget* childWidget = qobject_cast<QWidget*>(child)) {
                if (!childWidget->isVisible()) continue;
                childWidget->render(&imagePainter, childWidget->geometry().topLeft(), QRegion(), QWidget::DrawChildren);
            }
        }
        imagePainter.end();

        QPainter p(this);
        p.setOpacity(d->opacity);
        p.drawPixmap(QRect(QPoint(d->scrimOverscan, d->scrimOverscan), image.size()), image);
    } else {
        QPainter p(this);
        p.setOpacity(1 - d->opacity);
        p.setPen(Qt::transparent);
        p.setBrush(d->parentWidget->palette().color(QPalette::Window));
        p.drawRect(0, 0, this->width(), this->height());
    }
}

bool tScrim::eventFilter(QObject* watched, QEvent* event) {
    if (watched == this->parentWidget() && event->type() == QEvent::Resize) {
        updateGeometry();
    }
    return false;
}

void tScrim::mousePressEvent(QMouseEvent* event) {
}

void tScrim::mouseReleaseEvent(QMouseEvent* event) {
    if (this->underMouse()) emit scrimClicked();
}
