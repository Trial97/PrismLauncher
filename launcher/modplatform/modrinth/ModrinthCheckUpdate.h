#pragma once

#include "modplatform/CheckUpdateTask.h"

class ModrinthCheckUpdate : public CheckUpdateTask {
    Q_OBJECT

   public:
    ModrinthCheckUpdate(QList<Resource*>& resources,
                        std::vector<Version>& mcVersions,
                        QList<Resources::ModLoader> loadersList,
                        ResourceFolderModel* resourceModel);

   public slots:
    bool abort() override;

   protected slots:
    void executeTask() override;
    void getUpdateModsForLoader(std::optional<Resources::ModLoaders> loader = {}, bool forceModLoaderCheck = false);
    void checkVersionsResponse(QByteArray* response, std::optional<Resources::ModLoaders> loader);
    void checkNextLoader();

   private:
    Task::Ptr m_job = nullptr;
    QHash<QString, Resource*> m_mappings;
    QString m_hashType;
    int m_loaderIdx = 0;
    qsizetype m_initialSize = 0;
};
