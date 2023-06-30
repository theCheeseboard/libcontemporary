#ifndef TSTACKEDWIDGETANIMATIONSLIDEHORIZONTAL_H
#define TSTACKEDWIDGETANIMATIONSLIDEHORIZONTAL_H

#include "tstackedwidgetanimation.h"

struct tStackedWidgetAnimationSlideHorizontalPrivate;
class tStackedWidgetAnimationSlideHorizontal : public tStackedWidgetAnimation {
        Q_OBJECT
    public:
        explicit tStackedWidgetAnimationSlideHorizontal(int oldIndex, int newIndex, tStackedWidget* parent);
        ~tStackedWidgetAnimationSlideHorizontal();

        void startAnimation();
        void stopAnimation();

    signals:

    private:
        tStackedWidgetAnimationSlideHorizontalPrivate* d;
};

#endif // TSTACKEDWIDGETANIMATIONSLIDEHORIZONTAL_H
