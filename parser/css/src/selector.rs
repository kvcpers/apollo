// use crate::error::CssResult;
use serde::{Deserialize, Serialize};

/// CSS selector with specificity
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Selector {
    pub parts: Vec<SimpleSelector>,
    pub combinators: Vec<Combinator>,
    pub specificity: crate::specificity::Specificity,
}

/// List of selectors (comma-separated)
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct SelectorList {
    pub selectors: Vec<Selector>,
}

/// Simple CSS selector component
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum SimpleSelector {
    Universal,
    Type(String),  // Element type (div, span, etc.)
    Id(String),    // ID selector (#id)
    Class(String), // Class selector (.class)
    Attribute {
        name: String,
        operator: AttributeOperator,
        value: Option<String>,
    },
    PseudoClass(PseudoClass),
    PseudoElement(PseudoElement),
}

/// Attribute selector operators
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AttributeOperator {
    Exists,         // [attr]
    Equals,         // [attr=value]
    Includes,       // [attr~=value]
    DashMatch,      // [attr|=value]
    PrefixMatch,    // [attr^=value]
    SuffixMatch,    // [attr$=value]
    SubstringMatch, // [attr*=value]
}

/// CSS combinators between selectors
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Combinator {
    Descendant,      // (space) - descendant selector
    Child,           // > - child selector
    AdjacentSibling, // + - adjacent sibling selector
    GeneralSibling,  // ~ - general sibling selector
}

/// Pseudo-class selectors
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum PseudoClass {
    Hover,
    Active,
    Focus,
    Visited,
    Link,
    FirstChild,
    LastChild,
    OnlyChild,
    NthChild(String), // :nth-child(2n+1), :nth-child(odd), etc.
    NthLastChild(String),
    NthOfType(String),
    NthLastOfType(String),
    FirstOfType,
    LastOfType,
    OnlyOfType,
    Empty,
    Root,
    Target,
    Enabled,
    Disabled,
    Checked,
    Indeterminate,
    Valid,
    Invalid,
    Required,
    Optional,
    InRange,
    OutOfRange,
    ReadOnly,
    ReadWrite,
    Custom(String), // Custom pseudo-class
}

/// Pseudo-element selectors
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum PseudoElement {
    Before,
    After,
    FirstLine,
    FirstLetter,
    Selection,
    Custom(String), // Custom pseudo-element
}

impl Selector {
    pub fn new() -> Self {
        Self {
            parts: Vec::new(),
            combinators: Vec::new(),
            specificity: crate::specificity::Specificity::new(0, 0, 0),
        }
    }

    pub fn add_simple_selector(&mut self, selector: SimpleSelector) {
        self.parts.push(selector);
        self.update_specificity();
    }

    pub fn add_combinator(&mut self, combinator: Combinator) {
        self.combinators.push(combinator);
    }

    fn update_specificity(&mut self) {
        let mut a = 0; // IDs
        let mut b = 0; // Classes, attributes, pseudo-classes
        let mut c = 0; // Element types, pseudo-elements

        for part in &self.parts {
            match part {
                SimpleSelector::Id(_) => a += 1,
                SimpleSelector::Class(_)
                | SimpleSelector::Attribute { .. }
                | SimpleSelector::PseudoClass(_) => b += 1,
                SimpleSelector::Type(_) | SimpleSelector::PseudoElement(_) => c += 1,
                SimpleSelector::Universal => {
                    // Universal selector has specificity (0,0,0)
                }
            }
        }

        self.specificity = crate::specificity::Specificity::new(a, b, c);
    }

    /// Check if this selector matches a given element
    pub fn matches_element(
        &self,
        element: &html_parser::Element,
        context: &SelectorMatchContext,
    ) -> bool {
        // Simplified matching - in a real implementation, this would be more complex
        // considering combinators and the full selector chain

        if self.parts.is_empty() {
            return false;
        }

        // Check if the last part matches the element
        let last_part = &self.parts[self.parts.len() - 1];
        self.simple_selector_matches(last_part, element, context)
    }

    fn simple_selector_matches(
        &self,
        selector: &SimpleSelector,
        element: &html_parser::Element,
        context: &SelectorMatchContext,
    ) -> bool {
        match selector {
            SimpleSelector::Universal => true,
            SimpleSelector::Type(tag_name) => {
                element.tag_name.to_lowercase() == tag_name.to_lowercase()
            }
            SimpleSelector::Id(id) => element.get_id() == Some(id),
            SimpleSelector::Class(class) => element.get_class_list().contains(&class.as_str()),
            SimpleSelector::Attribute {
                name,
                operator,
                value,
            } => self.attribute_matches(element, name, operator, value.as_deref()),
            SimpleSelector::PseudoClass(pseudo) => {
                self.pseudo_class_matches(pseudo, element, context)
            }
            SimpleSelector::PseudoElement(_) => {
                // Pseudo-elements are handled separately
                false
            }
        }
    }

    fn attribute_matches(
        &self,
        element: &html_parser::Element,
        name: &str,
        operator: &AttributeOperator,
        value: Option<&str>,
    ) -> bool {
        let element_value = match element.get_attribute(name) {
            Some(val) => val,
            None => return matches!(operator, AttributeOperator::Exists),
        };

        match operator {
            AttributeOperator::Exists => true,
            AttributeOperator::Equals => element_value == value.unwrap_or(""),
            AttributeOperator::Includes => {
                if let Some(val) = value {
                    element_value.split_whitespace().any(|v| v == val)
                } else {
                    false
                }
            }
            AttributeOperator::DashMatch => {
                if let Some(val) = value {
                    element_value == val || element_value.starts_with(&format!("{}-", val))
                } else {
                    false
                }
            }
            AttributeOperator::PrefixMatch => {
                if let Some(val) = value {
                    element_value.starts_with(val)
                } else {
                    false
                }
            }
            AttributeOperator::SuffixMatch => {
                if let Some(val) = value {
                    element_value.ends_with(val)
                } else {
                    false
                }
            }
            AttributeOperator::SubstringMatch => {
                if let Some(val) = value {
                    element_value.contains(val)
                } else {
                    false
                }
            }
        }
    }

    fn pseudo_class_matches(
        &self,
        pseudo: &PseudoClass,
        element: &html_parser::Element,
        context: &SelectorMatchContext,
    ) -> bool {
        match pseudo {
            PseudoClass::Hover => context.is_hovered,
            PseudoClass::Active => context.is_active,
            PseudoClass::Focus => context.is_focused,
            PseudoClass::Visited => context.is_visited,
            PseudoClass::Link => {
                element.tag_name.to_lowercase() == "a" && element.get_attribute("href").is_some()
            }
            PseudoClass::FirstChild => context.is_first_child,
            PseudoClass::LastChild => context.is_last_child,
            PseudoClass::OnlyChild => context.is_first_child && context.is_last_child,
            PseudoClass::Empty => context.has_no_children,
            PseudoClass::Root => element.tag_name.to_lowercase() == "html",
            PseudoClass::Enabled => !context.is_disabled,
            PseudoClass::Disabled => context.is_disabled,
            PseudoClass::Checked => context.is_checked,
            PseudoClass::Valid => context.is_valid,
            PseudoClass::Invalid => !context.is_valid,
            PseudoClass::Required => context.is_required,
            PseudoClass::Optional => !context.is_required,
            PseudoClass::ReadOnly => context.is_read_only,
            PseudoClass::ReadWrite => !context.is_read_only,
            _ => {
                // For complex pseudo-classes like :nth-child, we would need more context
                // about the element's position in the DOM tree
                false
            }
        }
    }
}

impl Default for Selector {
    fn default() -> Self {
        Self::new()
    }
}

impl SelectorList {
    pub fn new() -> Self {
        Self {
            selectors: Vec::new(),
        }
    }

    pub fn add_selector(&mut self, selector: Selector) {
        self.selectors.push(selector);
    }

    /// Check if any selector in the list matches the element
    pub fn matches_element(
        &self,
        element: &html_parser::Element,
        context: &SelectorMatchContext,
    ) -> bool {
        self.selectors
            .iter()
            .any(|selector| selector.matches_element(element, context))
    }
}

impl Default for SelectorList {
    fn default() -> Self {
        Self::new()
    }
}

/// Context for selector matching
#[derive(Debug, Clone, PartialEq)]
pub struct SelectorMatchContext {
    pub is_hovered: bool,
    pub is_active: bool,
    pub is_focused: bool,
    pub is_visited: bool,
    pub is_first_child: bool,
    pub is_last_child: bool,
    pub has_no_children: bool,
    pub is_disabled: bool,
    pub is_checked: bool,
    pub is_valid: bool,
    pub is_required: bool,
    pub is_read_only: bool,
}

impl Default for SelectorMatchContext {
    fn default() -> Self {
        Self {
            is_hovered: false,
            is_active: false,
            is_focused: false,
            is_visited: false,
            is_first_child: false,
            is_last_child: false,
            has_no_children: true,
            is_disabled: false,
            is_checked: false,
            is_valid: true,
            is_required: false,
            is_read_only: false,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_selector_specificity() {
        let mut selector = Selector::new();
        selector.add_simple_selector(SimpleSelector::Type("div".to_string()));
        selector.add_simple_selector(SimpleSelector::Class("container".to_string()));
        selector.add_simple_selector(SimpleSelector::Id("main".to_string()));

        assert_eq!(selector.specificity.a, 1); // ID
        assert_eq!(selector.specificity.b, 1); // Class
        assert_eq!(selector.specificity.c, 1); // Type
    }

    #[test]
    fn test_simple_selector_matching() {
        use crate::dom::Element;

        let mut element = Element::new("div".to_string());
        element.set_attribute("id".to_string(), "test".to_string());
        element.set_attribute("class".to_string(), "container main".to_string());

        let mut selector = Selector::new();
        selector.add_simple_selector(SimpleSelector::Type("div".to_string()));

        let context = SelectorMatchContext::default();
        assert!(selector.matches_element(&element, &context));

        let mut id_selector = Selector::new();
        id_selector.add_simple_selector(SimpleSelector::Id("test".to_string()));
        assert!(id_selector.matches_element(&element, &context));

        let mut class_selector = Selector::new();
        class_selector.add_simple_selector(SimpleSelector::Class("container".to_string()));
        assert!(class_selector.matches_element(&element, &context));
    }

    #[test]
    fn test_attribute_selector_matching() {
        use crate::dom::Element;

        let mut element = Element::new("input".to_string());
        element.set_attribute("type".to_string(), "text".to_string());
        element.set_attribute("data-value".to_string(), "test".to_string());

        let mut equals_selector = Selector::new();
        equals_selector.add_simple_selector(SimpleSelector::Attribute {
            name: "type".to_string(),
            operator: AttributeOperator::Equals,
            value: Some("text".to_string()),
        });

        let context = SelectorMatchContext::default();
        assert!(equals_selector.matches_element(&element, &context));

        let mut includes_selector = Selector::new();
        includes_selector.add_simple_selector(SimpleSelector::Attribute {
            name: "data-value".to_string(),
            operator: AttributeOperator::SubstringMatch,
            value: Some("est".to_string()),
        });
        assert!(includes_selector.matches_element(&element, &context));
    }
}
