#ifndef TSTACKEDWIDGETANIMATIONSLIDE_H
#define TSTACKEDWIDGETANIMATIONSLIDE_H

#include "tstackedwidgetanimation.h"

struct tStackedWidgetAnimationSlidePrivate;
class tStackedWidgetAnimationSlide : public tStackedWidgetAnimation {
        Q_OBJECT
    public:
        explicit tStackedWidgetAnimationSlide(int oldIndex, int newIndex, tStackedWidget* parent, bool ease = true);
        ~tStackedWidgetAnimationSlide();

        void startAnimation();
        void stopAnimation();
        void setProgress(double progress);

    signals:

    protected:
        virtual QRect oldWidgetRect(double progress) = 0;
        virtual QRect newWidgetRect(double progress) = 0;
        bool ease();

    private:
        tStackedWidgetAnimationSlidePrivate* d;

        void internalSetReversed(bool reversed);
};

#endif // TSTACKEDWIDGETANIMATIONSLIDE_H
