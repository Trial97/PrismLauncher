use cxx_qt_lib::QList;
use qobject::QString;
use qobject::QStringList;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fs;
use std::path::Path;
use toml;

/// The bridge definition for our QObject
#[cxx::bridge]
pub mod qobject {

    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        include!("cxx-qt-lib/qstringlist.h");
        /// An alias to the QString type
        type QString = cxx_qt_lib::QString;
        type QStringList = cxx_qt_lib::QStringList;
    }

    pub struct PackwizMod {
        pub name: QString,
        pub filename: QString,
        pub side: Side,
        pub loaders: QStringList,

        pub mc_versions: QStringList,
        pub release_type: QString,
        pub version_number: QString,

        pub download: Download,
        pub update: Update,
    }

    #[derive(Serialize, Deserialize)]
    #[serde(rename_all = "lowercase")]
    pub enum Side {
        Client,
        Server,
        Both,
    }

    #[derive(Serialize, Deserialize, Default)]
    struct Download {
        pub mode: QString,
        pub url: QString,
        pub hash: QString,

        #[serde(rename = "hash-format")]
        pub hash_format: QString,
    }

    #[derive(Default)]
    struct Update {
        platform: QString,
        mod_id: QString,
        version: QString,
    }

    extern "Rust" {
        fn load_packwiz_file(file_path: &QString) -> Result<PackwizMod>;
    }
}

#[derive(Serialize, Deserialize, Default)]
pub struct PackwizModSimple {
    pub name: QString,
    pub filename: QString,
    pub side: qobject::Side,
    #[serde(rename = "x-prismlauncher-loaders")]
    pub loaders: Vec<QString>,

    #[serde(rename = "x-prismlauncher-mc-versions")]
    pub mc_versions: Vec<QString>,

    #[serde(rename = "x-prismlauncher-release-type")]
    pub release_type: QString,

    #[serde(rename = "x-prismlauncher-version-number")]
    pub version_number: QString,

    pub download: qobject::Download,
    pub update: qobject::Update,
}

fn vec_to_qstringlist(vec: Vec<QString>) -> QStringList {
    let mut qstringlist = QList::default();
    for s in vec {
        qstringlist.append(s);
    }
    (&qstringlist).into()
}

fn load_packwiz_file(qfile_path: &QString) -> Result<qobject::PackwizMod, String> {
    let file_path = qfile_path.to_string();
    let path = Path::new(&file_path);

    if !path.exists() {
        return Err(format!("File not found: {}", file_path));
    }

    let file_content = fs::read_to_string(path).map_err(|e| e.to_string())?;

    // Try parsing the content as JSON or TOML
    toml::from_str::<PackwizModSimple>(&file_content)
        .map(|parsed_toml| qobject::PackwizMod {
            name: parsed_toml.name,
            filename: parsed_toml.filename,
            side: parsed_toml.side,
            loaders: vec_to_qstringlist(parsed_toml.loaders),
            mc_versions: vec_to_qstringlist(parsed_toml.mc_versions),
            release_type: parsed_toml.release_type,
            version_number: parsed_toml.version_number,
            download: parsed_toml.download,
            update: parsed_toml.update,
        })
        .map_err(|e| e.message().to_string())
}

impl Default for qobject::Side {
    fn default() -> Self {
        Self::Both
    }
}

#[derive(Serialize, Deserialize, Default)]
struct Modrinth {
    #[serde(rename = "mod-id")]
    mod_id: QString,
    version: QString,
}

#[derive(Serialize, Deserialize, Default)]
struct CurseForge {
    #[serde(rename = "project-id")]
    project_id: QString,
    #[serde(rename = "file-id")]
    file_id: QString,
}

impl Serialize for qobject::Update {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        match self.platform.to_string().as_str() {
            "curseforge" => {
                let curseforge = CurseForge {
                    project_id: self.mod_id.clone(),
                    file_id: self.version.clone(),
                };
                curseforge.serialize(serializer)
            }
            "modrinth" => {
                let modrinth = Modrinth {
                    mod_id: self.mod_id.clone(),
                    version: self.version.clone(),
                };
                modrinth.serialize(serializer)
            }
            _ => Err(serde::ser::Error::custom(format!(
                "Unsupported platform: {}",
                self.platform
            ))),
        }
    }
}

#[derive(Serialize, Deserialize, Debug)]
#[serde(tag = "type", content = "data")]
enum Platform {
    #[serde(rename = "curseforge")]
    CurseForge {
        #[serde(rename = "project-id")]
        project_id: QString,
        #[serde(rename = "file-id")]
        file_id: QString,
    },
    #[serde(rename = "modrinth")]
    Modrinth {
        #[serde(rename = "mod-id")]
        mod_id: QString,
        version: QString,
    },
}

impl<'de> Deserialize<'de> for qobject::Update {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        // Deserialize into the Platform enum
        let platform: Platform = Platform::deserialize(deserializer)?;

        // Convert Platform enum into Update struct
        match platform {
            Platform::CurseForge {
                project_id,
                file_id,
            } => Ok(Self {
                platform: "curseforge".into(),
                mod_id: project_id,
                version: file_id,
            }),
            Platform::Modrinth { mod_id, version } => Ok(Self {
                platform: "modrinth".into(),
                mod_id,
                version,
            }),
        }
    }
}
