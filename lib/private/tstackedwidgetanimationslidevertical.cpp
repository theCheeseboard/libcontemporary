#include "tstackedwidgetanimationslidevertical.h"

#include "tstackedwidget.h"
#include "tvariantanimation.h"
#include <QWidget>

struct tStackedWidgetAnimationSlideVerticalPrivate {
        tVariantAnimation* animation;
};

tStackedWidgetAnimationSlideVertical::tStackedWidgetAnimationSlideVertical(int oldIndex, int newIndex, tStackedWidget* parent) :
    tStackedWidgetAnimation{oldIndex, newIndex, parent} {
    d = new tStackedWidgetAnimationSlideVerticalPrivate();
}

tStackedWidgetAnimationSlideVertical::~tStackedWidgetAnimationSlideVertical() {
    delete d;
}

void tStackedWidgetAnimationSlideVertical::startAnimation() {
    this->newWidget()->show();
    this->newWidget()->raise();

    auto* animation = new tVariantAnimation();
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(250);
    connect(animation, &tVariantAnimation::valueChanged, this, [this](QVariant value) {
        auto progress = value.toReal();
        auto geometry = QRect(QPoint(0, 0), this->parentStack()->size());

        QRect currentWidgetRect;
        QRect newWidgetRect;
        if (this->movingForwards()) {
            newWidgetRect = geometry.translated(0, this->parentStack()->height() * (1 - progress));
            currentWidgetRect = geometry.translated(0, -this->parentStack()->height() / 8 * progress);
        } else {
            newWidgetRect = geometry.translated(0, -this->parentStack()->height() * (1 - progress));
            currentWidgetRect = geometry.translated(0, this->parentStack()->height() / 8 * progress);
        }

        this->oldWidget()->setGeometry(currentWidgetRect);
        this->newWidget()->setGeometry(newWidgetRect);
    });
    connect(animation, &tPropertyAnimation::finished, this, [this] {
        this->emitDone();
        this->deleteLater();
    });
    animation->start();
    d->animation = animation;
}

void tStackedWidgetAnimationSlideVertical::stopAnimation() {
    d->animation->stop();
    this->deleteLater();
}
