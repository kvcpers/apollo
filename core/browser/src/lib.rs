pub mod document;
pub mod engine;
pub mod error;
pub mod events;
pub mod navigation;
pub mod tab;
pub mod window;

pub use document::Document;
pub use engine::BrowserEngine;
pub use error::{BrowserError, BrowserResult};
pub use events::{EventListener, EventTarget};
pub use navigation::NavigationController;
pub use tab::Tab;
pub use window::Window;

use std::sync::Arc;
use tokio::sync::RwLock;

/// Main browser application
pub struct Browser {
    engine: Arc<RwLock<BrowserEngine>>,
    windows: Vec<Arc<RwLock<Window>>>,
    is_running: bool,
}

impl Browser {
    pub fn new() -> Self {
        Self {
            engine: Arc::new(RwLock::new(BrowserEngine::new())),
            windows: Vec::new(),
            is_running: false,
        }
    }

    pub async fn initialize(&mut self) -> BrowserResult<()> {
        tracing::info!("Initializing browser engine");

        // Initialize the browser engine
        let mut engine = self.engine.write().await;
        engine.initialize().await?;

        // Create the main window
        let main_window = Arc::new(RwLock::new(Window::new()));
        self.windows.push(main_window);

        self.is_running = true;
        tracing::info!("Browser initialized successfully");

        Ok(())
    }

    pub async fn run(&mut self) -> BrowserResult<()> {
        if !self.is_running {
            self.initialize().await?;
        }

        tracing::info!("Starting browser main loop");

        // Main event loop
        loop {
            // Process events, update UI, handle input, etc.
            tokio::time::sleep(tokio::time::Duration::from_millis(16)).await; // ~60 FPS

            // Check if we should exit
            if !self.is_running {
                break;
            }
        }

        tracing::info!("Browser main loop ended");
        Ok(())
    }

    pub fn shutdown(&mut self) {
        tracing::info!("Shutting down browser");
        self.is_running = false;
    }

    pub async fn create_window(&mut self) -> BrowserResult<Arc<RwLock<Window>>> {
        let window = Arc::new(RwLock::new(Window::new()));
        self.windows.push(window.clone());
        Ok(window)
    }

    pub async fn get_main_window(&self) -> Option<Arc<RwLock<Window>>> {
        self.windows.first().cloned()
    }

    pub fn get_window_count(&self) -> usize {
        self.windows.len()
    }

    pub async fn close_window(&mut self, window: Arc<RwLock<Window>>) -> BrowserResult<()> {
        if let Some(pos) = self.windows.iter().position(|w| Arc::ptr_eq(w, &window)) {
            self.windows.remove(pos);
            tracing::info!("Window closed");
        }
        Ok(())
    }
}

impl Default for Browser {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_browser_creation() {
        let browser = Browser::new();
        assert_eq!(browser.get_window_count(), 0);
        assert!(!browser.is_running);
    }

    #[tokio::test]
    async fn test_browser_initialization() {
        let mut browser = Browser::new();
        browser.initialize().await.unwrap();

        assert!(browser.is_running);
        assert_eq!(browser.get_window_count(), 1);
    }

    #[tokio::test]
    async fn test_window_creation() {
        let mut browser = Browser::new();
        browser.initialize().await.unwrap();

        let window = browser.create_window().await.unwrap();
        assert_eq!(browser.get_window_count(), 2);
    }
}
