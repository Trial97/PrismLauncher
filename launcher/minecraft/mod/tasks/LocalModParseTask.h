#pragma once

#include <QDebug>
#include <QObject>

#include "minecraft/mod/Mod.h"

#include "minecraft/mod/format/Info.h"
#include "tasks/Task.h"

namespace ModUtils {

PackwizV2::Info ReadFabricModInfo(QByteArray contents);
PackwizV2::Info ReadQuiltModInfo(QByteArray contents);
PackwizV2::Info ReadForgeInfo(QByteArray contents);
PackwizV2::Info ReadLiteModInfo(QByteArray contents);

bool process(QString path, ResourceType resourceType, PackwizV2::Info& mod);

bool processZIP(QString path, PackwizV2::Info& mod);
bool processFolder(QString path, PackwizV2::Info& mod);
bool processLitemod(QString path, PackwizV2::Info& mod);

/** Checks whether a file is valid as a mod or not. */
bool validate(QFileInfo file);

bool processIconPNG(const Mod& mod, QByteArray&& raw_data, QPixmap* pixmap);
bool loadIconFile(const Mod& mod, QPixmap* pixmap);
}  // namespace ModUtils

class LocalModParseTask : public Task {
    Q_OBJECT
   public:
    PackwizV2::Info result() const { return m_result; }

    [[nodiscard]] bool canAbort() const override { return true; }
    bool abort() override;

    LocalModParseTask(ResourceType type, const QFileInfo& modFile);
    void executeTask() override;

   private:
    ResourceType m_type;
    QFileInfo m_modFile;
    PackwizV2::Info m_result;

    std::atomic<bool> m_aborted = false;
};
