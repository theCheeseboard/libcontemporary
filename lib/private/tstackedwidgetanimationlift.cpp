#include "tstackedwidgetanimationlift.h"

#include "tstackedwidget.h"
#include <QWidget>

struct tStackedWidgetAnimationLiftPrivate {
        QParallelAnimationGroup* animation;
};

tStackedWidgetAnimationLift::tStackedWidgetAnimationLift(int oldIndex, int newIndex, tStackedWidget* parent) :
    tStackedWidgetAnimation{oldIndex, newIndex, parent} {
    d = new tStackedWidgetAnimationLiftPrivate();
}

tStackedWidgetAnimationLift::~tStackedWidgetAnimationLift() {
    delete d;
}

void tStackedWidgetAnimationLift::startAnimation() {
    this->newWidget()->setGeometry(0, this->parentStack()->height() / 8, this->parentStack()->width(), this->parentStack()->height());

    this->newWidget()->show();
    this->newWidget()->raise();

    auto* group = new QParallelAnimationGroup;
    auto* animation = new tPropertyAnimation(this->newWidget(), "geometry");
    animation->setStartValue(this->newWidget()->geometry());
    animation->setEndValue(QRect(0, 0, this->parentStack()->width(), this->parentStack()->height()));
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(250);
    connect(this->parentStack(), &tStackedWidget::resized, animation, [this, animation] {
        animation->setEndValue(QRect(0, 0, this->parentStack()->width(), this->parentStack()->height()));
    });
    group->addAnimation(animation);

    auto* effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0);
    this->newWidget()->setGraphicsEffect(effect);

    auto* anim = new tPropertyAnimation(effect, "opacity");
    anim->setStartValue((float) 0);
    anim->setEndValue((float) 1);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->setDuration(250);
    group->addAnimation(anim);

    connect(group, &QParallelAnimationGroup::finished, this, [this] {
        this->emitDone();
        this->deleteLater();
    });
    group->start();
    d->animation = group;
}

void tStackedWidgetAnimationLift::stopAnimation() {
    d->animation->stop();
    this->deleteLater();
}
