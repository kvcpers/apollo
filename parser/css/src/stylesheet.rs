use crate::properties::{PropertyDeclaration, PropertyMap};
use crate::selector::SelectorList;
use crate::specificity::Specificity;
use serde::{Deserialize, Serialize};

/// CSS stylesheet
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Stylesheet {
    pub rules: Vec<Rule>,
    pub at_rules: Vec<AtRule>,
    pub imports: Vec<ImportRule>,
}

/// CSS rule
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Rule {
    pub selectors: SelectorList,
    pub declarations: Vec<PropertyDeclaration>,
    pub specificity: Specificity,
}

/// CSS at-rule
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct AtRule {
    pub name: String,
    pub parameters: Vec<crate::tokenizer::CssToken>,
    pub body: Vec<crate::tokenizer::CssToken>,
}

/// CSS import rule
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ImportRule {
    pub url: String,
    pub media_queries: Vec<MediaQuery>,
}

/// CSS media query
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct MediaQuery {
    pub media_type: Option<String>,
    pub conditions: Vec<MediaCondition>,
}

/// CSS media condition
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum MediaCondition {
    Feature(String, Option<String>),
    And,
    Or,
    Not,
    Only,
}

impl Stylesheet {
    pub fn new() -> Self {
        Self {
            rules: Vec::new(),
            at_rules: Vec::new(),
            imports: Vec::new(),
        }
    }

    pub fn add_rule(&mut self, rule: Rule) {
        self.rules.push(rule);
    }

    pub fn add_at_rule(&mut self, at_rule: AtRule) {
        self.at_rules.push(at_rule);
    }

    pub fn add_import(&mut self, import: ImportRule) {
        self.imports.push(import);
    }

    /// Get all rules that match the given element
    pub fn get_matching_rules(
        &self,
        element: &html_parser::Element,
        context: &crate::selector::SelectorMatchContext,
    ) -> Vec<&Rule> {
        let mut matching_rules = Vec::new();

        for rule in &self.rules {
            if rule.selectors.matches_element(element, context) {
                matching_rules.push(rule);
            }
        }

        matching_rules
    }

    /// Get all declarations for a specific property
    pub fn get_declarations_for_property(
        &self,
        property: &crate::properties::Property,
    ) -> Vec<&PropertyDeclaration> {
        let mut declarations = Vec::new();

        for rule in &self.rules {
            for declaration in &rule.declarations {
                if std::mem::discriminant(&declaration.property) == std::mem::discriminant(property)
                {
                    declarations.push(declaration);
                }
            }
        }

        declarations
    }

    /// Compute cascade for a specific property on an element
    pub fn compute_cascade(
        &self,
        element: &html_parser::Element,
        property: &crate::properties::Property,
        context: &crate::selector::SelectorMatchContext,
    ) -> Option<PropertyDeclaration> {
        let mut candidates = Vec::new();

        // Find all matching rules
        for rule in &self.rules {
            if rule.selectors.matches_element(element, context) {
                for declaration in &rule.declarations {
                    if std::mem::discriminant(&declaration.property)
                        == std::mem::discriminant(property)
                    {
                        candidates.push(declaration);
                    }
                }
            }
        }

        if candidates.is_empty() {
            return None;
        }

        // Sort by specificity and source order
        candidates.sort_by(|a, b| {
            // In a real implementation, this would use the cascade context
            // For now, just return the first match
            std::cmp::Ordering::Equal
        });

        Some(candidates[0].clone())
    }
}

impl Default for Stylesheet {
    fn default() -> Self {
        Self::new()
    }
}

impl Rule {
    pub fn new(selectors: SelectorList, declarations: Vec<PropertyDeclaration>) -> Self {
        // Calculate specificity for the rule (highest specificity among selectors)
        let specificity = selectors
            .selectors
            .iter()
            .map(|s| s.specificity)
            .max()
            .unwrap_or_default();

        Self {
            selectors,
            declarations,
            specificity,
        }
    }

    /// Check if this rule matches the given element
    pub fn matches_element(
        &self,
        element: &html_parser::Element,
        context: &crate::selector::SelectorMatchContext,
    ) -> bool {
        self.selectors.matches_element(element, context)
    }

    /// Get all declarations for a specific property
    pub fn get_declarations_for_property(
        &self,
        property: &crate::properties::Property,
    ) -> Vec<&PropertyDeclaration> {
        self.declarations
            .iter()
            .filter(|decl| {
                std::mem::discriminant(&decl.property) == std::mem::discriminant(property)
            })
            .collect()
    }

    /// Convert rule to property map
    pub fn to_property_map(&self) -> PropertyMap {
        let mut property_map = PropertyMap::new();

        for declaration in &self.declarations {
            property_map.set_property(declaration.property.clone(), declaration.value.clone());
        }

        property_map
    }
}

impl AtRule {
    pub fn new(
        name: String,
        parameters: Vec<crate::tokenizer::CssToken>,
        body: Vec<crate::tokenizer::CssToken>,
    ) -> Self {
        Self {
            name,
            parameters,
            body,
        }
    }

    /// Check if this is a media rule
    pub fn is_media_rule(&self) -> bool {
        self.name.to_lowercase() == "media"
    }

    /// Check if this is an import rule
    pub fn is_import_rule(&self) -> bool {
        self.name.to_lowercase() == "import"
    }

    /// Check if this is a font-face rule
    pub fn is_font_face_rule(&self) -> bool {
        self.name.to_lowercase() == "font-face"
    }

    /// Check if this is a keyframes rule
    pub fn is_keyframes_rule(&self) -> bool {
        self.name.to_lowercase() == "keyframes" || self.name.to_lowercase() == "keyframes"
    }

    /// Check if this is a supports rule
    pub fn is_supports_rule(&self) -> bool {
        self.name.to_lowercase() == "supports"
    }

    /// Check if this is a layer rule
    pub fn is_layer_rule(&self) -> bool {
        self.name.to_lowercase() == "layer"
    }

    /// Parse media query from parameters
    pub fn parse_media_query(&self) -> Option<MediaQuery> {
        if !self.is_media_rule() {
            return None;
        }

        // Simplified media query parsing
        let mut media_type = None;
        let mut conditions = Vec::new();

        for token in &self.parameters {
            match &token.token_type {
                crate::tokenizer::CssTokenType::Ident(name) => {
                    if media_type.is_none() {
                        media_type = Some(name.clone());
                    } else {
                        conditions.push(MediaCondition::Feature(name.clone(), None));
                    }
                }
                _ => {}
            }
        }

        Some(MediaQuery {
            media_type,
            conditions,
        })
    }

    /// Parse import URL from parameters
    pub fn parse_import_url(&self) -> Option<String> {
        if !self.is_import_rule() {
            return None;
        }

        for token in &self.parameters {
            match &token.token_type {
                crate::tokenizer::CssTokenType::String(url) => {
                    return Some(url.clone());
                }
                crate::tokenizer::CssTokenType::Url(url) => {
                    return Some(url.clone());
                }
                _ => {}
            }
        }

        None
    }
}

impl MediaQuery {
    pub fn new() -> Self {
        Self {
            media_type: None,
            conditions: Vec::new(),
        }
    }

    pub fn with_media_type(media_type: String) -> Self {
        Self {
            media_type: Some(media_type),
            conditions: Vec::new(),
        }
    }

    /// Check if this media query matches the current media
    pub fn matches(
        &self,
        media_type: &str,
        features: &std::collections::HashMap<String, String>,
    ) -> bool {
        // Check media type
        if let Some(query_media_type) = &self.media_type {
            if query_media_type.to_lowercase() != media_type.to_lowercase()
                && query_media_type != "all"
            {
                return false;
            }
        }

        // Check conditions
        for condition in &self.conditions {
            match condition {
                MediaCondition::Feature(name, value) => {
                    if let Some(actual_value) = features.get(name) {
                        if let Some(expected_value) = value {
                            if actual_value != expected_value {
                                return false;
                            }
                        }
                    } else {
                        return false;
                    }
                }
                _ => {
                    // Handle complex conditions (and, or, not)
                    // Simplified for now
                }
            }
        }

        true
    }
}

impl Default for MediaQuery {
    fn default() -> Self {
        Self::new()
    }
}

/// CSS cascade layer
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CascadeLayer {
    pub name: String,
    pub order: u32,
    pub rules: Vec<Rule>,
}

impl CascadeLayer {
    pub fn new(name: String, order: u32) -> Self {
        Self {
            name,
            order,
            rules: Vec::new(),
        }
    }

    pub fn add_rule(&mut self, rule: Rule) {
        self.rules.push(rule);
    }

    /// Get all rules that match the given element
    pub fn get_matching_rules(
        &self,
        element: &html_parser::Element,
        context: &crate::selector::SelectorMatchContext,
    ) -> Vec<&Rule> {
        let mut matching_rules = Vec::new();

        for rule in &self.rules {
            if rule.selectors.matches_element(element, context) {
                matching_rules.push(rule);
            }
        }

        matching_rules
    }
}

/// CSS container query
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ContainerQuery {
    pub name: Option<String>,
    pub conditions: Vec<ContainerCondition>,
}

/// CSS container condition
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum ContainerCondition {
    Width(ContainerRange),
    Height(ContainerRange),
    InlineSize(ContainerRange),
    BlockSize(ContainerRange),
    AspectRatio(ContainerRange),
    Orientation(ContainerOrientation),
}

/// Container range
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ContainerRange {
    pub min: Option<f32>,
    pub max: Option<f32>,
    pub unit: String,
}

/// Container orientation
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum ContainerOrientation {
    Portrait,
    Landscape,
}

impl ContainerQuery {
    pub fn new() -> Self {
        Self {
            name: None,
            conditions: Vec::new(),
        }
    }

    pub fn with_name(name: String) -> Self {
        Self {
            name: Some(name),
            conditions: Vec::new(),
        }
    }

    /// Check if this container query matches the given container
    pub fn matches(&self, container_size: (f32, f32), container_name: Option<&str>) -> bool {
        // Check container name
        if let Some(query_name) = &self.name {
            if let Some(actual_name) = container_name {
                if query_name != actual_name {
                    return false;
                }
            } else {
                return false;
            }
        }

        // Check conditions
        for condition in &self.conditions {
            match condition {
                ContainerCondition::Width(range) => {
                    if !self.check_range(container_size.0, range) {
                        return false;
                    }
                }
                ContainerCondition::Height(range) => {
                    if !self.check_range(container_size.1, range) {
                        return false;
                    }
                }
                ContainerCondition::InlineSize(range) => {
                    if !self.check_range(container_size.0, range) {
                        return false;
                    }
                }
                ContainerCondition::BlockSize(range) => {
                    if !self.check_range(container_size.1, range) {
                        return false;
                    }
                }
                ContainerCondition::AspectRatio(range) => {
                    let aspect_ratio = container_size.0 / container_size.1;
                    if !self.check_range(aspect_ratio, range) {
                        return false;
                    }
                }
                ContainerCondition::Orientation(orientation) => {
                    let is_landscape = container_size.0 > container_size.1;
                    match orientation {
                        ContainerOrientation::Landscape => {
                            if !is_landscape {
                                return false;
                            }
                        }
                        ContainerOrientation::Portrait => {
                            if is_landscape {
                                return false;
                            }
                        }
                    }
                }
            }
        }

        true
    }

    fn check_range(&self, value: f32, range: &ContainerRange) -> bool {
        if let Some(min) = range.min {
            if value < min {
                return false;
            }
        }
        if let Some(max) = range.max {
            if value > max {
                return false;
            }
        }
        true
    }
}

impl Default for ContainerQuery {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_stylesheet_creation() {
        let stylesheet = Stylesheet::new();
        assert_eq!(stylesheet.rules.len(), 0);
        assert_eq!(stylesheet.at_rules.len(), 0);
    }

    #[test]
    fn test_rule_creation() {
        let selectors = SelectorList::new();
        let declarations = Vec::new();
        let rule = Rule::new(selectors, declarations);

        assert_eq!(rule.declarations.len(), 0);
    }

    #[test]
    fn test_at_rule_creation() {
        let at_rule = AtRule::new("media".to_string(), Vec::new(), Vec::new());

        assert_eq!(at_rule.name, "media");
        assert!(at_rule.is_media_rule());
        assert!(!at_rule.is_import_rule());
    }

    #[test]
    fn test_media_query_matching() {
        let mut media_query = MediaQuery::with_media_type("screen".to_string());

        assert!(media_query.matches("screen", &std::collections::HashMap::new()));
        assert!(!media_query.matches("print", &std::collections::HashMap::new()));
    }

    #[test]
    fn test_container_query_matching() {
        let container_query = ContainerQuery::new();

        assert!(container_query.matches((800.0, 600.0), None));
    }
}
