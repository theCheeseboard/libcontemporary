#include "tstackedwidgetanimationcontroller.h"

#include "tstackedwidgetanimation.h"
#include <QPointer>

struct tStackedWidgetAnimationControllerPrivate {
        tStackedWidget* parentStack;
        QPointer<tStackedWidgetAnimation> currentAnimation;
        double currentProgress = 0;
};

tStackedWidgetAnimationController::tStackedWidgetAnimationController(tStackedWidget* parentStack, QObject* parent) :
    QObject{parent} {
    d = new tStackedWidgetAnimationControllerPrivate();
    d->parentStack = parentStack;
}

tStackedWidgetAnimationController::~tStackedWidgetAnimationController() {
    delete d;
}

void tStackedWidgetAnimationController::startAnimation(tStackedWidgetAnimation* animation) {
    if (d->currentAnimation) {
        d->currentAnimation->stopAnimation();
    }

    d->currentAnimation = animation;
    connect(animation, &tStackedWidgetAnimation::done, this, &tStackedWidgetAnimationController::done);
    animation->startAnimation();
}

void tStackedWidgetAnimationController::stopCurrentAnimation() {
    if (d->currentAnimation) d->currentAnimation->stopAnimation();
}

void tStackedWidgetAnimationController::partialStartAnimation(tStackedWidgetAnimation* animation) {
    if (d->currentAnimation) {
        d->currentAnimation->stopAnimation();
    }

    d->currentAnimation = animation;
    connect(animation, &tStackedWidgetAnimation::done, this, &tStackedWidgetAnimationController::done);
}

void tStackedWidgetAnimationController::partialAnimationProgress(double progress) {
    if (progress < 0) progress = 0;
    if (progress > 1) progress = 1;
    d->currentAnimation->setProgress(progress);
    d->currentProgress = progress;
}

void tStackedWidgetAnimationController::completePartialAnimation(bool finish) {
    d->currentAnimation->setReversed(!finish);
    d->currentAnimation->startAnimation();
    d->currentAnimation->setProgress(d->currentProgress);
}
