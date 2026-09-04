#include "FlameModIndex.h"
#include <qhashfunctions.h>

#include <algorithm>

#include "FileSystem.h"
#include "Json.h"
#include "modplatform/ModIndex.h"
#include "modplatform/flame/FlameAPI.h"
#include "resourcesmeta/DependencyType.h"

void FlameMod::loadIndexedPack(ModPlatform::IndexedPack& pack, QJsonObject& obj)
{
    pack.addonId = Json::requireInteger(obj, "id");
    pack.provider = ModPlatform::ResourceProvider::FLAME;
    pack.name = Json::requireString(obj, "name");
    pack.slug = Json::requireString(obj, "slug");
    pack.websiteUrl = obj["links"].toObject()["websiteUrl"].toString("");
    pack.description = obj["summary"].toString("");

    QJsonObject logo = obj["logo"].toObject();
    pack.logoName = logo["title"].toString();
    pack.logoUrl = logo["thumbnailUrl"].toString();
    if (pack.logoUrl.isEmpty()) {
        pack.logoUrl = logo["url"].toString();
    }

    auto authors = obj["authors"].toArray();
    if (!authors.isEmpty()) {
        pack.authors.clear();
        for (auto authorIter : authors) {
            auto author = Json::requireObject(authorIter);
            ModPlatform::ModpackAuthor packAuthor;
            packAuthor.name = Json::requireString(author, "name");
            packAuthor.url = Json::requireString(author, "url");
            pack.authors.append(packAuthor);
        }
    }

    pack.resourceType = FlameAPI::getResourceType(obj["classId"].toInt(0));
    pack.extraDataLoaded = false;
    loadURLs(pack, obj);
}

void FlameMod::loadURLs(ModPlatform::IndexedPack& pack, QJsonObject& obj)
{
    auto linksObj = obj["links"].toObject();

    pack.extraData.issuesUrl = linksObj["issuesUrl"].toString();
    if (pack.extraData.issuesUrl.endsWith('/')) {
        pack.extraData.issuesUrl.chop(1);
    }

    pack.extraData.sourceUrl = linksObj["sourceUrl"].toString();
    if (pack.extraData.sourceUrl.endsWith('/')) {
        pack.extraData.sourceUrl.chop(1);
    }

    pack.extraData.wikiUrl = linksObj["wikiUrl"].toString();
    if (pack.extraData.wikiUrl.endsWith('/')) {
        pack.extraData.wikiUrl.chop(1);
    }

    if (!pack.extraData.body.isEmpty()) {
        pack.extraDataLoaded = true;
    }
}

void FlameMod::loadBody(ModPlatform::IndexedPack& pack)
{
    pack.extraData.body = FlameAPI::getModDescription(pack.addonId.toInt());

    if (!pack.extraData.issuesUrl.isEmpty() || !pack.extraData.sourceUrl.isEmpty() || !pack.extraData.wikiUrl.isEmpty()) {
        pack.extraDataLoaded = true;
    }
}

namespace {
QString enumToString(int hashAlgorithm)
{
    switch (hashAlgorithm) {
        default:
        case 1:
            return "sha1";
        case 2:
            return "md5";
    }
}

Resources::DependencyType mapDependency(int value)
{
    switch (value) {
        case 1:  // EmbeddedLibrary
            return Resources::DependencyType::Embedded;
        case 2:  // OptionalDependency
            return Resources::DependencyType::Optional;
        case 3:  // RequiredDependency
            return Resources::DependencyType::Required;
        case 4:  // Tool
            return Resources::DependencyType::Tool;
        case 5:  // Incompatible
            return Resources::DependencyType::Incompatible;
        case 6:  // Include
            return Resources::DependencyType::Include;
        default:
            return Resources::DependencyType::Unknown;
    }
}
}  // namespace

void FlameMod::loadIndexedPackVersions(ModPlatform::IndexedPack& pack, QJsonArray& arr)
{
    QList<ModPlatform::IndexedVersion> unsortedVersions;
    for (auto versionIter : arr) {
        auto obj = versionIter.toObject();

        auto file = loadIndexedPackVersion(obj);
        if (!file.addonId.isValid()) {
            file.addonId = pack.addonId;
        }

        if (file.fileId.isValid()) {  // Heuristic to check if the returned value is valid
            unsortedVersions.append(file);
        }
    }

    auto orderSortPredicate = [](const ModPlatform::IndexedVersion& a, const ModPlatform::IndexedVersion& b) -> bool {
        // dates are in RFC 3339 format
        return a.date > b.date;
    };
    std::ranges::sort(unsortedVersions, orderSortPredicate);
    pack.versions = unsortedVersions;
    pack.versionsLoaded = true;
}

auto FlameMod::loadIndexedPackVersion(QJsonObject& obj, bool loadChangelog) -> ModPlatform::IndexedVersion
{
    auto versionArray = Json::requireArray(obj, "gameVersions");

    ModPlatform::IndexedVersion file;
    for (auto mcVer : versionArray) {
        auto str = mcVer.toString();

        if (str.contains('.')) {
            file.mcVersion.append(str);
        }

        file.side = ModPlatform::SideType::NoSide;
        if (auto loader = str.toLower(); loader == "neoforge") {
            file.loaders |= Resources::ModLoader::NeoForge;
        } else if (loader == "forge") {
            file.loaders |= Resources::ModLoader::Forge;
        } else if (loader == "cauldron") {
            file.loaders |= Resources::ModLoader::Cauldron;
        } else if (loader == "liteloader") {
            file.loaders |= Resources::ModLoader::LiteLoader;
        } else if (loader == "fabric") {
            file.loaders |= Resources::ModLoader::Fabric;
        } else if (loader == "quilt") {
            file.loaders |= Resources::ModLoader::Quilt;
        } else if (loader == "server" || loader == "client") {
            if (!file.side.isValid()) {
                file.side = ModPlatform::SideType::fromString(loader);
            } else if (file.side != ModPlatform::SideType::fromString(loader)) {
                file.side = ModPlatform::SideType::UniversalSide;
            }
        }
    }

    file.addonId = Json::requireInteger(obj, "modId");
    file.fileId = Json::requireInteger(obj, "id");
    file.date = Json::requireString(obj, "fileDate");
    file.version = Json::requireString(obj, "displayName");
    file.downloadUrl = obj["downloadUrl"].toString();
    file.fileName = Json::requireString(obj, "fileName");
    file.fileName = FS::RemoveInvalidPathChars(file.fileName);

    ModPlatform::IndexedVersionType verType;
    switch (Json::requireInteger(obj, "releaseType")) {
        case 1:
            verType = ModPlatform::IndexedVersionType::Release;
            break;
        case 2:
            verType = ModPlatform::IndexedVersionType::Beta;
            break;
        case 3:
            verType = ModPlatform::IndexedVersionType::Alpha;
            break;
        default:
            verType = ModPlatform::IndexedVersionType::Unknown;
            break;
    }
    file.versionType = verType;

    auto hashList = obj["hashes"].toArray();
    for (auto h : hashList) {
        auto hashEntry = h.toObject();
        auto hashTypes = ModPlatform::ProviderCapabilities::hashType(ModPlatform::ResourceProvider::FLAME);
        auto hashAlgo = enumToString(hashEntry["algo"].toInt(1));
        if (hashTypes.contains(hashAlgo)) {
            file.hash = Json::requireString(hashEntry, "value");
            file.hashType = hashAlgo;
            break;
        }
    }

    auto dependencies = obj["dependencies"].toArray();
    for (auto d : dependencies) {
        auto dep = d.toObject();
        Resources::Dependency dependency;
        dependency.addonId = QString::number(Json::requireInteger(dep, "modId"));
        dependency.type = mapDependency(Json::requireInteger(dep, "relationType"));
        file.dependencies.append(dependency);
    }

    if (loadChangelog) {
        file.changelog = FlameAPI::getModFileChangelog(file.addonId.toInt(), file.fileId.toInt());
    }

    return file;
}
