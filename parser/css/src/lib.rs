pub mod tokenizer;
pub mod parser;
pub mod selector;
pub mod specificity;
pub mod properties;
pub mod stylesheet;
pub mod error;

pub use tokenizer::CssTokenizer;
pub use parser::CssParser;
pub use selector::{Selector, SelectorList, SimpleSelector, Combinator, PseudoClass, PseudoElement};
pub use specificity::Specificity;
pub use properties::{Property, PropertyValue, PropertyDeclaration};
pub use stylesheet::{Stylesheet, Rule, MediaQuery, AtRule};
pub use error::{CssError, CssResult};

/// Main CSS parsing interface
pub struct CssEngine {
    parser: CssParser,
}

impl CssEngine {
    pub fn new() -> Self {
        Self {
            parser: CssParser::new(),
        }
    }

    /// Parse CSS string into a stylesheet
    pub fn parse_stylesheet(&mut self, css: &str) -> CssResult<Stylesheet> {
        self.parser.parse_stylesheet(css)
    }

    /// Parse CSS rule string
    pub fn parse_rule(&mut self, css: &str) -> CssResult<Rule> {
        self.parser.parse_rule(css)
    }

    /// Parse CSS selector string
    pub fn parse_selector(&mut self, selector: &str) -> CssResult<Selector> {
        self.parser.parse_selector(selector)
    }

    /// Parse CSS property value
    pub fn parse_property_value(&mut self, value: &str) -> CssResult<PropertyValue> {
        self.parser.parse_property_value(value)
    }
}

impl Default for CssEngine {
    fn default() -> Self {
        Self::new()
    }
}
