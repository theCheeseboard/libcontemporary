#include "tstackedwidgetanimationslidehorizontal.h"

#include "tstackedwidget.h"
#include <QWidget>

tStackedWidgetAnimationSlideHorizontal::tStackedWidgetAnimationSlideHorizontal(int oldIndex, int newIndex, tStackedWidget* parent, bool ease) :
    tStackedWidgetAnimationSlide{oldIndex, newIndex, parent, ease} {
}

bool tStackedWidgetAnimationSlideHorizontal::moveLeft() {
    return (this->movingForwards() && !QApplication::isRightToLeft()) || (!this->movingForwards() && QApplication::isRightToLeft());
}

QRect tStackedWidgetAnimationSlideHorizontal::oldWidgetRect(double progress) {
    auto geometry = QRect(QPoint(0, 0), this->parentStack()->size());

    auto translation = this->ease() ? 50 : geometry.width() / 2;

    if (this->moveLeft()) {
        return geometry.translated(-translation * progress, 0);
    } else {
        return geometry.translated(translation * progress, 0);
    }
}

QRect tStackedWidgetAnimationSlideHorizontal::newWidgetRect(double progress) {
    auto geometry = QRect(QPoint(0, 0), this->parentStack()->size());

    auto translation = this->ease() ? 50 : geometry.width() / 2;

    if (this->moveLeft()) {
        return geometry.translated(translation * (1 - progress), 0);
    } else {
        return geometry.translated(-translation * (1 - progress), 0);
    }
}
