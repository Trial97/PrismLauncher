
#[cxx::bridge]
pub mod ffi {
    #[namespace = "prism::hematite::log"]
    unsafe extern "C++" {
        include!("hematite_static/debug.h");
        fn debug(msg: &str);
        fn warn(msg: &str);
        fn info(msg: &str);
    }

    #[namespace = "prism::hematite::log"]
    extern "Rust" {
        fn setup_rust_tracing_qdebug();
    }
}
fn setup_rust_tracing_qdebug() {
    use tracing_subscriber::prelude::*;
    tracing_subscriber::registry().with(QDebugLayer).init();
}

#[derive(Default)]
pub struct QDebugVisitor {
    field_lines: Vec<String>,
}

impl QDebugVisitor {
    pub fn lines(&self) -> String {
        self.field_lines.join("\n")
    }
}

impl tracing::field::Visit for QDebugVisitor {
    fn record_error(
        &mut self,
        field: &tracing::field::Field,
        value: &(dyn std::error::Error + 'static),
    ) {
        self.field_lines
            .push(format!("  field={} value={}", field.name(), value));
    }
    fn record_debug(&mut self, field: &tracing::field::Field, value: &dyn std::fmt::Debug) {
        self.field_lines
            .push(format!("  field={} value={:?}", field.name(), value));
    }
}

pub struct QDebugLayer;
impl<S> tracing_subscriber::Layer<S> for QDebugLayer
where
    S: tracing::Subscriber,
{
    fn on_event(
        &self,
        event: &tracing::Event<'_>,
        _ctx: tracing_subscriber::layer::Context<'_, S>,
    ) {
        let mut msg = indoc::formatdoc!(
            "
            [{:?}] {:?}
                {:?}
                
            
        ",
            event.metadata().level(),
            event.metadata().target(),
            event.metadata().name()
        );

        let mut visitor = QDebugVisitor::default();
        event.record(&mut visitor);
        let field_lines: String = visitor.lines();
        if field_lines.len() > 0 {
            msg.push_str("\n  ");
            msg.push_str(&field_lines);
        }

        match event.metadata().level() {
            &tracing::Level::WARN => ffi::warn(&msg),
            &tracing::Level::INFO => ffi::info(&msg),
            _ => ffi::debug(&msg),
        }
    }
}
