use crate::error::{CssError, CssResult};
use crate::properties::{Property, PropertyDeclaration, PropertyMap, PropertyValue};
use crate::selector::{
    AttributeOperator, Combinator, PseudoClass, PseudoElement, Selector, SelectorList,
    SimpleSelector,
};
use crate::stylesheet::{AtRule, MediaQuery, Rule, Stylesheet};
use crate::tokenizer::{CssTokenType, CssTokenizer};

/// CSS parser implementation
pub struct CssParser {
    tokens: Vec<crate::tokenizer::CssToken>,
    position: usize,
}

impl CssParser {
    pub fn new() -> Self {
        Self {
            tokens: Vec::new(),
            position: 0,
        }
    }

    pub fn parse_stylesheet(&mut self, css: &str) -> CssResult<Stylesheet> {
        let mut tokenizer = CssTokenizer::new(css);
        self.tokens = tokenizer.tokenize()?;
        self.position = 0;

        let mut stylesheet = Stylesheet::new();

        while !self.is_eof() {
            self.consume_whitespace();

            if self.is_eof() {
                break;
            }

            if self.current_token().token_type == CssTokenType::AtKeyword("".to_string()) {
                // Parse at-rule
                if let Some(at_rule) = self.parse_at_rule()? {
                    stylesheet.add_at_rule(at_rule);
                }
            } else {
                // Parse qualified rule
                if let Some(rule) = self.parse_qualified_rule()? {
                    stylesheet.add_rule(rule);
                }
            }
        }

        Ok(stylesheet)
    }

    pub fn parse_rule(&mut self, css: &str) -> CssResult<Rule> {
        let mut tokenizer = CssTokenizer::new(css);
        self.tokens = tokenizer.tokenize()?;
        self.position = 0;

        self.consume_whitespace();

        if let Some(rule) = self.parse_qualified_rule()? {
            Ok(rule)
        } else {
            Err(CssError::ParseError {
                message: "Expected CSS rule".to_string(),
                line: 1,
                column: 1,
                context: css.to_string(),
            })
        }
    }

    pub fn parse_selector(&mut self, selector: &str) -> CssResult<Selector> {
        let mut tokenizer = CssTokenizer::new(selector);
        self.tokens = tokenizer.tokenize()?;
        self.position = 0;

        self.consume_whitespace();

        if let Some(sel) = self.parse_complex_selector()? {
            Ok(sel)
        } else {
            Err(CssError::ParseError {
                message: "Expected CSS selector".to_string(),
                line: 1,
                column: 1,
                context: selector.to_string(),
            })
        }
    }

    pub fn parse_property_value(&mut self, value: &str) -> CssResult<PropertyValue> {
        let mut tokenizer = CssTokenizer::new(value);
        self.tokens = tokenizer.tokenize()?;
        self.position = 0;

        self.consume_whitespace();

        if let Some(val) = self.parse_value()? {
            Ok(val)
        } else {
            Err(CssError::ParseError {
                message: "Expected CSS property value".to_string(),
                line: 1,
                column: 1,
                context: value.to_string(),
            })
        }
    }

    fn is_eof(&self) -> bool {
        self.position >= self.tokens.len()
            || matches!(self.current_token().token_type, CssTokenType::EOF)
    }

    fn current_token(&self) -> &crate::tokenizer::CssToken {
        &self.tokens[self.position]
    }

    fn consume_token(&mut self) {
        if !self.is_eof() {
            self.position += 1;
        }
    }

    fn consume_whitespace(&mut self) {
        while !self.is_eof() && matches!(self.current_token().token_type, CssTokenType::Whitespace)
        {
            self.consume_token();
        }
    }

    fn parse_at_rule(&mut self) -> CssResult<Option<AtRule>> {
        // Simplified at-rule parsing
        if let CssTokenType::AtKeyword(name) = &self.current_token().token_type {
            let rule_name = name.clone();
            self.consume_token();

            // Parse at-rule parameters (simplified)
            let mut params = Vec::new();
            while !self.is_eof()
                && !matches!(
                    self.current_token().token_type,
                    CssTokenType::LeftCurlyBracket | CssTokenType::Semicolon
                )
            {
                params.push(self.current_token().clone());
                self.consume_token();
            }

            // Parse at-rule body
            let mut body = Vec::new();
            if matches!(
                self.current_token().token_type,
                CssTokenType::LeftCurlyBracket
            ) {
                self.consume_token(); // consume '{'
                while !self.is_eof()
                    && !matches!(
                        self.current_token().token_type,
                        CssTokenType::RightCurlyBracket
                    )
                {
                    body.push(self.current_token().clone());
                    self.consume_token();
                }
                if matches!(
                    self.current_token().token_type,
                    CssTokenType::RightCurlyBracket
                ) {
                    self.consume_token(); // consume '}'
                }
            } else if matches!(self.current_token().token_type, CssTokenType::Semicolon) {
                self.consume_token(); // consume ';'
            }

            Ok(Some(AtRule::new(rule_name, params, body)))
        } else {
            Ok(None)
        }
    }

    fn parse_qualified_rule(&mut self) -> CssResult<Option<Rule>> {
        let mut prelude = Vec::new();

        // Parse selector list
        while !self.is_eof()
            && !matches!(
                self.current_token().token_type,
                CssTokenType::LeftCurlyBracket
            )
        {
            prelude.push(self.current_token().clone());
            self.consume_token();
        }

        if prelude.is_empty() {
            return Ok(None);
        }

        // Parse rule body
        if matches!(
            self.current_token().token_type,
            CssTokenType::LeftCurlyBracket
        ) {
            self.consume_token(); // consume '{'

            let mut declarations = Vec::new();
            while !self.is_eof()
                && !matches!(
                    self.current_token().token_type,
                    CssTokenType::RightCurlyBracket
                )
            {
                self.consume_whitespace();

                if let Some(declaration) = self.parse_declaration()? {
                    declarations.push(declaration);
                }

                // Skip semicolon
                if matches!(self.current_token().token_type, CssTokenType::Semicolon) {
                    self.consume_token();
                }
            }

            if matches!(
                self.current_token().token_type,
                CssTokenType::RightCurlyBracket
            ) {
                self.consume_token(); // consume '}'
            }

            // Parse selectors from prelude
            let selector_list = self.parse_selector_list_from_prelude(prelude)?;

            Ok(Some(Rule::new(selector_list, declarations)))
        } else {
            Ok(None)
        }
    }

    fn parse_declaration(&mut self) -> CssResult<Option<PropertyDeclaration>> {
        if let CssTokenType::Ident(property_name) = &self.current_token().token_type {
            let property = Property::from_string(property_name);
            self.consume_token();

            if matches!(self.current_token().token_type, CssTokenType::Colon) {
                self.consume_token(); // consume ':'

                let mut value_tokens = Vec::new();
                let mut important = false;

                // Parse value
                while !self.is_eof()
                    && !matches!(
                        self.current_token().token_type,
                        CssTokenType::Semicolon | CssTokenType::RightCurlyBracket
                    )
                {
                    if matches!(self.current_token().token_type, CssTokenType::Delim('!')) {
                        self.consume_token();
                        if let CssTokenType::Ident(name) = &self.current_token().token_type {
                            if name.to_lowercase() == "important" {
                                important = true;
                                self.consume_token();
                                break;
                            }
                        }
                        value_tokens.push(self.current_token().clone());
                        self.consume_token();
                    } else {
                        value_tokens.push(self.current_token().clone());
                        self.consume_token();
                    }
                }

                let value = self.parse_value_from_tokens(value_tokens)?;
                Ok(Some(PropertyDeclaration::new(property, value, important)))
            } else {
                Ok(None)
            }
        } else {
            Ok(None)
        }
    }

    fn parse_value_from_tokens(
        &self,
        tokens: Vec<crate::tokenizer::CssToken>,
    ) -> CssResult<PropertyValue> {
        if tokens.is_empty() {
            return Err(CssError::ParseError {
                message: "Empty property value".to_string(),
                line: 1,
                column: 1,
                context: "".to_string(),
            });
        }

        // Simplified value parsing - just return the first token as a string for now
        match &tokens[0].token_type {
            CssTokenType::Ident(ident) => Ok(PropertyValue::Identifier(ident.clone())),
            CssTokenType::String(s) => Ok(PropertyValue::String(s.clone())),
            CssTokenType::Number(n) => Ok(PropertyValue::Number(*n)),
            CssTokenType::Percentage(p) => Ok(PropertyValue::Percentage(*p)),
            CssTokenType::Dimension(n, unit) => {
                Ok(PropertyValue::Length(*n, crate::properties::LengthUnit::Px))
                // Simplified
            }
            _ => Ok(PropertyValue::Unknown(tokens[0].token_type.to_string())),
        }
    }

    fn parse_value(&mut self) -> CssResult<Option<PropertyValue>> {
        if self.is_eof() {
            return Ok(None);
        }

        match &self.current_token().token_type {
            CssTokenType::Ident(ident) => {
                let value = PropertyValue::Identifier(ident.clone());
                self.consume_token();
                Ok(Some(value))
            }
            CssTokenType::String(s) => {
                let value = PropertyValue::String(s.clone());
                self.consume_token();
                Ok(Some(value))
            }
            CssTokenType::Number(n) => {
                let value = PropertyValue::Number(*n);
                self.consume_token();
                Ok(Some(value))
            }
            CssTokenType::Percentage(p) => {
                let value = PropertyValue::Percentage(*p);
                self.consume_token();
                Ok(Some(value))
            }
            CssTokenType::Dimension(n, unit) => {
                let value = PropertyValue::Length(*n, crate::properties::LengthUnit::Px); // Simplified
                self.consume_token();
                Ok(Some(value))
            }
            _ => Ok(None),
        }
    }

    fn parse_selector_list_from_prelude(
        &self,
        prelude: Vec<crate::tokenizer::CssToken>,
    ) -> CssResult<SelectorList> {
        // Simplified selector parsing - just create a basic selector for now
        let mut selector_list = SelectorList::new();

        // Find identifiers in the prelude to create simple selectors
        for token in prelude {
            match token.token_type {
                CssTokenType::Ident(name) => {
                    let mut selector = Selector::new();
                    selector.add_simple_selector(SimpleSelector::Type(name));
                    selector_list.add_selector(selector);
                }
                _ => {}
            }
        }

        Ok(selector_list)
    }

    fn parse_complex_selector(&mut self) -> CssResult<Option<Selector>> {
        if self.is_eof() {
            return Ok(None);
        }

        let mut selector = Selector::new();

        // Simplified selector parsing
        while !self.is_eof() {
            match &self.current_token().token_type {
                CssTokenType::Ident(name) => {
                    selector.add_simple_selector(SimpleSelector::Type(name.clone()));
                    self.consume_token();
                }
                CssTokenType::Hash(id) => {
                    selector.add_simple_selector(SimpleSelector::Id(id.clone()));
                    self.consume_token();
                }
                CssTokenType::Delim('.') => {
                    self.consume_token();
                    if let CssTokenType::Ident(class) = &self.current_token().token_type {
                        selector.add_simple_selector(SimpleSelector::Class(class.clone()));
                        self.consume_token();
                    }
                }
                CssTokenType::Delim('>') => {
                    selector.add_combinator(Combinator::Child);
                    self.consume_token();
                }
                CssTokenType::Delim('+') => {
                    selector.add_combinator(Combinator::AdjacentSibling);
                    self.consume_token();
                }
                CssTokenType::Delim('~') => {
                    selector.add_combinator(Combinator::GeneralSibling);
                    self.consume_token();
                }
                CssTokenType::Whitespace => {
                    selector.add_combinator(Combinator::Descendant);
                    self.consume_token();
                }
                _ => break,
            }
        }

        Ok(Some(selector))
    }
}

impl Default for CssParser {
    fn default() -> Self {
        Self::new()
    }
}

// Helper trait for converting token types to strings
impl std::fmt::Display for CssTokenType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            CssTokenType::Ident(name) => write!(f, "{}", name),
            CssTokenType::String(s) => write!(f, "\"{}\"", s),
            CssTokenType::Number(n) => write!(f, "{}", n),
            CssTokenType::Percentage(p) => write!(f, "{}%", p),
            CssTokenType::Dimension(n, unit) => write!(f, "{}{}", n, unit),
            _ => write!(f, "{:?}", self),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_simple_rule_parsing() {
        let mut parser = CssParser::new();
        let css = "div { color: red; }";
        let stylesheet = parser.parse_stylesheet(css).unwrap();

        assert_eq!(stylesheet.rules.len(), 1);
        let rule = &stylesheet.rules[0];
        assert_eq!(rule.declarations.len(), 1);
    }

    #[test]
    fn test_multiple_rules() {
        let mut parser = CssParser::new();
        let css = "div { color: red; } p { font-size: 16px; }";
        let stylesheet = parser.parse_stylesheet(css).unwrap();

        assert_eq!(stylesheet.rules.len(), 2);
    }

    #[test]
    fn test_selector_parsing() {
        let mut parser = CssParser::new();
        let selector = "div.container";
        let parsed_selector = parser.parse_selector(selector).unwrap();

        assert_eq!(parsed_selector.parts.len(), 2);
    }
}
