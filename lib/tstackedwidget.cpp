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

class tStackedWidgetPrivate {
    public:
        tStackedWidgetPrivate(tStackedWidget* parent) {
            this->parent = parent;
        }

        bool doingNewAnimation = false;
        int animatingIndex = -1;

        QSettings settings;

        tStackedWidget::Animation anim = tStackedWidget::None;
        std::function<void()> stopAnimations = nullptr;

        tStackedWidget* parent;
        QWidget* defaultWidget = nullptr;
};

tStackedWidget::tStackedWidget(QWidget* parent) :
    QStackedWidget(parent) {
    d = new tStackedWidgetPrivate(this);
}

tStackedWidget::~tStackedWidget() {
    if (d->defaultWidget) d->defaultWidget->deleteLater();
    delete d;
}

void tStackedWidget::setCurrentIndex(int index, Animation animation) {
    if (animation != None) {
        // Do some checks before setting the current index.
        if (d->animatingIndex != index && !d->doingNewAnimation) {
            doSetCurrentIndex(index, animation == DefaultAnimation ? d->anim : animation);
        }
    } else {
        d->animatingIndex = index;
        QStackedWidget::setCurrentIndex(index);
        d->doingNewAnimation = false;

        emit switchingFrame(index);
    }
}

void tStackedWidget::doSetCurrentIndex(int index, Animation animation) {
    d->animatingIndex = index;

    // Check if Power Stretch is on or if animations are disabled
    if (libContemporaryCommon::instance()->powerStretchEnabled() || !libContemporaryCommon::instance()->allowSystemAnimations()) {
        // Forego animations; power stretch is on
        QStackedWidget::setCurrentIndex(index);
    } else {
        // Forcibly set the current index.
        QWidget* currentWidget = widget(currentIndex());
        QWidget* nextWidget = widget(index);
        if (nextWidget == nullptr) {
            QStackedWidget::setCurrentIndex(index);
        } else {
            if (d->stopAnimations) {
                d->stopAnimations();
            }

            switch (animation) {
                case None:
                    {
                        QStackedWidget::setCurrentIndex(index);
                        break;
                    }
                case SlideHorizontal:
                    {
                        bool moveLeft;
                        if ((currentIndex() < index && !QApplication::isRightToLeft()) || (currentIndex() > index && QApplication::isRightToLeft())) {
                            moveLeft = true;
                        } else {
                            moveLeft = false;
                        }

                        nextWidget->show();
                        nextWidget->raise();

                        auto* animation = new tVariantAnimation();
                        animation->setStartValue(0.0);
                        animation->setEndValue(1.0);
                        animation->setEasingCurve(QEasingCurve::OutCubic);
                        animation->setDuration(250);
                        connect(animation, &tVariantAnimation::valueChanged, this, [this, moveLeft, nextWidget, currentWidget](QVariant value) {
                            auto progress = value.toReal();
                            auto geometry = QRect(QPoint(0, 0), this->size());

                            QRect currentWidgetRect;
                            QRect newWidgetRect;
                            if (moveLeft) {
                                newWidgetRect = geometry.translated(this->width() * (1 - progress), 0);
                                currentWidgetRect = geometry.translated(-this->width() / 8 * progress, 0);
                            } else {
                                newWidgetRect = geometry.translated(-this->width() * (1 - progress), 0);
                                currentWidgetRect = geometry.translated(this->width() / 8 * progress, 0);
                            }

                            currentWidget->setGeometry(currentWidgetRect);
                            nextWidget->setGeometry(newWidgetRect);
                        });
                        connect(animation, &tPropertyAnimation::finished, this, [this, index] {
                            QStackedWidget::setCurrentIndex(index);
                            d->doingNewAnimation = false;
                            d->stopAnimations = nullptr;
                        });
                        animation->start();

                        d->stopAnimations = [animation] {
                            animation->stop();
                        };
                        break;
                    }
                case SlideVertical:
                    {
                        bool moveUp;
                        if ((currentIndex() < index && !QApplication::isRightToLeft()) || (currentIndex() > index && QApplication::isRightToLeft())) {
                            moveUp = true;
                        } else {
                            moveUp = false;
                        }

                        nextWidget->show();
                        nextWidget->raise();

                        auto* animation = new tVariantAnimation();
                        animation->setStartValue(0.0);
                        animation->setEndValue(1.0);
                        animation->setEasingCurve(QEasingCurve::OutCubic);
                        animation->setDuration(250);
                        connect(animation, &tVariantAnimation::valueChanged, this, [this, moveUp, nextWidget, currentWidget](QVariant value) {
                            auto progress = value.toReal();
                            auto geometry = QRect(QPoint(0, 0), this->size());

                            QRect currentWidgetRect;
                            QRect newWidgetRect;
                            if (moveUp) {
                                newWidgetRect = geometry.translated(0, this->height() * (1 - progress));
                                currentWidgetRect = geometry.translated(0, -this->height() / 8 * progress);
                            } else {
                                newWidgetRect = geometry.translated(0, -this->height() * (1 - progress));
                                currentWidgetRect = geometry.translated(0, this->height() / 8 * progress);
                            }

                            currentWidget->setGeometry(currentWidgetRect);
                            nextWidget->setGeometry(newWidgetRect);
                        });
                        connect(animation, &tPropertyAnimation::finished, this, [this, index] {
                            QStackedWidget::setCurrentIndex(index);
                            d->doingNewAnimation = false;
                            d->stopAnimations = nullptr;
                        });
                        animation->start();

                        d->stopAnimations = [animation] {
                            animation->stop();
                        };
                        break;
                    }
                case Fade:
                    {
                        nextWidget->show();
                        nextWidget->raise();
                        nextWidget->resize(this->width(), this->height());

                        auto* effect = new QGraphicsOpacityEffect();
                        effect->setOpacity(0);
                        nextWidget->setGraphicsEffect(effect);

                        auto* anim = new tPropertyAnimation(effect, "opacity");
                        anim->setStartValue((float) 0);
                        anim->setEndValue((float) 1);
                        anim->setEasingCurve(QEasingCurve::OutCubic);
                        anim->setDuration(250);
                        connect(anim, &tPropertyAnimation::finished, this, [this, index, anim, effect] {
                            QStackedWidget::setCurrentIndex(index);
                            anim->deleteLater();
                            effect->deleteLater();
                            d->stopAnimations = nullptr;
                        });
                        anim->start();

                        d->stopAnimations = [anim] {
                            anim->stop();
                        };
                        break;
                    }
                case Lift:
                    {
                        nextWidget->setGeometry(0, this->height() / 8, this->width(), this->height());

                        nextWidget->show();
                        nextWidget->raise();

                        auto* group = new QParallelAnimationGroup;
                        auto* animation = new tPropertyAnimation(nextWidget, "geometry");
                        animation->setStartValue(nextWidget->geometry());
                        animation->setEndValue(QRect(0, 0, this->width(), this->height()));
                        animation->setEasingCurve(QEasingCurve::OutCubic);
                        animation->setDuration(250);
                        connect(this, &tStackedWidget::resized, animation, [this, animation] {
                            animation->setEndValue(QRect(0, 0, this->width(), this->height()));
                        });
                        group->addAnimation(animation);

                        auto* effect = new QGraphicsOpacityEffect();
                        effect->setOpacity(0);
                        nextWidget->setGraphicsEffect(effect);

                        auto* anim = new tPropertyAnimation(effect, "opacity");
                        anim->setStartValue((float) 0);
                        anim->setEndValue((float) 1);
                        anim->setEasingCurve(QEasingCurve::OutCubic);
                        anim->setDuration(250);
                        group->addAnimation(anim);

                        connect(group, &QParallelAnimationGroup::finished, this, [this, group, effect, index] {
                            QStackedWidget::setCurrentIndex(index);
                            d->doingNewAnimation = false;
                            group->deleteLater();
                            effect->deleteLater();
                            d->stopAnimations = nullptr;
                        });
                        group->start();

                        d->stopAnimations = [group] {
                            group->stop();
                        };

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
    return index;
}

int tStackedWidget::insertWidget(int index, QWidget* w) {
    if (d->defaultWidget) d->defaultWidget->hide();
    w->setAutoFillBackground(true);
    int indexReturn = QStackedWidget::insertWidget(index, w);
    emit widgetAdded();
    return indexReturn;
}

void tStackedWidget::removeWidget(QWidget* w) {
    emit removingWidget(w);
    QStackedWidget::removeWidget(w);

    if (this->count() == 0 && d->defaultWidget) d->defaultWidget->show();
}

void tStackedWidget::setDefaultWidget(QWidget* defaultWidget) {
    d->defaultWidget = defaultWidget;
    if (defaultWidget) {
        d->defaultWidget->setGeometry(QRect(QPoint(0, 0), this->size()));
        d->defaultWidget->setParent(this);
        if (this->count() == 0) d->defaultWidget->show();
    }
}

void tStackedWidget::setCurrentIndex(int index, bool doAnimation) {
    this->setCurrentIndex(index, doAnimation ? d->anim : None);
}

void tStackedWidget::resizeEvent(QResizeEvent* event) {
    if (d->defaultWidget) d->defaultWidget->resize(this->size());
    emit resized();
}
