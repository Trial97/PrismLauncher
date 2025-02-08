use cxx_qt_lib::QList;
use qobject::QString;
use qobject::QStringList;
use serde::{Deserialize, Deserializer, Serialize, Serializer};
use std::fs;
use std::path::Path;

serde_with::serde_conv!(
    pub QStringAsString,
    QString,
    |qstring: &QString| -> String {qstring.into()},
    |s: String| -> Result<QString, std::convert::Infallible> { Ok(s.into()) }
);

serde_with::serde_conv!(
    pub QStringListAsVecString,
    QStringList,
    |qstringlist: &QStringList| -> Vec<String> {
        let qlist: QList<QString> = qstringlist.into();
        qlist.iter().map(Into::into).collect::<Vec<_>>().into()
    },
    |vec: Vec<String>| -> Result<QStringList, std::convert::Infallible> {
        let qlist: QList<QString> = vec.iter().map(Into::into).collect::<Vec<_>>().into();
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
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub name: QString,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub filename: QString,
        pub side: Side,
        #[serde(with = "serde_with::As::<super::QStringListAsVecString>")]
        pub loaders: QStringList,

        #[serde(with = "serde_with::As::<super::QStringListAsVecString>")]
        pub mc_versions: QStringList,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub release_type: QString,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
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
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub mode: QString,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub url: QString,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub hash: QString,

        #[serde(rename = "hash-format")]
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        pub hash_format: QString,
    }

    #[derive(Serialize, Deserialize, Default)]
    struct Update {
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        platform: QString,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        mod_id: QString,
        #[serde(with = "serde_with::As::<super::QStringAsString>")]
        version: QString,
    }

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

    // Try parsing the content as JSON or TOML
    toml::from_str::<qobject::PackwizMod>(&file_content)
        .map_err(|e| e.message().to_string())
}

impl Default for qobject::Side {
    fn default() -> Self {
        Self::Both
    }
}

#[derive(Serialize, Deserialize, Default)]
struct Modrinth {
    #[serde(rename = "mod-id", with = "serde_with::As::<QStringAsString>")]
    mod_id: QString,
    #[serde(with = "serde_with::As::<QStringAsString>")]
    version: QString,
}

#[derive(Serialize, Deserialize, Default)]
struct CurseForge {
    #[serde(rename = "project-id", with = "serde_with::As::<QStringAsString>")]
    project_id: QString,
    #[serde(rename = "file-id", with = "serde_with::As::<QStringAsString>")]
    file_id: QString,
}


#[derive(Serialize, Deserialize, Debug)]
#[serde(tag = "type", content = "data")]
enum Platform {
    #[serde(rename = "curseforge")]
    CurseForge {
        #[serde(rename = "project-id", with = "serde_with::As::<QStringAsString>")]
        project_id: QString,
        #[serde(rename = "file-id", with = "serde_with::As::<QStringAsString>")]
        file_id: QString,
    },
    #[serde(rename = "modrinth")]
    Modrinth {
        #[serde(rename = "mod-id", with = "serde_with::As::<QStringAsString>")]
        mod_id: QString,
        #[serde(with = "serde_with::As::<QStringAsString>")]
        version: QString,
    },
}
