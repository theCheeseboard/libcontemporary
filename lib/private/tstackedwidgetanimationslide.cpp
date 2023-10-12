#include "tstackedwidgetanimationslide.h"

#include "tstackedwidget.h"
#include "tvariantanimation.h"
#include <QWidget>

struct tStackedWidgetAnimationSlidePrivate {
        tVariantAnimation* animation;
        bool ease;
};

tStackedWidgetAnimationSlide::tStackedWidgetAnimationSlide(int oldIndex, int newIndex, tStackedWidget* parent, bool ease) :
    tStackedWidgetAnimation{oldIndex, newIndex, parent} {
    d = new tStackedWidgetAnimationSlidePrivate();
    d->ease = ease;
    bool moveLeft;
    if ((this->movingForwards() && !QApplication::isRightToLeft()) || (!this->movingForwards() && QApplication::isRightToLeft())) {
        moveLeft = true;
    } else {
        moveLeft = false;
    }

    this->newWidget()->show();
    this->newWidget()->raise();

    auto* newEffect = new QGraphicsOpacityEffect();
    newEffect->setOpacity(0);
    this->newWidget()->setGraphicsEffect(newEffect);

    //    auto* oldEffect = new QGraphicsOpacityEffect();
    //    oldEffect->setOpacity(1);
    //    this->oldWidget()->setGraphicsEffect(oldEffect);

    auto* animation = new tVariantAnimation(this);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    if (ease) animation->setEasingCurve(QEasingCurve::InOutExpo);
    animation->setDuration(300);
    connect(animation, &tVariantAnimation::valueChanged, this, [this, moveLeft, newEffect](QVariant value) {
        auto progress = value.toReal();
        this->oldWidget()->setGeometry(this->oldWidgetRect(progress));
        this->newWidget()->setGeometry(this->newWidgetRect(progress));
        //        oldEffect->setOpacity(1 - progress);
        newEffect->setOpacity(progress);
    });
    connect(animation, &tPropertyAnimation::finished, this, [this] {
        this->newWidget()->setGraphicsEffect(nullptr);
        this->oldWidget()->setGraphicsEffect(nullptr);
        this->emitDone();
        this->deleteLater();
    });
    d->animation = animation;
}

tStackedWidgetAnimationSlide::~tStackedWidgetAnimationSlide() {
    delete d;
}

void tStackedWidgetAnimationSlide::startAnimation() {
    d->animation->start();
}

void tStackedWidgetAnimationSlide::stopAnimation() {
    d->animation->stop();
    this->deleteLater();
}

void tStackedWidgetAnimationSlide::setProgress(double progress) {
    d->animation->setCurrentTime(d->animation->totalDuration() * progress);
}

bool tStackedWidgetAnimationSlide::ease() {
    return d->ease;
}

void tStackedWidgetAnimationSlide::internalSetReversed(bool reversed) {
    d->animation->setDirection(reversed ? QAbstractAnimation::Backward : QAbstractAnimation::Forward);
}
