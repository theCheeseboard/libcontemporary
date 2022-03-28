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

#ifndef TSTACKEDWIDGET_H
#define TSTACKEDWIDGET_H

#include <QStackedWidget>
#include <tpropertyanimation.h>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QSettings>
#include <QWindow>
#include <QDebug>
#include <QApplication>

class tStackedWidgetPrivate;

class LIBCONTEMPORARY_EXPORT tStackedWidget : public QStackedWidget {
        Q_OBJECT
        Q_PROPERTY(Animation CurrentAnimation READ CurrentAnimation WRITE setCurrentAnimation NOTIFY CurrentAnimationChanged)

    public:
        explicit tStackedWidget(QWidget* parent = nullptr);
        ~tStackedWidget();

        enum Animation {
            None,
            SlideHorizontal,
            SlideVertical,
            Fade,
            Lift
        };
        Animation CurrentAnimation();

        int addWidget(QWidget* w);
        int insertWidget(int index, QWidget* w);

    signals:
        void switchingFrame(int switchTo);
        void CurrentAnimationChanged(Animation currentAnimation);
        void resized();
        void widgetAdded();

    public slots:
        void setCurrentIndex(int index, bool doAnimation = true);
        void setCurrentWidget(QWidget* w, bool doAnimation = true);
        void setCurrentAnimation(Animation animation);

    private:
        tStackedWidgetPrivate* d;

        void resizeEvent(QResizeEvent* event);

        void doSetCurrentIndex(int index);
};

#endif // ANIMATEDSTACKEDWIDGET_H
