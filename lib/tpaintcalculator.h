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
#ifndef TPAINTCALCULATOR_H
#define TPAINTCALCULATOR_H

#include "libcontemporary_global.h"
#include <QObject>
#include <QSharedDataPointer>

struct tPaintCalculatorPrivate;
class LIBCONTEMPORARY_EXPORT tPaintCalculator {
    public:
        tPaintCalculator();
        tPaintCalculator(const tPaintCalculator& other);
        ~tPaintCalculator();

        typedef std::function<void(QRectF)> DrawFunction;

        void setPainter(QPainter* painter);
        void setDrawBounds(QRectF bounds);
        void setDrawBounds(QSizeF size);

        void addRect(QRectF rect, DrawFunction drawFunction);
        void addRect(QString name, QRectF rect, DrawFunction drawFunction);
        void performPaint();
        void performPaint(QString rectName);
        QRectF boundsOf(QString name) const;
        QRectF boundingRect() const;
        QRectF anchoredBoundingRect() const;
        QRectF visualBoundingRect() const;
        QSizeF sizeWithMargins() const;
        QSizeF sizeWithMargins(QPointF origin) const;
        QStringList rectNames() const;

        void setBoundsCalculationExcludeList(QStringList excludeList);

        QStringList hitTest(QPointF point, bool includeDefaultNamed = false) const;

        Qt::LayoutDirection layoutDirection() const;
        void setLayoutDirection(Qt::LayoutDirection direction);

    private:
        tPaintCalculatorPrivate* d;
};

struct tPaintCalculatorScoperPrivate;
class LIBCONTEMPORARY_EXPORT tPaintCalculatorScoper {
    public:
        tPaintCalculatorScoper(tPaintCalculator* paintCalculator);
        ~tPaintCalculatorScoper();

    private:
        tPaintCalculatorScoperPrivate* d;
};

#endif // TPAINTCALCULATOR_H
