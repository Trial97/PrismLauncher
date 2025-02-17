//Generated build.rs, modify as needed
use cxx_qt_build::{CxxQtBuilder, Interface};
use std::path::PathBuf;

const HEADER_DIR: &str = "hematite-static";

fn header_dir() -> PathBuf {
    PathBuf::from(std::env::var("OUT_DIR").unwrap())
        .join("include")
        .join(HEADER_DIR)
}

fn write_headers() {
    println!("cargo::rerun-if-changed=include/");
    std::fs::create_dir_all(header_dir()).expect("Failed to create include directory");

    let headers = ["log.h"];

    for file_path in headers {
        println!("cargo::rerun-if-changed=include/{file_path}");
        std::fs::copy(format!("include/{file_path}"), header_dir().join(file_path))
            .expect("Failed to copy header file!");
    }
}

fn main() {
    write_headers();

    let interface = Interface::default()
        .export_include_directory(header_dir(), HEADER_DIR);

    let mut builder = CxxQtBuilder::library(interface);

    let cpp_files = ["src/log.cpp"];
    let rust_bridges = ["src/lib.rs", "src/log.rs"]; // files with cxx::bridge's

    for bridge in &rust_bridges {
        builder = builder.file(bridge);
    }

    builder = builder.cc_builder(move |cc| {
        for cpp_file in &cpp_files {
            cc.file(cpp_file);
            println!("cargo::rerun-if-changed={cpp_file}");
        }
        // additional includes?
        // cc.include("../../");
    });

    // TODO: not yet supported by cxx_qt
    // builder = builder.initializer(qt_build_utils::Initializer {
    //     file: Some("src/init.cpp".into()),
    //     ..qt_build_utils::Initializer::default_signature("init_hematite_core")
    // });

    builder.build();
}
