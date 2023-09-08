// SPDX-FileCopyrightText: 2022 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
 *  Copyright (c) 2023 Trial97 <alexandru.tripon97@gmail.com>
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
 *
 */

#pragma once

#include "BaseInstance.h"

#include "modplatform/flame/FlameAPI.h"
#include "modplatform/flame/FlamePackIndex.h"

#include "net/NetJob.h"

#include "ui/pages/instance/ManagedPackPage.h"

#include <QWidget>

class FlameManagedPackPage final : public ManagedPackPage {
    Q_OBJECT

   public:
    FlameManagedPackPage(BaseInstance* inst, InstanceWindow* instance_window, QWidget* parent = nullptr);
    ~FlameManagedPackPage() override = default;

    void parseManagedPack() override;
    [[nodiscard]] QString url() const override;

   public slots:
    void suggestVersion() override;

    void update() override;
    void updateFromFile() override;

   private:
    NetJob::Ptr m_fetch_job = nullptr;

    Flame::IndexedPack m_pack;
    FlameAPI m_api;
};
