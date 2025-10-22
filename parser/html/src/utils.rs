use crate::dom::{DomTree, Element};
use crate::error::HtmlResult;

/// HTML entity decoder
pub struct EntityDecoder;

impl EntityDecoder {
    /// Decode HTML entities in text
    pub fn decode(text: &str) -> String {
        let mut result = String::new();
        let mut chars = text.chars().peekable();

        while let Some(ch) = chars.next() {
            if ch == '&' {
                if let Some(decoded) = Self::decode_entity(&mut chars) {
                    result.push_str(&decoded);
                } else {
                    result.push('&');
                }
            } else {
                result.push(ch);
            }
        }

        result
    }

    fn decode_entity(chars: &mut std::iter::Peekable<std::str::Chars>) -> Option<String> {
        let mut entity = String::new();

        // Collect characters until we find ';' or invalid character
        while let Some(&ch) = chars.peek() {
            if ch == ';' {
                chars.next(); // consume ';'
                break;
            } else if ch.is_ascii_alphanumeric() || ch == '#' {
                entity.push(chars.next().unwrap());
            } else {
                return None;
            }
        }

        if entity.is_empty() {
            return None;
        }

        // Handle numeric entities
        if let Some(num_str) = entity.strip_prefix('#') {
            if !num_str.is_empty() {
                if let Ok(num) = if num_str.starts_with("x") || num_str.starts_with("X") {
                    u32::from_str_radix(&num_str[1..], 16)
                } else {
                    num_str.parse::<u32>()
                } {
                    if let Some(ch) = std::char::from_u32(num) {
                        return Some(ch.to_string());
                    }
                }
            }
        } else {
            // Handle named entities
            return Self::get_named_entity(&entity);
        }

        None
    }

    fn get_named_entity(entity: &str) -> Option<String> {
        match entity.to_lowercase().as_str() {
            "amp" => Some("&".to_string()),
            "lt" => Some("<".to_string()),
            "gt" => Some(">".to_string()),
            "quot" => Some("\"".to_string()),
            "apos" => Some("'".to_string()),
            "nbsp" => Some("\u{00A0}".to_string()),
            "copy" => Some("©".to_string()),
            "reg" => Some("®".to_string()),
            "trade" => Some("™".to_string()),
            "euro" => Some("€".to_string()),
            "pound" => Some("£".to_string()),
            "yen" => Some("¥".to_string()),
            "cent" => Some("¢".to_string()),
            "sect" => Some("§".to_string()),
            "para" => Some("¶".to_string()),
            "middot" => Some("·".to_string()),
            "hellip" => Some("…".to_string()),
            "ndash" => Some("–".to_string()),
            "mdash" => Some("—".to_string()),
            "lsquo" => Some("'".to_string()),
            "rsquo" => Some("'".to_string()),
            "ldquo" => Some("\u{201C}".to_string()),
            "rdquo" => Some("\u{201D}".to_string()),
            "laquo" => Some("«".to_string()),
            "raquo" => Some("»".to_string()),
            "deg" => Some("°".to_string()),
            "plusmn" => Some("±".to_string()),
            "times" => Some("×".to_string()),
            "divide" => Some("÷".to_string()),
            "frac14" => Some("¼".to_string()),
            "frac12" => Some("½".to_string()),
            "frac34" => Some("¾".to_string()),
            "sup1" => Some("¹".to_string()),
            "sup2" => Some("²".to_string()),
            "sup3" => Some("³".to_string()),
            "alpha" => Some("α".to_string()),
            "beta" => Some("β".to_string()),
            "gamma" => Some("γ".to_string()),
            "delta" => Some("δ".to_string()),
            "epsilon" => Some("ε".to_string()),
            "zeta" => Some("ζ".to_string()),
            "eta" => Some("η".to_string()),
            "theta" => Some("θ".to_string()),
            "iota" => Some("ι".to_string()),
            "kappa" => Some("κ".to_string()),
            "lambda" => Some("λ".to_string()),
            "mu" => Some("μ".to_string()),
            "nu" => Some("ν".to_string()),
            "xi" => Some("ξ".to_string()),
            "omicron" => Some("ο".to_string()),
            "pi" => Some("π".to_string()),
            "rho" => Some("ρ".to_string()),
            "sigma" => Some("σ".to_string()),
            "tau" => Some("τ".to_string()),
            "upsilon" => Some("υ".to_string()),
            "phi" => Some("φ".to_string()),
            "chi" => Some("χ".to_string()),
            "psi" => Some("ψ".to_string()),
            "omega" => Some("ω".to_string()),
            _ => None,
        }
    }
}

/// HTML validator
pub struct HtmlValidator;

impl HtmlValidator {
    /// Validate HTML structure
    pub fn validate(dom_tree: &DomTree) -> HtmlResult<()> {
        // Check for required elements
        if dom_tree.document.document_element.is_none() {
            return Err(crate::error::HtmlError::ParseError {
                message: "Missing document element (html)".to_string(),
                line: 0,
                column: 0,
                context: "validation".to_string(),
            });
        }

        // Validate element nesting rules
        Self::validate_nesting(dom_tree)?;

        Ok(())
    }

    fn validate_nesting(dom_tree: &DomTree) -> HtmlResult<()> {
        for (node_id, node) in dom_tree.nodes.iter().enumerate() {
            if let Some(element) = node.as_element() {
                Self::validate_element_nesting(dom_tree, node_id, element)?;
            }
        }
        Ok(())
    }

    fn validate_element_nesting(
        dom_tree: &DomTree,
        node_id: usize,
        element: &Element,
    ) -> HtmlResult<()> {
        let tag_name = element.tag_name.to_lowercase();

        // Check parent constraints
        if let Some(parent_id) = dom_tree.get_node(node_id).and_then(|node| node.parent) {
            if let Some(parent) = dom_tree.get_node(parent_id) {
                if let Some(parent_element) = parent.as_element() {
                    let parent_tag = parent_element.tag_name.to_lowercase();

                    // Check invalid parent-child relationships
                    match tag_name.as_str() {
                        "head" | "body" => {
                            if parent_tag != "html" {
                                return Err(crate::error::HtmlError::ParseError {
                                    message: format!("{} element must be child of html", tag_name),
                                    line: 0,
                                    column: 0,
                                    context: "validation".to_string(),
                                });
                            }
                        }
                        "title" | "meta" | "link" | "style" | "script" | "base" => {
                            if parent_tag != "head" {
                                return Err(crate::error::HtmlError::ParseError {
                                    message: format!("{} element must be child of head", tag_name),
                                    line: 0,
                                    column: 0,
                                    context: "validation".to_string(),
                                });
                            }
                        }
                        "li" => {
                            if !matches!(parent_tag.as_str(), "ul" | "ol") {
                                return Err(crate::error::HtmlError::ParseError {
                                    message: "li element must be child of ul or ol".to_string(),
                                    line: 0,
                                    column: 0,
                                    context: "validation".to_string(),
                                });
                            }
                        }
                        "dt" | "dd" => {
                            if parent_tag != "dl" {
                                return Err(crate::error::HtmlError::ParseError {
                                    message: format!("{} element must be child of dl", tag_name),
                                    line: 0,
                                    column: 0,
                                    context: "validation".to_string(),
                                });
                            }
                        }
                        "tr" => {
                            if !matches!(parent_tag.as_str(), "table" | "thead" | "tbody" | "tfoot")
                            {
                                return Err(crate::error::HtmlError::ParseError {
                                    message:
                                        "tr element must be child of table, thead, tbody, or tfoot"
                                            .to_string(),
                                    line: 0,
                                    column: 0,
                                    context: "validation".to_string(),
                                });
                            }
                        }
                        "td" | "th" => {
                            if parent_tag != "tr" {
                                return Err(crate::error::HtmlError::ParseError {
                                    message: format!("{} element must be child of tr", tag_name),
                                    line: 0,
                                    column: 0,
                                    context: "validation".to_string(),
                                });
                            }
                        }
                        _ => {}
                    }
                }
            }
        }

        Ok(())
    }
}

/// HTML formatter for pretty printing
pub struct HtmlFormatter {
    indent_size: usize,
    max_line_length: usize,
}

impl HtmlFormatter {
    pub fn new() -> Self {
        Self {
            indent_size: 2,
            max_line_length: 120,
        }
    }

    pub fn with_indent_size(mut self, size: usize) -> Self {
        self.indent_size = size;
        self
    }

    pub fn with_max_line_length(mut self, length: usize) -> Self {
        self.max_line_length = length;
        self
    }

    pub fn format(&self, dom_tree: &DomTree) -> String {
        if let Some(root_id) = dom_tree.root_id {
            self.format_node(dom_tree, root_id, 0)
        } else {
            String::new()
        }
    }

    fn format_node(&self, dom_tree: &DomTree, node_id: usize, depth: usize) -> String {
        let node = match dom_tree.get_node(node_id) {
            Some(node) => node,
            None => return String::new(),
        };

        let indent = " ".repeat(depth * self.indent_size);

        match &node.node_type {
            crate::dom::NodeType::Element(element) => {
                let mut result = String::new();

                // Opening tag
                result.push_str(&indent);
                result.push('<');
                result.push_str(&element.tag_name);

                // Attributes
                for (name, value) in &element.attributes {
                    result.push(' ');
                    result.push_str(name);
                    result.push_str("=\"");
                    result.push_str(&Self::escape_attribute_value(value));
                    result.push('"');
                }

                if element.is_void || element.is_self_closing {
                    result.push_str(" />");
                    return result;
                }

                result.push('>');

                // Children
                if !node.children.is_empty() {
                    result.push('\n');
                    for &child_id in &node.children {
                        result.push_str(&self.format_node(dom_tree, child_id, depth + 1));
                        result.push('\n');
                    }
                    result.push_str(&indent);
                }

                // Closing tag
                result.push_str("</");
                result.push_str(&element.tag_name);
                result.push('>');

                result
            }
            crate::dom::NodeType::Text(text) => {
                let escaped = Self::escape_text(text);
                if escaped.trim().is_empty() {
                    escaped
                } else {
                    format!("{}{}", indent, escaped)
                }
            }
            crate::dom::NodeType::Comment(comment) => {
                format!("{}<!--{}-->", indent, comment)
            }
            crate::dom::NodeType::Document(_) => {
                let mut result = String::new();
                for &child_id in &node.children {
                    result.push_str(&self.format_node(dom_tree, child_id, depth));
                    result.push('\n');
                }
                result
            }
            crate::dom::NodeType::DocumentType(doctype) => {
                format!("{}<!DOCTYPE {}>", indent, doctype)
            }
            crate::dom::NodeType::ProcessingInstruction(target, data) => {
                format!("{}<?{} {}?>", indent, target, data)
            }
        }
    }

    fn escape_text(text: &str) -> String {
        text.chars()
            .map(|c| match c {
                '<' => "&lt;".to_string(),
                '>' => "&gt;".to_string(),
                '&' => "&amp;".to_string(),
                '"' => "&quot;".to_string(),
                '\'' => "&#39;".to_string(),
                _ => c.to_string(),
            })
            .collect()
    }

    fn escape_attribute_value(value: &str) -> String {
        value
            .chars()
            .map(|c| match c {
                '"' => "&quot;".to_string(),
                '&' => "&amp;".to_string(),
                '<' => "&lt;".to_string(),
                '>' => "&gt;".to_string(),
                '\'' => "&#39;".to_string(),
                _ => c.to_string(),
            })
            .collect()
    }
}

impl Default for HtmlFormatter {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_entity_decoding() {
        assert_eq!(EntityDecoder::decode("&amp;"), "&");
        assert_eq!(EntityDecoder::decode("&lt;"), "<");
        assert_eq!(EntityDecoder::decode("&gt;"), ">");
        assert_eq!(EntityDecoder::decode("&quot;"), "\"");
        assert_eq!(EntityDecoder::decode("&apos;"), "'");
        assert_eq!(EntityDecoder::decode("&nbsp;"), "\u{00A0}");
        assert_eq!(EntityDecoder::decode("&copy;"), "©");
        assert_eq!(EntityDecoder::decode("&alpha;"), "α");
        assert_eq!(EntityDecoder::decode("&#65;"), "A");
        assert_eq!(EntityDecoder::decode("&#x41;"), "A");
        assert_eq!(EntityDecoder::decode("Hello &amp; World"), "Hello & World");
    }

    #[test]
    fn test_html_formatting() {
        let mut dom_tree = DomTree::new();
        let html_id = dom_tree.add_node(Node::element("html".to_string()));
        let head_id = dom_tree.add_node(Node::element("head".to_string()));
        let title_id = dom_tree.add_node(Node::element("title".to_string()));
        let title_text_id = dom_tree.add_node(Node::text("Test Page".to_string()));
        let body_id = dom_tree.add_node(Node::element("body".to_string()));
        let div_id = dom_tree.add_node(Node::element("div".to_string()));
        let text_id = dom_tree.add_node(Node::text("Hello World".to_string()));

        dom_tree.append_child(html_id, head_id).unwrap();
        dom_tree.append_child(head_id, title_id).unwrap();
        dom_tree.append_child(title_id, title_text_id).unwrap();
        dom_tree.append_child(html_id, body_id).unwrap();
        dom_tree.append_child(body_id, div_id).unwrap();
        dom_tree.append_child(div_id, text_id).unwrap();

        dom_tree.root_id = Some(html_id);

        let formatter = HtmlFormatter::new();
        let formatted = formatter.format(&dom_tree);

        assert!(formatted.contains("<html>"));
        assert!(formatted.contains("<head>"));
        assert!(formatted.contains("<title>"));
        assert!(formatted.contains("Test Page"));
        assert!(formatted.contains("<body>"));
        assert!(formatted.contains("<div>"));
        assert!(formatted.contains("Hello World"));
    }
}
