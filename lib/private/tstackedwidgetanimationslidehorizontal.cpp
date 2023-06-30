#include "tstackedwidgetanimationslidehorizontal.h"

#include "tstackedwidget.h"
#include "tvariantanimation.h"
#include <QWidget>

struct tStackedWidgetAnimationSlideHorizontalPrivate {
        tVariantAnimation* animation;
};

tStackedWidgetAnimationSlideHorizontal::tStackedWidgetAnimationSlideHorizontal(int oldIndex, int newIndex, tStackedWidget* parent) :
    tStackedWidgetAnimation{oldIndex, newIndex, parent} {
    d = new tStackedWidgetAnimationSlideHorizontalPrivate();
}

tStackedWidgetAnimationSlideHorizontal::~tStackedWidgetAnimationSlideHorizontal() {
    delete d;
}

void tStackedWidgetAnimationSlideHorizontal::startAnimation() {
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
        auto geometry = QRect(QPoint(0, 0), this->parentStack()->size());

        QRect currentWidgetRect;
        QRect newWidgetRect;
        if (moveLeft) {
            newWidgetRect = geometry.translated(this->parentStack()->width() * (1 - progress), 0);
            currentWidgetRect = geometry.translated(-this->parentStack()->width() / 8 * progress, 0);
        } else {
            newWidgetRect = geometry.translated(-this->parentStack()->width() * (1 - progress), 0);
            currentWidgetRect = geometry.translated(this->parentStack()->width() / 8 * progress, 0);
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

void tStackedWidgetAnimationSlideHorizontal::stopAnimation() {
    d->animation->stop();
    this->deleteLater();
}
