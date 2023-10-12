/*******************************************************************************
 *
 * libcontemporary - Common libraries
 * Copyright (C) 2022 Victor Tran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include "tstackedwidget.h"

#include "private/tstackedwidgetanimationcontroller.h"
#include "private/tstackedwidgetanimationfade.h"
#include "private/tstackedwidgetanimationlift.h"
#include "private/tstackedwidgetanimationslidehorizontal.h"
#include "private/tstackedwidgetanimationslidevertical.h"
#include <QWheelEvent>

class tStackedWidgetPrivate {
    public:
        tStackedWidgetPrivate(tStackedWidget* parent) :
            animationController(parent) {
            this->parent = parent;
        }

        int animatingIndex = -1;

        QSettings settings;

        tStackedWidget::Animation anim = tStackedWidget::None;

        tStackedWidgetAnimationController animationController;

        tStackedWidget* parent;
        QWidget* defaultWidget = nullptr;

        bool backGestureInProgress = false;
        int backGestureProgress = 0;
        bool autoNavigable = false;
};

tStackedWidget::tStackedWidget(QWidget* parent) :
    QStackedWidget(parent) {
    d = new tStackedWidgetPrivate(this);
    connect(&d->animationController, &tStackedWidgetAnimationController::done, this, [this](int newIndex) {
        QStackedWidget::setCurrentIndex(newIndex);
        widget(newIndex)->raise();
    });
}

tStackedWidget::~tStackedWidget() {
    if (d->defaultWidget) d->defaultWidget->deleteLater();
    delete d;
}

void tStackedWidget::setCurrentIndex(int index, Animation animation) {
    if (animation != None) {
        // Do some checks before setting the current index.
        if (d->animatingIndex != index) {
            doSetCurrentIndex(index, animation == DefaultAnimation ? d->anim : animation);
        }
    } else {
        d->animatingIndex = index;
        QStackedWidget::setCurrentIndex(index);

        emit switchingFrame(index);
    }
}

void tStackedWidget::doSetCurrentIndex(int index, Animation animation) {
    d->animatingIndex = index;

    // Check if Power Stretch is on or if animations are disabled
    if (libContemporaryCommon::instance()->powerStretchEnabled() || !libContemporaryCommon::instance()->allowSystemAnimations()) {
        // Forego animations; power stretch is on
        if (d->backGestureInProgress) {
            d->animationController.completePartialAnimation(true);
            d->backGestureInProgress = false;
        }
        QStackedWidget::setCurrentIndex(index);
    } else {
        // Forcibly set the current index.
        QWidget* currentWidget = widget(currentIndex());
        QWidget* nextWidget = widget(index);
        if (nextWidget == nullptr) {
            QStackedWidget::setCurrentIndex(index);
        } else if (d->backGestureInProgress) {
            if (index == currentIndex()) {
                d->animationController.completePartialAnimation(false);
            } else {
                d->animationController.completePartialAnimation(true);
            }
            d->backGestureInProgress = false;
        } else {
            switch (animation) {
                case None:
                    {
                        QStackedWidget::setCurrentIndex(index);
                        break;
                    }
                case SlideHorizontal:
                    {
                        d->animationController.startAnimation(new tStackedWidgetAnimationSlideHorizontal(currentIndex(), index, this));
                        break;
                    }
                case SlideVertical:
                    {
                        d->animationController.startAnimation(new tStackedWidgetAnimationSlideVertical(currentIndex(), index, this));
                        break;
                    }
                case Fade:
                    {
                        d->animationController.startAnimation(new tStackedWidgetAnimationFade(currentIndex(), index, this));
                        break;
                    }
                case Lift:
                    {
                        d->animationController.startAnimation(new tStackedWidgetAnimationLift(currentIndex(), index, this));
                        break;
                    }
            }
        }
    }
    emit switchingFrame(index);
}

void tStackedWidget::setCurrentWidget(QWidget* w, bool doAnimation) {
    this->setCurrentIndex(indexOf(w), doAnimation);
}

void tStackedWidget::setCurrentWidget(QWidget* w, Animation animation) {
    this->setCurrentIndex(indexOf(w), animation);
}

tStackedWidget::Animation tStackedWidget::CurrentAnimation() {
    return d->anim;
}

void tStackedWidget::setCurrentAnimation(Animation animation) {
    if (animation == DefaultAnimation) return;
    d->anim = animation;
    emit CurrentAnimationChanged(animation);
}

int tStackedWidget::addWidget(QWidget* w) {
    if (d->defaultWidget) d->defaultWidget->hide();
    w->setAutoFillBackground(true);
    int index = QStackedWidget::addWidget(w);
    emit widgetAdded();
    emit switchingFrame(this->currentIndex());
    return index;
}

int tStackedWidget::insertWidget(int index, QWidget* w) {
    if (d->defaultWidget) d->defaultWidget->hide();
    w->setAutoFillBackground(true);
    int indexReturn = QStackedWidget::insertWidget(index, w);
    emit widgetAdded();
    emit switchingFrame(this->currentIndex());
    return indexReturn;
}

void tStackedWidget::removeWidget(QWidget* w) {
    emit removingWidget(w);
    QStackedWidget::removeWidget(w);

    if (this->count() == 0 && d->defaultWidget) d->defaultWidget->show();
    emit switchingFrame(this->currentIndex());
}

void tStackedWidget::setDefaultWidget(QWidget* defaultWidget) {
    d->defaultWidget = defaultWidget;
    if (defaultWidget) {
        d->defaultWidget->setGeometry(QRect(QPoint(0, 0), this->size()));
        d->defaultWidget->setParent(this);
        if (this->count() == 0) d->defaultWidget->show();
        emit switchingFrame(this->currentIndex());
    }
}

void tStackedWidget::setAutoNavigable(bool autoNavigable) {
    d->autoNavigable = autoNavigable;
}

void tStackedWidget::setCurrentIndex(int index, bool doAnimation) {
    this->setCurrentIndex(index, doAnimation ? d->anim : None);
}

void tStackedWidget::resizeEvent(QResizeEvent* event) {
    if (d->defaultWidget) d->defaultWidget->resize(this->size());
    emit resized();
}

void tStackedWidget::wheelEvent(QWheelEvent* event) {
    if (!d->autoNavigable) {
        event->ignore();
        return;
    }

    if (event->isBeginEvent() && currentIndex() != 0) {
        d->backGestureInProgress = true;
        d->backGestureProgress = 0;
        d->animationController.partialStartAnimation(new tStackedWidgetAnimationSlideHorizontal(currentIndex(), currentIndex() - 1, this, false));
        event->accept();
    }

    if (d->backGestureInProgress && (event->phase() == Qt::ScrollMomentum || event->phase() == Qt::ScrollEnd)) {
        // Finalise the gesture
        bool finish = d->backGestureProgress / static_cast<double>(this->width()) * 1.5 > 0.5;
        if (event->phase() == Qt::ScrollMomentum) {
            finish = event->pixelDelta().x() > 10;
        }
        this->doSetCurrentIndex(currentIndex() - finish, this->CurrentAnimation());
        event->accept();
        return;
    }

    if (event->pixelDelta().x() == 0) return;
    if (d->backGestureInProgress) {
        d->backGestureProgress += event->pixelDelta().x();
        d->animationController.partialAnimationProgress(d->backGestureProgress / static_cast<double>(this->width()) * 1.5);
        event->accept();
    }

    event->ignore();
}
