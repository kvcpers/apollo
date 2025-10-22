use std::collections::VecDeque;

/// Navigation controller for managing browser history
pub struct NavigationController {
    history: VecDeque<String>,
    current_index: usize,
    max_history_size: usize,
}

impl NavigationController {
    pub fn new() -> Self {
        Self {
            history: VecDeque::new(),
            current_index: 0,
            max_history_size: 100, // Limit history size
        }
    }

    pub fn with_max_history_size(max_size: usize) -> Self {
        Self {
            history: VecDeque::new(),
            current_index: 0,
            max_history_size: max_size,
        }
    }

    /// Push a new entry to the navigation history
    pub fn push_entry(&mut self, url: String) {
        // Remove any entries after current index (when navigating to new page from middle of history)
        if self.current_index < self.history.len() {
            self.history.truncate(self.current_index + 1);
        }

        // Add new entry
        self.history.push_back(url);
        self.current_index = self.history.len() - 1;

        // Limit history size
        if self.history.len() > self.max_history_size {
            self.history.pop_front();
            self.current_index -= 1;
        }
    }

    /// Go back in history
    pub fn go_back(&mut self) -> Option<String> {
        if self.can_go_back() {
            self.current_index -= 1;
            self.history.get(self.current_index).cloned()
        } else {
            None
        }
    }

    /// Go forward in history
    pub fn go_forward(&mut self) -> Option<String> {
        if self.can_go_forward() {
            self.current_index += 1;
            self.history.get(self.current_index).cloned()
        } else {
            None
        }
    }

    /// Check if can go back
    pub fn can_go_back(&self) -> bool {
        self.current_index > 0
    }

    /// Check if can go forward
    pub fn can_go_forward(&self) -> bool {
        self.current_index < self.history.len() - 1
    }

    /// Get current URL
    pub fn get_current_url(&self) -> Option<&String> {
        self.history.get(self.current_index)
    }

    /// Get navigation history
    pub fn get_history(&self) -> &VecDeque<String> {
        &self.history
    }

    /// Get current index
    pub fn get_current_index(&self) -> usize {
        self.current_index
    }

    /// Clear navigation history
    pub fn clear_history(&mut self) {
        self.history.clear();
        self.current_index = 0;
    }

    /// Get history size
    pub fn get_history_size(&self) -> usize {
        self.history.len()
    }

    /// Set maximum history size
    pub fn set_max_history_size(&mut self, max_size: usize) {
        self.max_history_size = max_size;
        
        // Trim history if it exceeds new max size
        while self.history.len() > self.max_history_size {
            if self.current_index > 0 {
                self.history.pop_front();
                self.current_index -= 1;
            } else {
                self.history.pop_back();
            }
        }
    }

    /// Get maximum history size
    pub fn get_max_history_size(&self) -> usize {
        self.max_history_size
    }

    /// Navigate to a specific index in history
    pub fn navigate_to_index(&mut self, index: usize) -> Option<String> {
        if index < self.history.len() {
            self.current_index = index;
            self.history.get(index).cloned()
        } else {
            None
        }
    }

    /// Get history entries around current position
    pub fn get_history_window(&self, before: usize, after: usize) -> Vec<(usize, &String)> {
        let mut result = Vec::new();
        let start = self.current_index.saturating_sub(before);
        let end = (self.current_index + after + 1).min(self.history.len());
        
        for i in start..end {
            if let Some(url) = self.history.get(i) {
                result.push((i, url));
            }
        }
        
        result
    }
}

impl Default for NavigationController {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_navigation_creation() {
        let nav = NavigationController::new();
        assert_eq!(nav.get_history_size(), 0);
        assert_eq!(nav.get_current_index(), 0);
        assert!(!nav.can_go_back());
        assert!(!nav.can_go_forward());
    }

    #[test]
    fn test_navigation_basic() {
        let mut nav = NavigationController::new();
        
        nav.push_entry("https://example.com".to_string());
        assert_eq!(nav.get_history_size(), 1);
        assert_eq!(nav.get_current_index(), 0);
        assert!(!nav.can_go_back());
        assert!(!nav.can_go_forward());
        assert_eq!(nav.get_current_url(), Some(&"https://example.com".to_string()));
        
        nav.push_entry("https://google.com".to_string());
        assert_eq!(nav.get_history_size(), 2);
        assert_eq!(nav.get_current_index(), 1);
        assert!(nav.can_go_back());
        assert!(!nav.can_go_forward());
    }

    #[test]
    fn test_navigation_back_forward() {
        let mut nav = NavigationController::new();
        
        nav.push_entry("https://example.com".to_string());
        nav.push_entry("https://google.com".to_string());
        nav.push_entry("https://github.com".to_string());
        
        assert_eq!(nav.get_current_url(), Some(&"https://github.com".to_string()));
        
        // Go back
        let url = nav.go_back().unwrap();
        assert_eq!(url, "https://google.com");
        assert_eq!(nav.get_current_url(), Some(&"https://google.com".to_string()));
        assert!(nav.can_go_back());
        assert!(nav.can_go_forward());
        
        // Go back again
        let url = nav.go_back().unwrap();
        assert_eq!(url, "https://example.com");
        assert_eq!(nav.get_current_url(), Some(&"https://example.com".to_string()));
        assert!(!nav.can_go_back());
        assert!(nav.can_go_forward());
        
        // Go forward
        let url = nav.go_forward().unwrap();
        assert_eq!(url, "https://google.com");
        assert_eq!(nav.get_current_url(), Some(&"https://google.com".to_string()));
        assert!(nav.can_go_back());
        assert!(nav.can_go_forward());
    }

    #[test]
    fn test_navigation_history_trimming() {
        let mut nav = NavigationController::new();
        
        // Add entries
        nav.push_entry("https://example.com".to_string());
        nav.push_entry("https://google.com".to_string());
        nav.push_entry("https://github.com".to_string());
        
        // Go back and add new entry (should trim forward history)
        nav.go_back().unwrap();
        nav.push_entry("https://stackoverflow.com".to_string());
        
        assert_eq!(nav.get_history_size(), 3);
        assert_eq!(nav.get_current_url(), Some(&"https://stackoverflow.com".to_string()));
        assert!(nav.can_go_back());
        assert!(!nav.can_go_forward());
    }

    #[test]
    fn test_navigation_max_size() {
        let mut nav = NavigationController::with_max_history_size(3);
        
        nav.push_entry("https://example.com".to_string());
        nav.push_entry("https://google.com".to_string());
        nav.push_entry("https://github.com".to_string());
        nav.push_entry("https://stackoverflow.com".to_string());
        
        assert_eq!(nav.get_history_size(), 3);
        assert_eq!(nav.get_current_url(), Some(&"https://stackoverflow.com".to_string()));
    }

    #[test]
    fn test_navigation_clear() {
        let mut nav = NavigationController::new();
        
        nav.push_entry("https://example.com".to_string());
        nav.push_entry("https://google.com".to_string());
        
        nav.clear_history();
        
        assert_eq!(nav.get_history_size(), 0);
        assert_eq!(nav.get_current_index(), 0);
        assert!(!nav.can_go_back());
        assert!(!nav.can_go_forward());
    }

    #[test]
    fn test_navigation_window() {
        let mut nav = NavigationController::new();
        
        nav.push_entry("https://example.com".to_string());
        nav.push_entry("https://google.com".to_string());
        nav.push_entry("https://github.com".to_string());
        nav.push_entry("https://stackoverflow.com".to_string());
        
        // Go to middle of history
        nav.go_back().unwrap();
        
        let window = nav.get_history_window(1, 1);
        assert_eq!(window.len(), 3);
        assert_eq!(window[0].0, 1);
        assert_eq!(window[1].0, 2);
        assert_eq!(window[2].0, 3);
    }
}
