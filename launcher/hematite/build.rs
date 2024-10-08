//Generated build.rs, modify as needed

use cxx_qt_build::CxxQtBuilder;

fn main() {
    CxxQtBuilder::new()
        // Link Qt's Network library
        // - Qt Core is always linked
        // - Qt Gui is linked by enabling the qt_gui Cargo feature (default).
        // - Qt Qml is linked by enabling the qt_qml Cargo feature (default).
        // - Qt Qml requires linking Qt Network on macOS
        // - use .qt_module("Network") qt link a Qt library e.g. Link Qt's Network library
        // .qml_module(QmlModule {
        //     uri: "org.prismlauncher.hematite.hematite_static",
        //     rust_files: &["src/cxxqt_object.rs"],
        //     qml_files: &["../qml/main.qml"],
        //     ..Default::default()
        // })
        .file("src/lib.rs")
        .cc_builder(|cc| {
            cc.include("../../");
        })
        .build();
}
