mod browser_app;

use browser_app::BrowserApp;
use tracing::{error, info};
// use tracing_subscriber;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Initialize logging
    tracing_subscriber::fmt()
        .with_env_filter("browser=debug,info")
        .init();

    info!("🚀 Starting Apollo Browser Engine");

    // Create and initialize browser application
    let mut app = BrowserApp::new();

    match app.initialize().await {
        Ok(_) => {
            info!("✅ Browser application initialized successfully");

            // Show welcome message
            println!("🚀 Welcome to Apollo Browser Engine!");
            println!("=====================================");
            println!("A complete browser engine built with Rust and C++");
            println!();

            // Show engine status
            app.show_engine_status();

            // Run interactive mode
            app.run_interactive().await?;
        }
        Err(e) => {
            error!("❌ Failed to initialize browser application: {}", e);
            return Err(e);
        }
    }

    info!("👋 Browser application shutdown complete");
    Ok(())
}
