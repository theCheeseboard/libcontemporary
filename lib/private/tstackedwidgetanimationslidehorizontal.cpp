#include "tstackedwidgetanimationslidehorizontal.h"

#include "tstackedwidget.h"
#include <QWidget>

tStackedWidgetAnimationSlideHorizontal::tStackedWidgetAnimationSlideHorizontal(int oldIndex, int newIndex, tStackedWidget* parent) :
    tStackedWidgetAnimationSlide{oldIndex, newIndex, parent} {
}

bool tStackedWidgetAnimationSlideHorizontal::moveLeft() {
    return (this->movingForwards() && !QApplication::isRightToLeft()) || (!this->movingForwards() && QApplication::isRightToLeft());
}

QRect tStackedWidgetAnimationSlideHorizontal::oldWidgetRect(double progress) {
    auto geometry = QRect(QPoint(0, 0), this->parentStack()->size());

    if (this->moveLeft()) {
        return geometry.translated(-this->parentStack()->width() / 8 * progress, 0);
    } else {
        return geometry.translated(this->parentStack()->width() / 8 * progress, 0);
    }
}

QRect tStackedWidgetAnimationSlideHorizontal::newWidgetRect(double progress) {
    auto geometry = QRect(QPoint(0, 0), this->parentStack()->size());

    if (this->moveLeft()) {
        return geometry.translated(this->parentStack()->width() * (1 - progress), 0);
    } else {
        return geometry.translated(-this->parentStack()->width() * (1 - progress), 0);
    }
}
