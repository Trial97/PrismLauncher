use cxx_qt_lib::QList;
use qobject::QString;
use qobject::QStringList;
use serde::{Deserialize, Serialize};
use std::fs;
use std::path::Path;

pub mod debug;

serde_with::serde_conv!(
    pub QStringListAsVecQString,
    QStringList,
    |qstringlist: &QStringList| -> Vec<QString> {
        let qlist: QList<QString> = qstringlist.into();
        (&qlist).into()
    },
    |vec: Vec<QString>| -> Result<QStringList, std::convert::Infallible> {
        let qlist: QList<QString> = vec.into();
        Ok((&qlist).into())
    }
);

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

    #[derive(Serialize, Deserialize)]
    pub struct PackwizMod {
        pub name: QString,
        pub filename: QString,
        pub side: Side,
        #[serde(with = "serde_with::As::<super::QStringListAsVecQString>")]
        pub loaders: QStringList,

        #[serde(with = "serde_with::As::<super::QStringListAsVecQString>")]
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

    #[derive(Serialize, Deserialize, Default)]
    struct Update {
        platform: QString,
        mod_id: QString,
        version: QString,
    }

    #[namespace = "prism::hematite::serde"]
    extern "Rust" {
        fn load_packwiz_file(file_path: &QString) -> Result<PackwizMod>;
    }
}

fn load_packwiz_file(qfile_path: &QString) -> Result<qobject::PackwizMod, String> {
    let file_path = qfile_path.to_string();
    let path = Path::new(&file_path);

    if !path.exists() {
        return Err(format!("File not found: {}", file_path));
    }

    let file_content = fs::read_to_string(path).map_err(|e| e.to_string())?;

    let mut track = serde_path_to_error::Track::new();

    let td = toml::Deserializer::new(&file_content);
    let path = serde_path_to_error::Deserializer::new(td, &mut track);

    serde_ignored::deserialize(path, |path: serde_ignored::Path| {
        tracing::warn!("Found ignored key: {path}");
    })
    .map_err(|e| {
        let path = track.path().to_string();
        format!("TOML Error: path={path} | {e}",)
    })

    // Try parsing the content as JSON or TOML
    // toml::from_str::<qobject::PackwizMod>(&file_content).map_err(|e| e.message().to_string())
}

impl Default for qobject::Side {
    fn default() -> Self {
        Self::Both
    }
}

#[derive(Serialize, Deserialize, Default)]
struct Modrinth {
    mod_id: QString,
    version: QString,
}

#[derive(Serialize, Deserialize, Default)]
struct CurseForge {
    project_id: QString,
    file_id: QString,
}

#[derive(Serialize, Deserialize, Debug)]
#[serde(tag = "type", content = "data")]
enum Platform {
    #[serde(rename = "curseforge")]
    CurseForge {
        project_id: QString,
        file_id: QString,
    },
    #[serde(rename = "modrinth")]
    Modrinth { mod_id: QString, version: QString },
}
