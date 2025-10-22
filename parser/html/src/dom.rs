use crate::error::HtmlResult;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

/// DOM Node types
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum NodeType {
    Element(Element),
    Text(String),
    Comment(String),
    Document(Document),
    DocumentType(String),
    ProcessingInstruction(String, String),
}

/// DOM Node structure
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Node {
    pub node_type: NodeType,
    pub parent: Option<usize>,
    pub children: Vec<usize>,
    pub next_sibling: Option<usize>,
    pub previous_sibling: Option<usize>,
}

impl Node {
    pub fn new(node_type: NodeType) -> Self {
        Self {
            node_type,
            parent: None,
            children: Vec::new(),
            next_sibling: None,
            previous_sibling: None,
        }
    }

    pub fn element(tag_name: String) -> Self {
        Self::new(NodeType::Element(Element::new(tag_name)))
    }

    pub fn text(content: String) -> Self {
        Self::new(NodeType::Text(content))
    }

    pub fn comment(content: String) -> Self {
        Self::new(NodeType::Comment(content))
    }

    pub fn document() -> Self {
        Self::new(NodeType::Document(Document::new()))
    }

    pub fn document_type(doctype: String) -> Self {
        Self::new(NodeType::DocumentType(doctype))
    }

    pub fn is_element(&self) -> bool {
        matches!(self.node_type, NodeType::Element(_))
    }

    pub fn is_text(&self) -> bool {
        matches!(self.node_type, NodeType::Text(_))
    }

    pub fn is_comment(&self) -> bool {
        matches!(self.node_type, NodeType::Comment(_))
    }

    pub fn is_document(&self) -> bool {
        matches!(self.node_type, NodeType::Document(_))
    }

    pub fn as_element(&self) -> Option<&Element> {
        match &self.node_type {
            NodeType::Element(elem) => Some(elem),
            _ => None,
        }
    }

    pub fn as_element_mut(&mut self) -> Option<&mut Element> {
        match &mut self.node_type {
            NodeType::Element(elem) => Some(elem),
            _ => None,
        }
    }

    pub fn as_text(&self) -> Option<&String> {
        match &self.node_type {
            NodeType::Text(text) => Some(text),
            _ => None,
        }
    }
}

/// Element structure with attributes and properties
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Element {
    pub tag_name: String,
    pub namespace: Option<String>,
    pub attributes: AttributeMap,
    pub is_void: bool,
    pub is_self_closing: bool,
}

impl Element {
    pub fn new(tag_name: String) -> Self {
        let is_void = Self::is_void_element(&tag_name);
        Self {
            tag_name,
            namespace: None,
            attributes: AttributeMap::new(),
            is_void,
            is_self_closing: false,
        }
    }

    pub fn with_namespace(tag_name: String, namespace: String) -> Self {
        let is_void = Self::is_void_element(&tag_name);
        Self {
            tag_name,
            namespace: Some(namespace),
            attributes: AttributeMap::new(),
            is_void,
            is_self_closing: false,
        }
    }

    /// Check if element is a void element (cannot have children)
    pub fn is_void_element(tag_name: &str) -> bool {
        matches!(
            tag_name.to_lowercase().as_str(),
            "area"
                | "base"
                | "br"
                | "col"
                | "embed"
                | "hr"
                | "img"
                | "input"
                | "link"
                | "meta"
                | "param"
                | "source"
                | "track"
                | "wbr"
        )
    }

    pub fn set_attribute(&mut self, name: String, value: String) {
        self.attributes.insert(name, value);
    }

    pub fn get_attribute(&self, name: &str) -> Option<&String> {
        self.attributes.get(name)
    }

    pub fn remove_attribute(&mut self, name: &str) -> Option<String> {
        self.attributes.remove(name)
    }

    pub fn has_attribute(&self, name: &str) -> bool {
        self.attributes.contains_key(name)
    }

    pub fn get_id(&self) -> Option<&String> {
        self.get_attribute("id")
    }

    pub fn get_class_list(&self) -> Vec<&str> {
        self.get_attribute("class")
            .map(|classes| classes.split_whitespace().collect())
            .unwrap_or_default()
    }

    pub fn matches_selector(&self, selector: &str) -> bool {
        // Simple selector matching - will be enhanced in CSS parser
        match selector.chars().next() {
            Some('#') => {
                let id = &selector[1..];
                self.get_id().is_some_and(|element_id| element_id == id)
            }
            Some('.') => {
                let class = &selector[1..];
                self.get_class_list().contains(&class)
            }
            _ => self.tag_name.to_lowercase() == selector.to_lowercase(),
        }
    }
}

/// Attribute map for elements
pub type AttributeMap = HashMap<String, String>;

/// Document structure
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Document {
    pub doctype: Option<String>,
    pub document_element: Option<usize>, // Root element (html)
    pub head: Option<usize>,
    pub body: Option<usize>,
}

impl Document {
    pub fn new() -> Self {
        Self {
            doctype: None,
            document_element: None,
            head: None,
            body: None,
        }
    }

    pub fn set_doctype(&mut self, doctype: String) {
        self.doctype = Some(doctype);
    }

    pub fn set_document_element(&mut self, element_id: usize) {
        self.document_element = Some(element_id);
    }

    pub fn set_head(&mut self, head_id: usize) {
        self.head = Some(head_id);
    }

    pub fn set_body(&mut self, body_id: usize) {
        self.body = Some(body_id);
    }
}

impl Default for Document {
    fn default() -> Self {
        Self::new()
    }
}

/// DOM Tree for managing nodes
#[derive(Debug, Clone, PartialEq)]
pub struct DomTree {
    pub nodes: Vec<Node>,
    pub document: Document,
    pub root_id: Option<usize>,
}

impl DomTree {
    pub fn new() -> Self {
        Self {
            nodes: Vec::new(),
            document: Document::new(),
            root_id: None,
        }
    }

    pub fn add_node(&mut self, node: Node) -> usize {
        let id = self.nodes.len();
        self.nodes.push(node);
        id
    }

    pub fn get_node(&self, id: usize) -> Option<&Node> {
        self.nodes.get(id)
    }

    pub fn get_node_mut(&mut self, id: usize) -> Option<&mut Node> {
        self.nodes.get_mut(id)
    }

    pub fn append_child(&mut self, parent_id: usize, child_id: usize) -> HtmlResult<()> {
        if parent_id >= self.nodes.len() || child_id >= self.nodes.len() {
            return Err(crate::error::HtmlError::ParseError {
                message: "Invalid node ID".to_string(),
                line: 0,
                column: 0,
                context: "append_child".to_string(),
            });
        }

        // Handle the last child link if there is one
        let last_child_id = {
            let parent = &self.nodes[parent_id];
            parent.children.last().copied()
        };

        if let Some(last_child_id) = last_child_id {
            self.nodes[last_child_id].next_sibling = Some(child_id);
            self.nodes[child_id].previous_sibling = Some(last_child_id);
        }

        // Add child to parent and set parent on child
        self.nodes[parent_id].children.push(child_id);
        self.nodes[child_id].parent = Some(parent_id);

        Ok(())
    }

    pub fn remove_child(&mut self, parent_id: usize, child_id: usize) -> HtmlResult<()> {
        if parent_id >= self.nodes.len() || child_id >= self.nodes.len() {
            return Err(crate::error::HtmlError::ParseError {
                message: "Invalid node ID".to_string(),
                line: 0,
                column: 0,
                context: "remove_child".to_string(),
            });
        }

        // Get sibling information before modifying
        let (prev_sibling, next_sibling) = {
            let child = &self.nodes[child_id];
            (child.previous_sibling, child.next_sibling)
        };

        // Remove from parent's children list
        if let Some(pos) = self.nodes[parent_id]
            .children
            .iter()
            .position(|&id| id == child_id)
        {
            self.nodes[parent_id].children.remove(pos);
        }

        // Update child node
        self.nodes[child_id].parent = None;
        self.nodes[child_id].previous_sibling = None;
        self.nodes[child_id].next_sibling = None;

        // Update sibling links
        if let Some(prev) = prev_sibling {
            self.nodes[prev].next_sibling = next_sibling;
        }
        if let Some(next) = next_sibling {
            self.nodes[next].previous_sibling = prev_sibling;
        }

        Ok(())
    }

    pub fn get_element_by_id(&self, id: &str) -> Option<usize> {
        for (node_id, node) in self.nodes.iter().enumerate() {
            if let Some(element) = node.as_element() {
                if element.get_id().is_some_and(|element_id| element_id == id) {
                    return Some(node_id);
                }
            }
        }
        None
    }

    pub fn query_selector(&self, selector: &str) -> Option<usize> {
        for (node_id, node) in self.nodes.iter().enumerate() {
            if let Some(element) = node.as_element() {
                if element.matches_selector(selector) {
                    return Some(node_id);
                }
            }
        }
        None
    }

    pub fn query_selector_all(&self, selector: &str) -> Vec<usize> {
        let mut results = Vec::new();
        for (node_id, node) in self.nodes.iter().enumerate() {
            if let Some(element) = node.as_element() {
                if element.matches_selector(selector) {
                    results.push(node_id);
                }
            }
        }
        results
    }
}

impl Default for DomTree {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_element_creation() {
        let element = Element::new("div".to_string());
        assert_eq!(element.tag_name, "div");
        assert!(!element.is_void);
        assert!(element.attributes.is_empty());
    }

    #[test]
    fn test_void_element() {
        let img = Element::new("img".to_string());
        assert!(img.is_void);

        let br = Element::new("br".to_string());
        assert!(br.is_void);
    }

    #[test]
    fn test_element_attributes() {
        let mut element = Element::new("div".to_string());
        element.set_attribute("id".to_string(), "test".to_string());
        element.set_attribute("class".to_string(), "container main".to_string());

        assert_eq!(element.get_attribute("id"), Some(&"test".to_string()));
        assert_eq!(element.get_id(), Some(&"test".to_string()));
        assert_eq!(element.get_class_list(), vec!["container", "main"]);
        assert!(element.has_attribute("id"));
        assert!(!element.has_attribute("style"));
    }

    #[test]
    fn test_selector_matching() {
        let mut element = Element::new("div".to_string());
        element.set_attribute("id".to_string(), "test".to_string());
        element.set_attribute("class".to_string(), "container".to_string());

        assert!(element.matches_selector("div"));
        assert!(element.matches_selector("#test"));
        assert!(element.matches_selector(".container"));
        assert!(!element.matches_selector("span"));
        assert!(!element.matches_selector("#other"));
    }

    #[test]
    fn test_dom_tree_operations() {
        let mut tree = DomTree::new();

        let root = tree.add_node(Node::element("html".to_string()));
        let head = tree.add_node(Node::element("head".to_string()));
        let body = tree.add_node(Node::element("body".to_string()));

        tree.append_child(root, head).unwrap();
        tree.append_child(root, body).unwrap();

        let root_node = tree.get_node(root).unwrap();
        assert_eq!(root_node.children.len(), 2);
        assert_eq!(root_node.children[0], head);
        assert_eq!(root_node.children[1], body);

        let head_node = tree.get_node(head).unwrap();
        assert_eq!(head_node.parent, Some(root));
    }
}
