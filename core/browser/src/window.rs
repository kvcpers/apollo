use crate::error::{BrowserError, BrowserResult};
use crate::tab::Tab;
use std::collections::HashMap;

/// Browser window containing multiple tabs
pub struct Window {
    tabs: Vec<Tab>,
    active_tab_index: usize,
    window_id: u32,
    title: String,
    width: u32,
    height: u32,
    x: i32,
    y: i32,
    is_visible: bool,
    is_maximized: bool,
    is_minimized: bool,
}

impl Window {
    pub fn new() -> Self {
        Self {
            tabs: Vec::new(),
            active_tab_index: 0,
            window_id: 0,
            title: "Browser Window".to_string(),
            width: 1200,
            height: 800,
            x: 100,
            y: 100,
            is_visible: true,
            is_maximized: false,
            is_minimized: false,
        }
    }

    pub fn with_id(window_id: u32) -> Self {
        Self {
            tabs: Vec::new(),
            active_tab_index: 0,
            window_id,
            title: "Browser Window".to_string(),
            width: 1200,
            height: 800,
            x: 100,
            y: 100,
            is_visible: true,
            is_maximized: false,
            is_minimized: false,
        }
    }

    /// Create a new tab
    pub fn create_tab(&mut self) -> BrowserResult<usize> {
        let tab = Tab::new();
        let tab_index = self.tabs.len();
        self.tabs.push(tab);
        Ok(tab_index)
    }

    /// Close a tab
    pub fn close_tab(&mut self, tab_index: usize) -> BrowserResult<()> {
        if tab_index >= self.tabs.len() {
            return Err(BrowserError::RuntimeError("Invalid tab index".to_string()));
        }

        self.tabs.remove(tab_index);

        // Adjust active tab index if necessary
        if self.active_tab_index >= tab_index && self.active_tab_index > 0 {
            self.active_tab_index -= 1;
        }

        // If no tabs left, create a new one
        if self.tabs.is_empty() {
            self.create_tab()?;
            self.active_tab_index = 0;
        }

        Ok(())
    }

    /// Switch to a specific tab
    pub fn switch_to_tab(&mut self, tab_index: usize) -> BrowserResult<()> {
        if tab_index >= self.tabs.len() {
            return Err(BrowserError::RuntimeError("Invalid tab index".to_string()));
        }

        self.active_tab_index = tab_index;
        Ok(())
    }

    /// Get the active tab
    pub fn get_active_tab(&self) -> Option<&Tab> {
        self.tabs.get(self.active_tab_index)
    }

    /// Get the active tab mutably
    pub fn get_active_tab_mut(&mut self) -> Option<&mut Tab> {
        self.tabs.get_mut(self.active_tab_index)
    }

    /// Get all tabs
    pub fn get_tabs(&self) -> &Vec<Tab> {
        &self.tabs
    }

    /// Get tab count
    pub fn get_tab_count(&self) -> usize {
        self.tabs.len()
    }

    /// Get active tab index
    pub fn get_active_tab_index(&self) -> usize {
        self.active_tab_index
    }

    /// Set window title
    pub fn set_title(&mut self, title: String) {
        self.title = title;
    }

    /// Get window title
    pub fn get_title(&self) -> &String {
        &self.title
    }

    /// Set window size
    pub fn set_size(&mut self, width: u32, height: u32) {
        self.width = width;
        self.height = height;
    }

    /// Get window size
    pub fn get_size(&self) -> (u32, u32) {
        (self.width, self.height)
    }

    /// Set window position
    pub fn set_position(&mut self, x: i32, y: i32) {
        self.x = x;
        self.y = y;
    }

    /// Get window position
    pub fn get_position(&self) -> (i32, i32) {
        (self.x, self.y)
    }

    /// Show the window
    pub fn show(&mut self) {
        self.is_visible = true;
        self.is_minimized = false;
    }

    /// Hide the window
    pub fn hide(&mut self) {
        self.is_visible = false;
    }

    /// Minimize the window
    pub fn minimize(&mut self) {
        self.is_minimized = true;
    }

    /// Maximize the window
    pub fn maximize(&mut self) {
        self.is_maximized = true;
    }

    /// Restore the window
    pub fn restore(&mut self) {
        self.is_maximized = false;
        self.is_minimized = false;
    }

    /// Check if window is visible
    pub fn is_visible(&self) -> bool {
        self.is_visible
    }

    /// Check if window is maximized
    pub fn is_maximized(&self) -> bool {
        self.is_maximized
    }

    /// Check if window is minimized
    pub fn is_minimized(&self) -> bool {
        self.is_minimized
    }

    /// Get window ID
    pub fn get_window_id(&self) -> u32 {
        self.window_id
    }

    /// Navigate active tab to URL
    pub fn navigate(&mut self, url: &str) -> BrowserResult<()> {
        if let Some(tab) = self.get_active_tab_mut() {
            tab.navigate(url)?;
        } else {
            return Err(BrowserError::RuntimeError("No active tab".to_string()));
        }
        Ok(())
    }

    /// Go back in active tab
    pub fn go_back(&mut self) -> BrowserResult<()> {
        if let Some(tab) = self.get_active_tab_mut() {
            tab.go_back()?;
        } else {
            return Err(BrowserError::RuntimeError("No active tab".to_string()));
        }
        Ok(())
    }

    /// Go forward in active tab
    pub fn go_forward(&mut self) -> BrowserResult<()> {
        if let Some(tab) = self.get_active_tab_mut() {
            tab.go_forward()?;
        } else {
            return Err(BrowserError::RuntimeError("No active tab".to_string()));
        }
        Ok(())
    }

    /// Reload active tab
    pub fn reload(&mut self) -> BrowserResult<()> {
        if let Some(tab) = self.get_active_tab_mut() {
            tab.reload()?;
        } else {
            return Err(BrowserError::RuntimeError("No active tab".to_string()));
        }
        Ok(())
    }

    /// Stop loading active tab
    pub fn stop(&mut self) -> BrowserResult<()> {
        if let Some(tab) = self.get_active_tab_mut() {
            tab.stop()?;
        } else {
            return Err(BrowserError::RuntimeError("No active tab".to_string()));
        }
        Ok(())
    }
}

impl Default for Window {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_window_creation() {
        let window = Window::new();
        assert_eq!(window.get_tab_count(), 0);
        assert_eq!(window.get_active_tab_index(), 0);
        assert!(window.is_visible());
        assert!(!window.is_maximized());
        assert!(!window.is_minimized());
    }

    #[test]
    fn test_tab_management() {
        let mut window = Window::new();
        
        // Create tabs
        let tab1 = window.create_tab().unwrap();
        let tab2 = window.create_tab().unwrap();
        
        assert_eq!(window.get_tab_count(), 2);
        assert_eq!(tab1, 0);
        assert_eq!(tab2, 1);
        
        // Switch tabs
        window.switch_to_tab(1).unwrap();
        assert_eq!(window.get_active_tab_index(), 1);
        
        // Close tab
        window.close_tab(0).unwrap();
        assert_eq!(window.get_tab_count(), 1);
        assert_eq!(window.get_active_tab_index(), 0); // Should adjust
    }

    #[test]
    fn test_window_properties() {
        let mut window = Window::new();
        
        window.set_title("Test Window".to_string());
        window.set_size(800, 600);
        window.set_position(50, 50);
        
        assert_eq!(window.get_title(), "Test Window");
        assert_eq!(window.get_size(), (800, 600));
        assert_eq!(window.get_position(), (50, 50));
    }

    #[test]
    fn test_window_state() {
        let mut window = Window::new();
        
        window.minimize();
        assert!(window.is_minimized());
        
        window.maximize();
        assert!(window.is_maximized());
        
        window.restore();
        assert!(!window.is_maximized());
        assert!(!window.is_minimized());
    }
}
