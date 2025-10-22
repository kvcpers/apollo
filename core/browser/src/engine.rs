use crate::error::{BrowserError, BrowserResult};
use css_parser::CssEngine;
use html_parser::HtmlEngine;

/// Browser engine that coordinates all browser components
pub struct BrowserEngine {
    html_engine: HtmlEngine,
    css_engine: CssEngine,
    is_initialized: bool,
}

impl BrowserEngine {
    pub fn new() -> Self {
        Self {
            html_engine: HtmlEngine::new(),
            css_engine: CssEngine::new(),
            is_initialized: false,
        }
    }

    pub async fn initialize(&mut self) -> BrowserResult<()> {
        if self.is_initialized {
            return Ok(());
        }

        tracing::info!("Initializing browser engine components");

        // Initialize HTML parser
        tracing::debug!("HTML parser initialized");

        // Initialize CSS parser
        tracing::debug!("CSS parser initialized");

        // Initialize other components as needed
        // - JavaScript engine
        // - Layout engine
        // - Rendering engine
        // - Network stack
        // - Storage systems

        self.is_initialized = true;
        tracing::info!("Browser engine initialized successfully");

        Ok(())
    }

    pub fn is_initialized(&self) -> bool {
        self.is_initialized
    }

    /// Parse HTML content
    pub fn parse_html(&mut self, html: &str) -> BrowserResult<html_parser::Document> {
        self.html_engine
            .parse_html(html)
            .map_err(|e| BrowserError::ParseError(format!("HTML parsing failed: {}", e)))
    }

    /// Parse CSS content
    pub fn parse_css(&mut self, css: &str) -> BrowserResult<css_parser::Stylesheet> {
        self.css_engine
            .parse_stylesheet(css)
            .map_err(|e| BrowserError::ParseError(format!("CSS parsing failed: {}", e)))
    }

    /// Process a complete HTML document with CSS
    pub fn process_document(
        &mut self,
        html: &str,
        css: Option<&str>,
    ) -> BrowserResult<ProcessedDocument> {
        let document = self.parse_html(html)?;

        let stylesheet = if let Some(css_content) = css {
            Some(self.parse_css(css_content)?)
        } else {
            None
        };

        Ok(ProcessedDocument {
            document,
            stylesheet,
        })
    }

    /// Get the HTML engine reference
    pub fn html_engine(&self) -> &HtmlEngine {
        &self.html_engine
    }

    /// Get the CSS engine reference
    pub fn css_engine(&self) -> &CssEngine {
        &self.css_engine
    }

    /// Get mutable HTML engine reference
    pub fn html_engine_mut(&mut self) -> &mut HtmlEngine {
        &mut self.html_engine
    }

    /// Get mutable CSS engine reference
    pub fn css_engine_mut(&mut self) -> &mut CssEngine {
        &mut self.css_engine
    }
}

impl Default for BrowserEngine {
    fn default() -> Self {
        Self::new()
    }
}

/// A processed document with both HTML and CSS
#[derive(Debug, Clone)]
pub struct ProcessedDocument {
    pub document: html_parser::Document,
    pub stylesheet: Option<css_parser::Stylesheet>,
}

impl ProcessedDocument {
    pub fn new(
        document: html_parser::Document,
        stylesheet: Option<css_parser::Stylesheet>,
    ) -> Self {
        Self {
            document,
            stylesheet,
        }
    }

    /// Get the HTML document
    pub fn document(&self) -> &html_parser::Document {
        &self.document
    }

    /// Get the CSS stylesheet
    pub fn stylesheet(&self) -> Option<&css_parser::Stylesheet> {
        self.stylesheet.as_ref()
    }

    /// Check if the document has CSS
    pub fn has_stylesheet(&self) -> bool {
        self.stylesheet.is_some()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_browser_engine_creation() {
        let engine = BrowserEngine::new();
        assert!(!engine.is_initialized());
    }

    #[tokio::test]
    async fn test_browser_engine_initialization() {
        let mut engine = BrowserEngine::new();
        engine.initialize().await.unwrap();
        assert!(engine.is_initialized());
    }

    #[tokio::test]
    async fn test_html_parsing() {
        let mut engine = BrowserEngine::new();
        engine.initialize().await.unwrap();

        let html = "<html><head><title>Test</title></head><body><h1>Hello World</h1></body></html>";
        let document = engine.parse_html(html).unwrap();

        assert!(document.document_element.is_some());
    }

    #[tokio::test]
    async fn test_css_parsing() {
        let mut engine = BrowserEngine::new();
        engine.initialize().await.unwrap();

        let css = "body { color: red; } h1 { font-size: 24px; }";
        let stylesheet = engine.parse_css(css).unwrap();

        assert_eq!(stylesheet.rules.len(), 2);
    }

    #[tokio::test]
    async fn test_document_processing() {
        let mut engine = BrowserEngine::new();
        engine.initialize().await.unwrap();

        let html = "<html><body><h1>Hello</h1></body></html>";
        let css = "h1 { color: blue; }";

        let processed = engine.process_document(html, Some(css)).unwrap();

        assert!(processed.has_stylesheet());
        assert!(processed.document().document_element.is_some());
    }
}
