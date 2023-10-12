#include "tstackedwidgetanimation.h"

#include "tstackedwidget.h"

struct tStackedWidgetAnimationPrivate {
        QWidget* oldWidget;
        QWidget* newWidget;
        tStackedWidget* parentStack;
        bool movingForwards;
        int oldIndex;
        int newIndex;
        bool reversed = false;
};

tStackedWidgetAnimation::tStackedWidgetAnimation(int oldIndex, int newIndex, tStackedWidget* parent) :
    QObject{parent} {
    d = new tStackedWidgetAnimationPrivate();
    d->oldWidget = parent->widget(oldIndex);
    d->newWidget = parent->widget(newIndex);
    d->parentStack = parent;
    d->movingForwards = oldIndex < newIndex;
    d->oldIndex = oldIndex;
    d->newIndex = newIndex;
}

tStackedWidgetAnimation::~tStackedWidgetAnimation() {
    delete d;
}

QWidget* tStackedWidgetAnimation::oldWidget() {
    return d->oldWidget;
}

QWidget* tStackedWidgetAnimation::newWidget() {
    return d->newWidget;
}

tStackedWidget* tStackedWidgetAnimation::parentStack() {
    return d->parentStack;
}

bool tStackedWidgetAnimation::movingForwards() {
    return d->movingForwards;
}

void tStackedWidgetAnimation::setReversed(bool reversed) {
    d->reversed = reversed;
    this->internalSetReversed(reversed);
}

void tStackedWidgetAnimation::emitDone() {
    emit done(d->reversed ? d->oldIndex : d->newIndex);
}
