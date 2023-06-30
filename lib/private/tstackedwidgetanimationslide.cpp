#include "tstackedwidgetanimationslide.h"

#include "tstackedwidget.h"
#include "tvariantanimation.h"
#include <QWidget>

struct tStackedWidgetAnimationSlidePrivate {
        tVariantAnimation* animation;
};

tStackedWidgetAnimationSlide::tStackedWidgetAnimationSlide(int oldIndex, int newIndex, tStackedWidget* parent) :
    tStackedWidgetAnimation{oldIndex, newIndex, parent} {
    d = new tStackedWidgetAnimationSlidePrivate();
}

tStackedWidgetAnimationSlide::~tStackedWidgetAnimationSlide() {
    delete d;
}

void tStackedWidgetAnimationSlide::startAnimation() {
    bool moveLeft;
    if ((this->movingForwards() && !QApplication::isRightToLeft()) || (!this->movingForwards() && QApplication::isRightToLeft())) {
        moveLeft = true;
    } else {
        moveLeft = false;
    }

    this->newWidget()->show();
    this->newWidget()->raise();

    auto* animation = new tVariantAnimation();
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(250);
    connect(animation, &tVariantAnimation::valueChanged, this, [this, moveLeft](QVariant value) {
        auto progress = value.toReal();
        this->oldWidget()->setGeometry(this->oldWidgetRect(progress));
        this->newWidget()->setGeometry(this->newWidgetRect(progress));
    });
    connect(animation, &tPropertyAnimation::finished, this, [this] {
        this->emitDone();
        this->deleteLater();
    });
    animation->start();
    d->animation = animation;
}

void tStackedWidgetAnimationSlide::stopAnimation() {
    d->animation->stop();
    this->deleteLater();
}
