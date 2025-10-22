pub mod tokenizer;
pub mod parser;
pub mod dom;
pub mod error;
pub mod utils;

pub use tokenizer::HtmlTokenizer;
pub use parser::HtmlParser;
pub use dom::{Document, Element, Node, NodeType, AttributeMap};
pub use error::{HtmlError, HtmlResult};

use std::collections::HashMap;

/// Main HTML parsing interface
pub struct HtmlEngine {
    parser: HtmlParser,
}

impl HtmlEngine {
    pub fn new() -> Self {
        Self {
            parser: HtmlParser::new(),
        }
    }

    /// Parse HTML string into a DOM document
    pub fn parse_html(&mut self, html: &str) -> HtmlResult<Document> {
        self.parser.parse(html)
    }

    /// Parse HTML fragment (without html/body tags)
    pub fn parse_fragment(&mut self, html: &str, context: &str) -> HtmlResult<Vec<usize>> {
        self.parser.parse_fragment(html, context)
    }

    /// Create a new element with tag name
    pub fn create_element(&self, tag_name: &str) -> Element {
        Element::new(tag_name.to_string())
    }

    /// Create a text node
    pub fn create_text_node(&self, text: &str) -> Node {
        Node::text(text.to_string())
    }

    /// Create a comment node
    pub fn create_comment_node(&self, text: &str) -> Node {
        Node::comment(text.to_string())
    }
}

impl Default for HtmlEngine {
    fn default() -> Self {
        Self::new()
    }
}
