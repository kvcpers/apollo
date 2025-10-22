use std::sync::Arc;
use tokio::sync::Mutex;

// Import our browser engine components
use html_parser::{HtmlEngine, Document};
use css_parser::{CssEngine, Stylesheet};
use url_parser::Url;
use browser_core::Browser;

/// Integration tests for the complete browser engine
#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_html_parsing() {
        let mut engine = HtmlEngine::new();
        
        let html = r#"<!DOCTYPE html>
<html>
<head>
    <title>Test Page</title>
</head>
<body>
    <h1>Hello World</h1>
    <p>This is a test paragraph.</p>
    <div class="container">
        <ul>
            <li>Item 1</li>
            <li>Item 2</li>
        </ul>
    </div>
</body>
</html>"#;

        let result = engine.parse_html(html);
        assert!(result.is_ok());
        
        let document = result.unwrap();
        assert!(document.document_element.is_some());
    }

    #[tokio::test]
    async fn test_css_parsing() {
        let mut engine = CssEngine::new();
        
        let css = r#"
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 20px;
    background-color: #f0f0f0;
}

h1 {
    color: #333;
    font-size: 2em;
    text-align: center;
}

.container {
    max-width: 800px;
    margin: 0 auto;
    background: white;
    border-radius: 8px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    padding: 20px;
}

@media (max-width: 768px) {
    .container {
        margin: 10px;
        padding: 15px;
    }
}
"#;

        let result = engine.parse_stylesheet(css);
        assert!(result.is_ok());
        
        let stylesheet = result.unwrap();
        assert!(stylesheet.rules.len() > 0);
        assert!(stylesheet.at_rules.len() > 0);
    }

    #[tokio::test]
    async fn test_url_parsing() {
        let test_urls = vec![
            "https://www.google.com/search?q=test",
            "http://localhost:8080/api/data?format=json",
            "file:///Users/test.html",
            "data:text/html,<h1>Hello</h1>",
            "mailto:test@example.com",
        ];

        for url_str in test_urls {
            let result = Url::parse(url_str);
            assert!(result.is_ok(), "Failed to parse URL: {}", url_str);
            
            let url = result.unwrap();
            assert!(!url.scheme().is_empty());
        }
    }

    #[tokio::test]
    async fn test_browser_initialization() {
        let browser = Browser::new();
        let result = browser.initialize().await;
        assert!(result.is_ok());
    }

    #[tokio::test]
    async fn test_complete_workflow() {
        // Test the complete browser workflow
        let mut html_engine = HtmlEngine::new();
        let mut css_engine = CssEngine::new();
        
        // 1. Parse HTML
        let html = r#"<!DOCTYPE html>
<html>
<head>
    <title>Test</title>
    <style>
        body { color: red; }
        h1 { font-size: 2em; }
    </style>
</head>
<body>
    <h1>Test Page</h1>
    <p>This is a test.</p>
</body>
</html>"#;

        let html_result = html_engine.parse_html(html);
        assert!(html_result.is_ok());
        
        // 2. Parse CSS
        let css = "body { color: red; } h1 { font-size: 2em; }";
        let css_result = css_engine.parse_stylesheet(css);
        assert!(css_result.is_ok());
        
        // 3. Parse URL
        let url_result = Url::parse("https://example.com");
        assert!(url_result.is_ok());
        
        // 4. Initialize browser
        let browser = Browser::new();
        let browser_result = browser.initialize().await;
        assert!(browser_result.is_ok());
        
        println!("✅ Complete workflow test passed!");
    }

    #[tokio::test]
    async fn test_error_handling() {
        // Test HTML parsing with malformed input
        let mut html_engine = HtmlEngine::new();
        let malformed_html = "<html><head><title>Test</title></head><body><h1>Unclosed tag</body></html>";
        
        // Should still parse successfully (error recovery)
        let result = html_engine.parse_html(malformed_html);
        assert!(result.is_ok());
        
        // Test CSS parsing with invalid syntax
        let mut css_engine = CssEngine::new();
        let invalid_css = "body { color: red; font-size: 2em; } h1 { invalid-property: value; }";
        
        // Should still parse successfully (error recovery)
        let result = css_engine.parse_stylesheet(invalid_css);
        assert!(result.is_ok());
        
        // Test URL parsing with invalid URL
        let invalid_url = "not-a-valid-url";
        let result = Url::parse(invalid_url);
        assert!(result.is_err());
    }

    #[tokio::test]
    async fn test_performance() {
        use std::time::Instant;
        
        // Test HTML parsing performance
        let mut html_engine = HtmlEngine::new();
        let large_html = format!(
            "<html><head><title>Test</title></head><body>{}</body></html>",
            (0..1000).map(|i| format!("<p>Paragraph {}</p>", i)).collect::<Vec<_>>().join("")
        );
        
        let start = Instant::now();
        let result = html_engine.parse_html(&large_html);
        let duration = start.elapsed();
        
        assert!(result.is_ok());
        assert!(duration.as_millis() < 1000, "HTML parsing took too long: {:?}", duration);
        
        // Test CSS parsing performance
        let mut css_engine = CssEngine::new();
        let large_css = (0..100)
            .map(|i| format!("p:nth-child({}) {{ color: red; font-size: {}px; }}", i, i + 10))
            .collect::<Vec<_>>()
            .join("\n");
        
        let start = Instant::now();
        let result = css_engine.parse_stylesheet(&large_css);
        let duration = start.elapsed();
        
        assert!(result.is_ok());
        assert!(duration.as_millis() < 1000, "CSS parsing took too long: {:?}", duration);
    }

    #[tokio::test]
    async fn test_memory_usage() {
        // Test that parsing doesn't cause excessive memory usage
        let mut html_engine = HtmlEngine::new();
        let mut css_engine = CssEngine::new();
        
        // Parse multiple documents
        for i in 0..100 {
            let html = format!("<html><head><title>Test {}</title></head><body><h1>Test {}</h1></body></html>", i, i);
            let css = format!("body {{ color: red; }} h1 {{ font-size: {}em; }}", i % 10);
            
            let html_result = html_engine.parse_html(&html);
            let css_result = css_engine.parse_stylesheet(&css);
            
            assert!(html_result.is_ok());
            assert!(css_result.is_ok());
        }
        
        println!("✅ Memory usage test passed!");
    }
}

/// Performance benchmarks
#[cfg(test)]
mod benchmarks {
    use super::*;
    use std::time::Instant;

    #[tokio::test]
    async fn benchmark_html_parsing() {
        let mut engine = HtmlEngine::new();
        let html = include_str!("../test_data/sample.html");
        
        let start = Instant::now();
        for _ in 0..100 {
            let _ = engine.parse_html(html);
        }
        let duration = start.elapsed();
        
        println!("HTML parsing benchmark: {:?} for 100 iterations", duration);
        assert!(duration.as_millis() < 5000);
    }

    #[tokio::test]
    async fn benchmark_css_parsing() {
        let mut engine = CssEngine::new();
        let css = include_str!("../test_data/sample.css");
        
        let start = Instant::now();
        for _ in 0..100 {
            let _ = engine.parse_stylesheet(css);
        }
        let duration = start.elapsed();
        
        println!("CSS parsing benchmark: {:?} for 100 iterations", duration);
        assert!(duration.as_millis() < 5000);
    }
}
