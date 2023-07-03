// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2023 Tayou <git@tayou.org>
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
#include "GameOptionsHeader.h"
#include <QMouseEvent>

void GameOptionsHeader::mousePressEvent(QMouseEvent* event)
{
    int mouseX = event->pos().x();

    for (int section = count() - 1; section >= 0; section--) {
        int sectionX = sectionViewportPosition(section);

        if (mouseX > sectionX) {
            if (mouseX - sectionX < 5 && section != 0) {    // if the mouse is in the left margin and it isn't the first column
                isPressed = true;
                resizedSection = section - 1;

                return;    // skip the default implementation of this event
            }

            break;
        }
    }

    QHeaderView::mousePressEvent(event);
}
void GameOptionsHeader::mouseMoveEvent(QMouseEvent* event)
{
    if (isPressed) {
        int mouseX = event->pos().x();
        int sectionX = sectionViewportPosition(resizedSection);

        resizeSection(resizedSection, qMax(minimumSectionSize(), mouseX - sectionX));

        return;    // skip the default implementation of this event
    }

    QHeaderView::mouseMoveEvent(event);
}
void GameOptionsHeader::mouseReleaseEvent(QMouseEvent* event)
{
    if (isPressed) {
        isPressed = false;

        return;    // skip the default implementation of this event
    }

    QHeaderView::mouseReleaseEvent(event);
}
GameOptionsHeader::GameOptionsHeader(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {}
