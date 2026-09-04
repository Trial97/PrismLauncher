// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
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
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#pragma once

#include <cstdint>

#include <QDateTime>
#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QPointer>
#include <memory>

#include "resourcesmeta/Entry.h"

class MinecraftInstance;

enum class ResourceType : std::uint8_t {
    UNKNOWN,     //!< Indicates an unspecified resource type.
    ZIPFILE,     //!< The resource is a zip file containing the resource's class files.
    SINGLEFILE,  //!< The resource is a single file (not a zip file).
    FOLDER,      //!< The resource is in a folder on the filesystem.
    LITEMOD,     //!< The resource is a litemod
};

QDebug operator<<(QDebug debug, ResourceType type);

enum class ResourceStatus : std::uint8_t {
    Installed,     // Both JAR and Metadata are present
    NotInstalled,  // Only the Metadata is present
    NoMetadata,    // Only the JAR is present
    Unknown,       // Default status
};

QDebug operator<<(QDebug debug, ResourceStatus status);

enum class SortType : std::uint8_t {
    Name,
    Date,
    Version,
    Enabled,
    PackFormat,
    Provider,
    Size,
    Side,
    McVersions,
    Loaders,
    ReleaseType,
    Requires,
    RequiredBy,
    Filename,
};

enum class EnableAction : std::uint8_t { ENABLE, DISABLE, TOGGLE };

/** General class for managed resources. It mirrors a file in disk, with some more info
 *  for display and house-keeping purposes.
 *
 *  Subclass it to add additional data / behavior, such as Mods or Resource packs.
 */
class Resource {
   public:
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

   public:
    using Ptr = std::shared_ptr<Resource>;
    Resource(const QFileInfo& fileInfo);

    Resource(const QString& filePath) : Resource(QFileInfo(filePath)) {}

    virtual ~Resource() = default;

    void setFile(QFileInfo fileInfo);
    void parseFile();

    auto fileinfo() const -> QFileInfo { return m_fileInfo; }
    auto dateTimeChanged() const -> QDateTime { return m_changedDateTime; }
    auto internalId() const -> QString { return m_internalId; }
    auto type() const -> ResourceType { return m_type; }
    bool enabled() const { return m_enabled; }
    auto getOriginalFileName() const -> QString;
    QString sizeStr() const { return m_sizeStr; }
    qint64 sizeInfo() const { return m_sizeInfo; }

    virtual auto name() const -> QString;
    virtual bool valid() const { return m_type != ResourceType::UNKNOWN; }

    /** Maps this resource's locally-parsed details onto the resource-index Details schema.
     *  Default: empty (no locally-parsed info available for this resource kind). */
    virtual auto toIndexDetails() const -> Resources::Details { return {}; }

    auto hashes() const -> Resources::Hashes { return m_hashes; }
    void setHashes(Resources::Hashes hashes) { m_hashes = std::move(hashes); }

    /** The cached resource-index entry for this resource: the single canonical source for
     *  provider/hash/parsed-detail info, kept in sync with what's persisted to
     *  resources.index.json. Populated on every resolve (whether by a fresh parse or by
     *  hydration from a previously-stored entry when the file hasn't changed). */
    auto entry() const -> const Resources::Entry& { return m_entry; }
    void setEntry(Resources::Entry entry) { m_entry = std::move(entry); }

    /** The unscaled, decoded icon/pack image (if any), used to populate Details::image for the
     *  index. Separate from whatever a subclass caches in QPixmapCache for display, which is
     *  scaled down for that purpose. Settable from a const context to match the existing
     *  const icon/image-caching setters on Mod/DataPack/TexturePack. */
    auto rawImage() const -> QImage { return m_rawImage; }
    void setRawImage(QImage image) const { m_rawImage = std::move(image); }

    auto status() const -> ResourceStatus { return m_status; };
    auto provider() const -> QString;
    virtual auto homepage() const -> QString;

    void setStatus(ResourceStatus status) { m_status = status; }

    /**
     * Returns compatibility issues with the resource and the instance.
     * This is initially empty, and may be updated when calling updateIssues.
     */
    QStringList issues() const;
    void updateIssues(const MinecraftInstance* inst);
    bool hasIssues() const { return !m_issues.empty(); }

    /** Compares two Resources, for sorting purposes, considering a ascending order, returning:
     *  > 0: 'this' comes after 'other'
     *  = 0: 'this' is equal to 'other'
     *  < 0: 'this' comes before 'other'
     */
    virtual int compare(const Resource& other, SortType type = SortType::Name) const;

    /** Returns whether the given filter should filter out 'this' (false),
     *  or if such filter includes the Resource (true).
     */
    virtual bool applyFilter(const QRegularExpression& filter) const;

    /** Changes the enabled property, according to 'action'.
     *
     *  Returns whether a change was applied to the Resource's properties.
     */
    bool enable(EnableAction action);

    auto shouldResolve() const -> bool { return !m_isResolving && !m_isResolved; }
    auto isResolving() const -> bool { return m_isResolving; }
    auto isResolved() const -> bool { return m_isResolved; }
    auto resolutionTicket() const -> int { return m_resolutionTicket; }

    void setResolving(bool resolving, int resolutionTicket)
    {
        m_isResolving = resolving;
        m_resolutionTicket = resolutionTicket;
    }

    // Delete all files of this resource.
    auto destroy(bool preserveMetadata = false, bool attemptTrash = true) -> bool;
    // Delete the metadata only (clears the resource's known providers; the index entry itself
    // is rebuilt and re-saved by the caller).
    auto destroyMetadata() -> void;

    auto isSymLink() const -> bool { return m_fileInfo.isSymLink(); }

    /**
     * @brief Take a instance path, checks if the file pointed to by the resource is a symlink or under a symlink in that instance
     *
     * @param instPath path to an instance directory
     * @return true
     * @return false
     */
    bool isSymLinkUnder(const QString& instPath) const;

    bool isMoreThanOneHardLink() const;

   protected:
    /* The file corresponding to this resource. */
    QFileInfo m_fileInfo{};
    /* The cached date when this file was last changed. */
    QDateTime m_changedDateTime;

    /* Internal ID for internal purposes. Properties such as human-readability should not be assumed. */
    QString m_internalId;
    /* Name as reported via the file name. In the absence of a better name, this is shown to the user. */
    QString m_name;

    /* The type of file we're dealing with. */
    ResourceType m_type = ResourceType::UNKNOWN;

    /* Installation status of the resource. */
    ResourceStatus m_status = ResourceStatus::Unknown;

    /* Locally-computed hashes of the resource file, always including at least Sha256 once resolved. */
    Resources::Hashes m_hashes;

    /* The cached resource-index entry for this resource. See entry(). */
    Resources::Entry m_entry;

    /* The unscaled, decoded icon/pack image, if any. See rawImage(). */
    mutable QImage m_rawImage;

    /* Whether the resource is enabled (e.g. shows up in the game) or not. */
    bool m_enabled = true;

    QList<const char*> m_issues;

    /* Used to keep trach of pending / concluded actions on the resource. */
    bool m_isResolving = false;
    bool m_isResolved = false;
    int m_resolutionTicket = 0;
    QString m_sizeStr;
    qint64 m_sizeInfo = 0;
    std::uintmax_t m_hardLinkCount = 0;
};
