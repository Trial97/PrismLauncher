// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2025 Trial97 <alexandru.tripon97@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ui/widgets/custom/RangeSlider.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>

RangeSlider::RangeSlider(QWidget* parent) : RangeSlider(Qt::Orientation::Horizontal, parent) {}

RangeSlider::RangeSlider(Qt::Orientation orientation, QWidget* parent) : QSlider(orientation, parent), m_lowerValue(0)
{
    setRange(0, 100);
    setValue(100);
}

int RangeSlider::getLower() const
{
    return m_lowerValue;
}

int RangeSlider::getUpper() const
{
    return value();
}

void RangeSlider::setLowerValue(int value)
{
    if (value <= getUpper() && value >= minimum()) {
        m_lowerValue = value;
        update();
    }
}

void RangeSlider::setUpperValue(int value)
{
    if (value >= getLower() && value <= maximum()) {
        setValue(value);
    }
}

void RangeSlider::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the slider track
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    // Draw the first groove (min to lower handle)
    opt.subControls = QStyle::SC_SliderGroove;
    opt.sliderPosition = 0;
    opt.sliderValue = 0;
    // opt.maximum = lowerValue;
    QRect lowerGroove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    if (orientation() == Qt::Horizontal) {
        lowerGroove.setRight(valueToPosition(getLower()));
    } else {
        lowerGroove.setTop(valueToPosition(getLower()));
    }
    painter.setClipRect(lowerGroove);
    style()->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);

    // Draw the second groove (lower handle to max)
    opt.sliderPosition = getUpper();
    opt.sliderValue = getUpper();
    opt.maximum = maximum();
    QRect upperGroove = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    if (orientation() == Qt::Horizontal) {
        upperGroove.setLeft(valueToPosition(getLower()));
    } else {
        upperGroove.setBottom(valueToPosition(getLower()));
    }
    painter.setClipRect(upperGroove);
    style()->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);

    // Draw the first handle (lower value)
    opt.subControls = QStyle::SC_SliderHandle;
    opt.sliderPosition = getLower();
    opt.sliderValue = getLower();
    QRect handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    painter.setClipRect(handleRect);
    style()->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);

    // Draw the second handle (upper value)
    opt.sliderPosition = getUpper();
    opt.sliderValue = getUpper();
    handleRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    painter.setClipRect(handleRect);
    style()->drawComplexControl(QStyle::CC_Slider, &opt, &painter, this);
}

void RangeSlider::mousePressEvent(QMouseEvent* event)
{
    int pos = pick(event->pos());
    int lowerX = valueToPosition(getLower());
    int upperX = valueToPosition(getUpper());

    auto lowerDis = abs(pos - lowerX);
    auto upperDis = abs(pos - upperX);
    // Check if the user clicked on a handle
    if (lowerDis < upperDis) {
        m_activeHandle = Handle::Lower;
    } else {
        m_activeHandle = Handle::Upper;
    }
    updateBasedOnPos(pos);
}

void RangeSlider::mouseMoveEvent(QMouseEvent* event)
{
    if (m_activeHandle == Handle::None)
        return;

    updateBasedOnPos(pick(event->pos()));
}

void RangeSlider::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    m_activeHandle = Handle::None;
}

int RangeSlider::valueToPosition(int value) const
{
    int sliderLength = orientation() == Qt::Horizontal ? width() : height();
    return style()->sliderPositionFromValue(minimum(), maximum(), value, sliderLength);
}

int RangeSlider::positionToValue(int pos) const
{
    int sliderLength = orientation() == Qt::Horizontal ? width() : height();
    return style()->sliderValueFromPosition(minimum(), maximum(), pos, sliderLength);
}

void RangeSlider::updateBasedOnPos(int x)
{
    int value = positionToValue(x);

    if (singleStep()) {
        value = static_cast<int>(value / singleStep()) * singleStep();
    }
    if (m_activeHandle == Handle::Lower) {
        setLowerValue(qMin(value, getUpper()));
    } else if (m_activeHandle == Handle::Upper) {
        setUpperValue(qMax(value, getLower()));
    }
}

int RangeSlider::pick(const QPoint& pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}
