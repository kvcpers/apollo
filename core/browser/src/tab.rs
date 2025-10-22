use crate::document::Document;
use crate::error::{BrowserError, BrowserResult};
use crate::navigation::NavigationController;
use std::collections::VecDeque;

/// Browser tab representing a single page/session
pub struct Tab {
    document: Option<Document>,
    navigation: NavigationController,
    tab_id: u32,
    title: String,
    url: Option<String>,
    is_loading: bool,
    loading_progress: f32,
    can_go_back: bool,
    can_go_forward: bool,
}

impl Tab {
    pub fn new() -> Self {
        Self {
            document: None,
            navigation: NavigationController::new(),
            tab_id: 0,
            title: "New Tab".to_string(),
            url: None,
            is_loading: false,
            loading_progress: 0.0,
            can_go_back: false,
            can_go_forward: false,
        }
    }

    pub fn with_id(tab_id: u32) -> Self {
        Self {
            document: None,
            navigation: NavigationController::new(),
            tab_id,
            title: "New Tab".to_string(),
            url: None,
            is_loading: false,
            loading_progress: 0.0,
            can_go_back: false,
            can_go_forward: false,
        }
    }

    /// Navigate to a URL
    pub fn navigate(&mut self, url: &str) -> BrowserResult<()> {
        tracing::info!("Navigating to: {}", url);

        self.is_loading = true;
        self.loading_progress = 0.0;

        // Add to navigation history
        self.navigation.push_entry(url.to_string());

        // Update URL
        self.url = Some(url.to_string());

        // Simulate loading progress
        self.loading_progress = 1.0;
        self.is_loading = false;

        // Update navigation state
        self.update_navigation_state();

        tracing::info!("Navigation completed to: {}", url);
        Ok(())
    }

    /// Go back in navigation history
    pub fn go_back(&mut self) -> BrowserResult<()> {
        if let Some(url) = self.navigation.go_back() {
            self.navigate(&url)?;
        } else {
            return Err(BrowserError::RuntimeError("Cannot go back".to_string()));
        }
        Ok(())
    }

    /// Go forward in navigation history
    pub fn go_forward(&mut self) -> BrowserResult<()> {
        if let Some(url) = self.navigation.go_forward() {
            self.navigate(&url)?;
        } else {
            return Err(BrowserError::RuntimeError("Cannot go forward".to_string()));
        }
        Ok(())
    }

    /// Reload the current page
    pub fn reload(&mut self) -> BrowserResult<()> {
        if let Some(url) = self.url.clone() {
            self.navigate(&url)?;
        } else {
            return Err(BrowserError::RuntimeError("No URL to reload".to_string()));
        }
        Ok(())
    }

    /// Stop loading the current page
    pub fn stop(&mut self) -> BrowserResult<()> {
        self.is_loading = false;
        self.loading_progress = 0.0;
        tracing::info!("Loading stopped");
        Ok(())
    }

    /// Set the document for this tab
    pub fn set_document(&mut self, document: Document) {
        self.document = Some(document);
    }

    /// Get the document
    pub fn get_document(&self) -> Option<&Document> {
        self.document.as_ref()
    }

    /// Get the document mutably
    pub fn get_document_mut(&mut self) -> Option<&mut Document> {
        self.document.as_mut()
    }

    /// Set the tab title
    pub fn set_title(&mut self, title: String) {
        self.title = title;
    }

    /// Get the tab title
    pub fn get_title(&self) -> &String {
        &self.title
    }

    /// Get the current URL
    pub fn get_url(&self) -> Option<&String> {
        self.url.as_ref()
    }

    /// Check if the tab is loading
    pub fn is_loading(&self) -> bool {
        self.is_loading
    }

    /// Get loading progress (0.0 to 1.0)
    pub fn get_loading_progress(&self) -> f32 {
        self.loading_progress
    }

    /// Check if can go back
    pub fn can_go_back(&self) -> bool {
        self.can_go_back
    }

    /// Check if can go forward
    pub fn can_go_forward(&self) -> bool {
        self.can_go_forward
    }

    /// Get tab ID
    pub fn get_tab_id(&self) -> u32 {
        self.tab_id
    }

    /// Update navigation state based on history
    fn update_navigation_state(&mut self) {
        self.can_go_back = self.navigation.can_go_back();
        self.can_go_forward = self.navigation.can_go_forward();
    }

    /// Get navigation history
    pub fn get_history(&self) -> &VecDeque<String> {
        self.navigation.get_history()
    }

    /// Clear navigation history
    pub fn clear_history(&mut self) {
        self.navigation.clear_history();
        self.update_navigation_state();
    }
}

impl Default for Tab {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_tab_creation() {
        let tab = Tab::new();
        assert_eq!(tab.get_title(), "New Tab");
        assert!(tab.get_url().is_none());
        assert!(!tab.is_loading());
        assert_eq!(tab.get_loading_progress(), 0.0);
        assert!(!tab.can_go_back());
        assert!(!tab.can_go_forward());
    }

    #[test]
    fn test_navigation() {
        let mut tab = Tab::new();

        // Navigate to a URL
        tab.navigate("https://example.com").unwrap();
        assert_eq!(tab.get_url(), Some(&"https://example.com".to_string()));
        assert!(!tab.is_loading());
        assert_eq!(tab.get_loading_progress(), 1.0);

        // Navigate to another URL
        tab.navigate("https://google.com").unwrap();
        assert_eq!(tab.get_url(), Some(&"https://google.com".to_string()));
        assert!(tab.can_go_back());
        assert!(!tab.can_go_forward());
    }

    #[test]
    fn test_navigation_history() {
        let mut tab = Tab::new();

        tab.navigate("https://example.com").unwrap();
        tab.navigate("https://google.com").unwrap();
        tab.navigate("https://github.com").unwrap();

        assert!(tab.can_go_back());
        assert!(!tab.can_go_forward());

        // Go back
        tab.go_back().unwrap();
        assert_eq!(tab.get_url(), Some(&"https://google.com".to_string()));
        assert!(tab.can_go_back());
        assert!(tab.can_go_forward());

        // Go forward
        tab.go_forward().unwrap();
        assert_eq!(tab.get_url(), Some(&"https://github.com".to_string()));
        assert!(tab.can_go_back());
        assert!(!tab.can_go_forward());
    }

    #[test]
    fn test_reload() {
        let mut tab = Tab::new();

        tab.navigate("https://example.com").unwrap();
        let url = tab.get_url().unwrap().clone();

        tab.reload().unwrap();
        assert_eq!(tab.get_url(), Some(&url));
    }

    #[test]
    fn test_stop_loading() {
        let mut tab = Tab::new();

        // Simulate loading state
        tab.navigate("https://example.com").unwrap();
        assert!(!tab.is_loading());

        tab.stop().unwrap();
        assert!(!tab.is_loading());
        assert_eq!(tab.get_loading_progress(), 0.0);
    }

    #[test]
    fn test_document_management() {
        let mut tab = Tab::new();
        let document = Document::new();

        assert!(tab.get_document().is_none());

        tab.set_document(document);
        assert!(tab.get_document().is_some());
    }
}
