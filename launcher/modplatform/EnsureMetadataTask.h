#pragma once

#include "ModIndex.h"

#include "modplatform/helpers/HashUtils.h"

#include <QDir>

#include "minecraft/mod/Resource.h"
#include "resourcesmeta/Type.h"

class MinecraftInstance;

class EnsureMetadataTask : public Task {
    Q_OBJECT

   public:
    EnsureMetadataTask(Resource*, MinecraftInstance*, const QDir&, Resources::Type, Resources::Platform = Resources::Platform::Modrinth);
    EnsureMetadataTask(QList<Resource*>&,
                       MinecraftInstance*,
                       const QDir&,
                       Resources::Type,
                       Resources::Platform = Resources::Platform::Modrinth);
    EnsureMetadataTask(QHash<QString, Resource*>&,
                       MinecraftInstance*,
                       const QDir&,
                       Resources::Type,
                       Resources::Platform = Resources::Platform::Modrinth);

    ~EnsureMetadataTask() override = default;

    Task::Ptr getHashingTask() { return m_hashingTask; }

   public slots:
    bool abort() override;
   protected slots:
    void executeTask() override;

   private:
    // FIXME: Move to their own namespace
    Task::Ptr modrinthVersionsTask();
    Task::Ptr modrinthProjectsTask();

    Task::Ptr flameVersionsTask();
    Task::Ptr flameProjectsTask();

    // Helpers
    enum class RemoveFromList : std::uint8_t { Yes, No };
    void emitReady(Resource*, QString key = {}, RemoveFromList = RemoveFromList::Yes);
    void emitFail(Resource*, QString key = {}, RemoveFromList = RemoveFromList::Yes);

    // Hashes and stuff
    Hashing::Hasher::Ptr createNewHash(Resource*);
    QString getExistingHash(Resource*);

   private slots:
    void updateMetadata(ModPlatform::IndexedPack& pack, ModPlatform::IndexedVersion& ver, Resource*);
    void updateMetadataCallback(ModPlatform::IndexedPack& pack, Resource* resource);

   signals:
    void metadataReady(Resource*);
    void metadataFailed(Resource*);

   private:
    QHash<QString, Resource*> m_resources;
    MinecraftInstance* m_instance;
    QDir m_resourceDir;
    Resources::Type m_type;
    Resources::Platform m_provider;

    QHash<QString, ModPlatform::IndexedVersion> m_tempVersions;
    Task::Ptr m_hashingTask;
    Task::Ptr m_currentTask;
    QHash<QString, Task::Ptr> m_updateMetadataTasks;
};
