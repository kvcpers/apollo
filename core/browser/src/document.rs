use crate::error::{BrowserError, BrowserResult};
use css_parser::Stylesheet;
use html_parser::{dom::DomTree, dom::Element, dom::Node, Document as HtmlDocument};

/// Browser document that combines HTML and CSS
pub struct Document {
    html_document: HtmlDocument,
    stylesheet: Option<Stylesheet>,
    dom_tree: DomTree,
    url: Option<String>,
    title: Option<String>,
}

impl Document {
    pub fn new() -> Self {
        Self {
            html_document: HtmlDocument::new(),
            stylesheet: None,
            dom_tree: DomTree::new(),
            url: None,
            title: None,
        }
    }

    pub fn from_html(html_document: HtmlDocument) -> Self {
        Self {
            html_document,
            stylesheet: None,
            dom_tree: DomTree::new(),
            url: None,
            title: None,
        }
    }

    pub fn with_stylesheet(mut self, stylesheet: Stylesheet) -> Self {
        self.stylesheet = Some(stylesheet);
        self
    }

    pub fn set_url(&mut self, url: String) {
        self.url = Some(url);
    }

    pub fn get_url(&self) -> Option<&String> {
        self.url.as_ref()
    }

    pub fn set_title(&mut self, title: String) {
        self.title = Some(title);
    }

    pub fn get_title(&self) -> Option<&String> {
        self.title.as_ref()
    }

    pub fn get_html_document(&self) -> &HtmlDocument {
        &self.html_document
    }

    pub fn get_stylesheet(&self) -> Option<&Stylesheet> {
        self.stylesheet.as_ref()
    }

    pub fn get_dom_tree(&self) -> &DomTree {
        &self.dom_tree
    }

    pub fn get_dom_tree_mut(&mut self) -> &mut DomTree {
        &mut self.dom_tree
    }

    /// Find an element by ID
    pub fn get_element_by_id(&self, id: &str) -> Option<&Node> {
        self.dom_tree
            .get_element_by_id(id)
            .and_then(|node_id| self.dom_tree.get_node(node_id))
    }

    /// Find elements by selector
    pub fn query_selector(&self, selector: &str) -> Option<&Node> {
        self.dom_tree
            .query_selector(selector)
            .and_then(|node_id| self.dom_tree.get_node(node_id))
    }

    /// Find all elements matching selector
    pub fn query_selector_all(&self, selector: &str) -> Vec<&Node> {
        let node_ids = self.dom_tree.query_selector_all(selector);
        node_ids
            .iter()
            .filter_map(|&node_id| self.dom_tree.get_node(node_id))
            .collect()
    }

    /// Create a new element
    pub fn create_element(&mut self, tag_name: &str) -> usize {
        let _element = Element::new(tag_name.to_string());
        let node = Node::element(tag_name.to_string());
        self.dom_tree.add_node(node)
    }

    /// Create a text node
    pub fn create_text_node(&mut self, text: &str) -> usize {
        let node = Node::text(text.to_string());
        self.dom_tree.add_node(node)
    }

    /// Append a child to an element
    pub fn append_child(&mut self, parent_id: usize, child_id: usize) -> BrowserResult<()> {
        self.dom_tree
            .append_child(parent_id, child_id)
            .map_err(|e| BrowserError::ParseError(format!("Failed to append child: {}", e)))
    }

    /// Remove a child from an element
    pub fn remove_child(&mut self, parent_id: usize, child_id: usize) -> BrowserResult<()> {
        self.dom_tree
            .remove_child(parent_id, child_id)
            .map_err(|e| BrowserError::ParseError(format!("Failed to remove child: {}", e)))
    }

    /// Set an attribute on an element
    pub fn set_attribute(
        &mut self,
        element_id: usize,
        name: &str,
        value: &str,
    ) -> BrowserResult<()> {
        if let Some(node) = self.dom_tree.get_node_mut(element_id) {
            if let Some(element) = node.as_element_mut() {
                element.set_attribute(name.to_string(), value.to_string());
                Ok(())
            } else {
                Err(BrowserError::ParseError(
                    "Node is not an element".to_string(),
                ))
            }
        } else {
            Err(BrowserError::ParseError("Element not found".to_string()))
        }
    }

    /// Get an attribute from an element
    pub fn get_attribute(&self, element_id: usize, name: &str) -> Option<String> {
        self.dom_tree
            .get_node(element_id)
            .and_then(|node| node.as_element())
            .and_then(|element| element.get_attribute(name))
            .cloned()
    }

    /// Check if document has a stylesheet
    pub fn has_stylesheet(&self) -> bool {
        self.stylesheet.is_some()
    }

    /// Get document body element
    pub fn get_body(&self) -> Option<&Node> {
        if let Some(body_id) = self.html_document.body {
            self.dom_tree.get_node(body_id)
        } else {
            None
        }
    }

    /// Get document head element
    pub fn get_head(&self) -> Option<&Node> {
        if let Some(head_id) = self.html_document.head {
            self.dom_tree.get_node(head_id)
        } else {
            None
        }
    }

    /// Get document root element
    pub fn get_document_element(&self) -> Option<&Node> {
        if let Some(doc_id) = self.html_document.document_element {
            self.dom_tree.get_node(doc_id)
        } else {
            None
        }
    }
}

impl Default for Document {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_document_creation() {
        let document = Document::new();
        assert!(document.get_url().is_none());
        assert!(document.get_title().is_none());
        assert!(!document.has_stylesheet());
    }

    #[test]
    fn test_document_url_and_title() {
        let mut document = Document::new();
        document.set_url("https://example.com".to_string());
        document.set_title("Example Page".to_string());

        assert_eq!(document.get_url(), Some(&"https://example.com".to_string()));
        assert_eq!(document.get_title(), Some(&"Example Page".to_string()));
    }

    #[test]
    fn test_element_creation() {
        let mut document = Document::new();
        let element_id = document.create_element("div");
        let text_id = document.create_text_node("Hello World");

        assert!(document.append_child(element_id, text_id).is_ok());
    }

    #[test]
    fn test_attribute_operations() {
        let mut document = Document::new();
        let element_id = document.create_element("div");

        assert!(document.set_attribute(element_id, "id", "test").is_ok());
        assert_eq!(
            document.get_attribute(element_id, "id"),
            Some("test".to_string())
        );
    }
}
