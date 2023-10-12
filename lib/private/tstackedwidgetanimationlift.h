#ifndef TSTACKEDWIDGETANIMATIONLIFT_H
#define TSTACKEDWIDGETANIMATIONLIFT_H

#include "tstackedwidgetanimation.h"

class tStackedWidget;
struct tStackedWidgetAnimationLiftPrivate;
class tStackedWidgetAnimationLift : public tStackedWidgetAnimation {
        Q_OBJECT
    public:
        explicit tStackedWidgetAnimationLift(int oldIndex, int newIndex, tStackedWidget* parent);
        ~tStackedWidgetAnimationLift();

        void startAnimation();
        void stopAnimation();
        void setProgress(double progress);

    signals:

    private:
        tStackedWidgetAnimationLiftPrivate* d;

        void internalSetReversed(bool reversed);
};

#endif // TSTACKEDWIDGETANIMATIONLIFT_H
