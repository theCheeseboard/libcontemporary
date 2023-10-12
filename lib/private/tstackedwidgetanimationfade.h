#ifndef TSTACKEDWIDGETANIMATIONFADE_H
#define TSTACKEDWIDGETANIMATIONFADE_H

#include "tstackedwidgetanimation.h"

class tStackedWidget;
struct tStackedWidgetAnimationFadePrivate;
class tStackedWidgetAnimationFade : public tStackedWidgetAnimation {
        Q_OBJECT
    public:
        explicit tStackedWidgetAnimationFade(int oldIndex, int newIndex, tStackedWidget* parent);
        ~tStackedWidgetAnimationFade();

        void startAnimation();
        void stopAnimation();
        void setProgress(double progress);

    signals:

    private:
        tStackedWidgetAnimationFadePrivate* d;
        void internalSetReversed(bool reversed);
};

#endif // TSTACKEDWIDGETANIMATIONFADE_H
