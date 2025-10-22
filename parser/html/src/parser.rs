use crate::dom::{Document, DomTree, Element, Node};
use crate::error::{HtmlError, HtmlResult};
use crate::tokenizer::{HtmlTokenizer, TokenType};
use std::collections::HashMap;

/// HTML parser that builds DOM tree from tokens
pub struct HtmlParser {
    tokenizer: HtmlTokenizer,
    dom_tree: DomTree,
    stack: Vec<usize>,                  // Stack of open elements
    foster_parenting_stack: Vec<usize>, // For table elements
    head_pointer: Option<usize>,
    form_pointer: Option<usize>,
}

impl HtmlParser {
    pub fn new() -> Self {
        Self {
            tokenizer: HtmlTokenizer::new(""),
            dom_tree: DomTree::new(),
            stack: Vec::new(),
            foster_parenting_stack: Vec::new(),
            head_pointer: None,
            form_pointer: None,
        }
    }

    pub fn parse(&mut self, html: &str) -> HtmlResult<Document> {
        self.tokenizer = HtmlTokenizer::new(html);
        self.dom_tree = DomTree::new();
        self.stack.clear();
        self.foster_parenting_stack.clear();
        self.head_pointer = None;
        self.form_pointer = None;

        let tokens = self.tokenizer.tokenize()?;
        self.parse_tokens(tokens)?;

        Ok(self.dom_tree.document.clone())
    }

    pub fn parse_fragment(&mut self, html: &str, context: &str) -> HtmlResult<Vec<usize>> {
        // Create a minimal document structure for fragment parsing
        let mut tokens = self.tokenizer.tokenize()?;

        // Insert context element tokens if needed
        let context_tokens = self.create_context_tokens(context);
        tokens.splice(0..0, context_tokens);

        self.parse_tokens(tokens)?;

        // Return the nodes that were added to the context element
        if let Some(context_node_id) = self.stack.last() {
            if let Some(context_node) = self.dom_tree.get_node(*context_node_id) {
                return Ok(context_node.children.clone());
            }
        }

        Ok(vec![])
    }

    fn create_context_tokens(&self, context: &str) -> Vec<crate::tokenizer::Token> {
        // Simplified - create opening and closing tags for context
        let mut tokens = Vec::new();

        tokens.push(crate::tokenizer::Token {
            token_type: TokenType::StartTag {
                name: context.to_string(),
                attributes: vec![],
                self_closing: false,
            },
            line: 1,
            column: 1,
            position: 0,
        });

        tokens.push(crate::tokenizer::Token {
            token_type: TokenType::EndTag(context.to_string()),
            line: 1,
            column: 1,
            position: 0,
        });

        tokens
    }

    fn parse_tokens(&mut self, tokens: Vec<crate::tokenizer::Token>) -> HtmlResult<()> {
        // Create document node
        let document_id = self.dom_tree.add_node(Node::document());
        self.dom_tree.root_id = Some(document_id);
        self.stack.push(document_id);

        for token in tokens {
            self.process_token(token)?;
        }

        Ok(())
    }

    fn process_token(&mut self, token: crate::tokenizer::Token) -> HtmlResult<()> {
        match token.token_type {
            TokenType::StartTag {
                name,
                attributes,
                self_closing,
            } => {
                self.process_start_tag(name, attributes, self_closing)?;
            }
            TokenType::EndTag(name) => {
                self.process_end_tag(name)?;
            }
            TokenType::Text(text) => {
                self.process_text(text)?;
            }
            TokenType::Comment(text) => {
                self.process_comment(text)?;
            }
            TokenType::Doctype(doctype) => {
                self.process_doctype(doctype)?;
            }
            TokenType::Character(ch) => {
                self.process_character(ch)?;
            }
            TokenType::EOF => {
                // End of input - close any remaining open tags
                self.close_remaining_tags()?;
            }
        }
        Ok(())
    }

    fn process_start_tag(
        &mut self,
        name: String,
        attributes: Vec<(String, String)>,
        self_closing: bool,
    ) -> HtmlResult<()> {
        let element = Element::new(name.clone());
        let mut node = Node::element(name.clone());

        // Set attributes
        if let Some(element_ref) = node.as_element_mut() {
            for (attr_name, attr_value) in attributes {
                element_ref.set_attribute(attr_name, attr_value);
            }
        }

        let node_id = self.dom_tree.add_node(node);

        // Insert into appropriate parent
        self.insert_element(node_id)?;

        // Handle special elements
        self.handle_special_elements(node_id, &name)?;

        // Push to stack if not self-closing and not void
        if !self_closing && !Element::is_void_element(&name) {
            self.stack.push(node_id);
        }

        Ok(())
    }

    fn process_end_tag(&mut self, name: String) -> HtmlResult<()> {
        // Find the corresponding opening tag in the stack
        for i in (0..self.stack.len()).rev() {
            if let Some(node) = self.dom_tree.get_node(self.stack[i]) {
                if let Some(element) = node.as_element() {
                    if element.tag_name.to_lowercase() == name.to_lowercase() {
                        // Close all elements up to and including this one
                        self.stack.truncate(i);
                        return Ok(());
                    }
                }
            }
        }

        // If we get here, the end tag doesn't match any open tag
        // This is a parse error, but we'll ignore it for robustness
        Ok(())
    }

    fn process_text(&mut self, text: String) -> HtmlResult<()> {
        // Skip whitespace-only text nodes in certain contexts
        if text.trim().is_empty() && self.should_ignore_whitespace() {
            return Ok(());
        }

        let node = Node::text(text);
        let node_id = self.dom_tree.add_node(node);
        self.insert_element(node_id)?;
        Ok(())
    }

    fn process_comment(&mut self, text: String) -> HtmlResult<()> {
        let node = Node::comment(text);
        let node_id = self.dom_tree.add_node(node);
        self.insert_element(node_id)?;
        Ok(())
    }

    fn process_doctype(&mut self, doctype: String) -> HtmlResult<()> {
        self.dom_tree.document.set_doctype(doctype);
        Ok(())
    }

    fn process_character(&mut self, ch: String) -> HtmlResult<()> {
        let node = Node::text(ch);
        let node_id = self.dom_tree.add_node(node);
        self.insert_element(node_id)?;
        Ok(())
    }

    fn insert_element(&mut self, node_id: usize) -> HtmlResult<()> {
        if let Some(parent_id) = self.stack.last() {
            self.dom_tree.append_child(*parent_id, node_id)?;
        }
        Ok(())
    }

    fn should_ignore_whitespace(&self) -> bool {
        // Check if current parent is a pre, textarea, or script element
        if let Some(parent_id) = self.stack.last() {
            if let Some(parent) = self.dom_tree.get_node(*parent_id) {
                if let Some(element) = parent.as_element() {
                    let tag_name = element.tag_name.to_lowercase();
                    return matches!(tag_name.as_str(), "pre" | "textarea" | "script" | "style");
                }
            }
        }
        true
    }

    fn handle_special_elements(&mut self, node_id: usize, tag_name: &str) -> HtmlResult<()> {
        match tag_name.to_lowercase().as_str() {
            "html" => {
                self.dom_tree.document.set_document_element(node_id);
            }
            "head" => {
                self.dom_tree.document.set_head(node_id);
                self.head_pointer = Some(node_id);
            }
            "body" => {
                self.dom_tree.document.set_body(node_id);
            }
            "form" => {
                self.form_pointer = Some(node_id);
            }
            _ => {}
        }
        Ok(())
    }

    fn close_remaining_tags(&mut self) -> HtmlResult<()> {
        // Close all remaining open tags (except html, head, body)
        let mut to_close = Vec::new();

        for &node_id in &self.stack {
            if let Some(node) = self.dom_tree.get_node(node_id) {
                if let Some(element) = node.as_element() {
                    let tag_name = element.tag_name.to_lowercase();
                    if !matches!(tag_name.as_str(), "html" | "head" | "body") {
                        to_close.push(node_id);
                    }
                }
            }
        }

        for node_id in to_close {
            if let Some(pos) = self.stack.iter().position(|&id| id == node_id) {
                self.stack.remove(pos);
            }
        }

        Ok(())
    }
}

impl Default for HtmlParser {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_simple_html_parsing() {
        let mut parser = HtmlParser::new();
        let html = "<html><head><title>Test</title></head><body><h1>Hello World</h1></body></html>";
        let document = parser.parse(html).unwrap();

        assert!(document.document_element.is_some());
        assert!(document.head.is_some());
        assert!(document.body.is_some());
    }

    #[test]
    fn test_attributes_parsing() {
        let mut parser = HtmlParser::new();
        let html = r#"<html><div id="container" class="main content">Hello</div></html>"#;
        let document = parser.parse(html).unwrap();

        // The parser should successfully parse without errors
        assert!(document.document_element.is_some());
    }

    #[test]
    fn test_comment_parsing() {
        let mut parser = HtmlParser::new();
        let html = "<html><!-- This is a comment --><div>Content</div></html>";
        let document = parser.parse(html).unwrap();

        assert!(document.document_element.is_some());
    }

    #[test]
    fn test_doctype_parsing() {
        let mut parser = HtmlParser::new();
        let html = "<!DOCTYPE html><html><head><title>Test</title></head><body></body></html>";
        let document = parser.parse(html).unwrap();

        assert_eq!(document.doctype, Some("DOCTYPE html".to_string()));
        assert!(document.document_element.is_some());
    }

    #[test]
    fn test_malformed_html() {
        let mut parser = HtmlParser::new();
        let html = "<html><div><p>Unclosed paragraph<div>Another div</div></div></html>";
        let document = parser.parse(html).unwrap();

        // Should handle malformed HTML gracefully
        assert!(document.document_element.is_some());
    }
}
