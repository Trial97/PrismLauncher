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

#pragma once

#include <QSlider>

class RangeSlider : public QSlider {
    Q_OBJECT

   public:
    explicit RangeSlider(QWidget* parent = nullptr);
    explicit RangeSlider(Qt::Orientation orientation, QWidget* parent = nullptr);

    int getLower() const;
    int getUpper() const;

    void setLowerValue(int value);
    void setUpperValue(int value);

   protected:
    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

   private:
    int valueToPosition(int value) const;
    int positionToValue(int position) const;
    void updateBasedOnPos(int x);
    int pick(const QPoint& pt) const;

   private:
    enum class Handle { None, Lower, Upper };
    Handle m_activeHandle = Handle::None;

    int m_lowerValue;
};
