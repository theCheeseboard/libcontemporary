#ifndef TSTACKEDWIDGETANIMATIONSLIDEVERTICAL_H
#define TSTACKEDWIDGETANIMATIONSLIDEVERTICAL_H

#include "tstackedwidgetanimation.h"

struct tStackedWidgetAnimationSlideVerticalPrivate;
class tStackedWidgetAnimationSlideVertical : public tStackedWidgetAnimation {
        Q_OBJECT
    public:
        explicit tStackedWidgetAnimationSlideVertical(int oldIndex, int newIndex, tStackedWidget* parent);
        ~tStackedWidgetAnimationSlideVertical();

        void startAnimation();
        void stopAnimation();

    signals:

    private:
        tStackedWidgetAnimationSlideVerticalPrivate* d;
};

#endif // TSTACKEDWIDGETANIMATIONSLIDEVERTICAL_H
