#ifndef TSTACKEDWIDGETANIMATION_H
#define TSTACKEDWIDGETANIMATION_H

#include <QObject>

class tStackedWidget;
struct tStackedWidgetAnimationPrivate;
class tStackedWidgetAnimation : public QObject {
        Q_OBJECT
    public:
        explicit tStackedWidgetAnimation(int oldIndex, int newIndex, tStackedWidget* parent);
        ~tStackedWidgetAnimation();

        QWidget* oldWidget();
        QWidget* newWidget();
        tStackedWidget* parentStack();
        bool movingForwards();

        virtual void startAnimation() = 0;
        virtual void stopAnimation() = 0;
        virtual void setProgress(double progress) = 0;
        void setReversed(bool reversed);

    signals:
        void done(int newIndex);

    protected:
        void emitDone();

    private:
        tStackedWidgetAnimationPrivate* d;

        virtual void internalSetReversed(bool reversed) = 0;
};

#endif // TSTACKEDWIDGETANIMATION_H
