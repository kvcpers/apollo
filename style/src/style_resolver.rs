use crate::cascade::Cascade;
use crate::computed_style::ComputedStyle;
use crate::inheritance::Inheritance;
use crate::media_queries::MediaQueryMatcher;
use crate::viewport::Viewport;
use css_parser::properties::{Property, PropertyValue};
use css_parser::{Rule, Selector, Specificity, Stylesheet};
use html_parser::dom::{DomTree, Element, Node};
use std::collections::HashMap;

/// Style resolver that computes final styles for DOM elements
pub struct StyleResolver {
    cascade: Cascade,
    inheritance: Inheritance,
    media_matcher: MediaQueryMatcher,
    viewport: Viewport,
    user_agent_styles: Stylesheet,
    user_styles: Stylesheet,
    author_styles: Vec<Stylesheet>,
    computed_styles: HashMap<usize, ComputedStyle>,
}

impl StyleResolver {
    pub fn new() -> Self {
        Self {
            cascade: Cascade::new(),
            inheritance: Inheritance::new(),
            media_matcher: MediaQueryMatcher::new(),
            viewport: Viewport::new(),
            user_agent_styles: Stylesheet::new(),
            user_styles: Stylesheet::new(),
            author_styles: Vec::new(),
            computed_styles: HashMap::new(),
        }
    }

    /// Add a stylesheet to the resolver
    pub fn add_stylesheet(&mut self, stylesheet: Stylesheet, origin: StyleOrigin) {
        match origin {
            StyleOrigin::UserAgent => {
                self.user_agent_styles = stylesheet;
            }
            StyleOrigin::User => {
                self.user_styles = stylesheet;
            }
            StyleOrigin::Author => {
                self.author_styles.push(stylesheet);
            }
        }
    }

    /// Compute styles for all elements in the DOM tree
    pub fn compute_styles(&mut self, dom_tree: &DomTree) -> Result<(), String> {
        self.computed_styles.clear();

        // Start with the root element
        if let Some(root_id) = dom_tree.root_id {
            self.compute_element_styles(dom_tree, root_id, None)?;
        }

        Ok(())
    }

    /// Compute styles for a specific element and its descendants
    fn compute_element_styles(
        &mut self,
        dom_tree: &DomTree,
        element_id: usize,
        parent_style: Option<&ComputedStyle>,
    ) -> Result<ComputedStyle, String> {
        let element = dom_tree.get_node(element_id).ok_or("Element not found")?;

        // Get all applicable rules for this element
        let applicable_rules = self.get_applicable_rules(dom_tree, element_id)?;

        // Apply cascade to get the winning declarations
        let winning_declarations = self.cascade.apply_cascade(applicable_rules)?;

        // Create computed style from declarations
        let mut computed_style = ComputedStyle::new();

        // Apply inherited properties from parent
        if let Some(parent) = parent_style {
            self.inheritance
                .apply_inheritance(&mut computed_style, parent);
        }

        // Apply winning declarations
        self.apply_declarations(&mut computed_style, &winning_declarations)?;

        // Store the computed style
        self.computed_styles
            .insert(element_id, computed_style.clone());

        // Recursively compute styles for children
        for &child_id in &element.children {
            if let Some(child_node) = dom_tree.get_node(child_id) {
                if child_node.is_element() {
                    self.compute_element_styles(dom_tree, child_id, Some(&computed_style))?;
                }
            }
        }

        Ok(computed_style)
    }

    /// Get all applicable rules for an element
    fn get_applicable_rules(
        &self,
        dom_tree: &DomTree,
        element_id: usize,
    ) -> Result<Vec<ApplicableRule>, String> {
        let mut applicable_rules = Vec::new();

        // Get element information
        let element = dom_tree.get_node(element_id).ok_or("Element not found")?;

        if !element.is_element() {
            return Ok(applicable_rules);
        }

        let element_ref = element.as_element().unwrap();

        // Check user agent styles
        self.add_rules_from_stylesheet(
            &self.user_agent_styles,
            element_ref,
            &mut applicable_rules,
            StyleOrigin::UserAgent,
        )?;

        // Check user styles
        self.add_rules_from_stylesheet(
            &self.user_styles,
            element_ref,
            &mut applicable_rules,
            StyleOrigin::User,
        )?;

        // Check author styles
        for stylesheet in &self.author_styles {
            self.add_rules_from_stylesheet(
                stylesheet,
                element_ref,
                &mut applicable_rules,
                StyleOrigin::Author,
            )?;
        }

        // Sort by specificity
        applicable_rules.sort_by(|a, b| a.specificity.cmp(&b.specificity));

        Ok(applicable_rules)
    }

    /// Add matching rules from a stylesheet
    fn add_rules_from_stylesheet(
        &self,
        stylesheet: &Stylesheet,
        element: &Element,
        applicable_rules: &mut Vec<ApplicableRule>,
        origin: StyleOrigin,
    ) -> Result<(), String> {
        for rule in &stylesheet.rules {
            if let Rule::StyleRule(style_rule) = rule {
                // Check if any selector matches
                for selector in &style_rule.selectors {
                    if self.selector_matches(selector, element)? {
                        let specificity = self.calculate_specificity(selector);
                        applicable_rules.push(ApplicableRule {
                            rule: style_rule.clone(),
                            specificity,
                            origin,
                        });
                        break; // Only need one matching selector per rule
                    }
                }
            }
        }
        Ok(())
    }

    /// Check if a selector matches an element
    fn selector_matches(&self, selector: &Selector, element: &Element) -> Result<bool, String> {
        // Simplified selector matching - in a real implementation, this would be much more complex
        match selector {
            Selector::Simple(simple_selector) => {
                self.simple_selector_matches(simple_selector, element)
            }
            Selector::Compound(compound_selector) => {
                // All simple selectors in the compound must match
                for simple_selector in compound_selector {
                    if !self.simple_selector_matches(simple_selector, element)? {
                        return Ok(false);
                    }
                }
                Ok(true)
            }
            Selector::Complex(complex_selector) => {
                // Complex selector matching would require traversing the DOM tree
                // For now, just match the last simple selector
                if let Some(last_selector) = complex_selector.last() {
                    self.simple_selector_matches(last_selector, element)
                } else {
                    Ok(false)
                }
            }
        }
    }

    /// Check if a simple selector matches an element
    fn simple_selector_matches(
        &self,
        selector: &css_parser::SimpleSelector,
        element: &Element,
    ) -> Result<bool, String> {
        // Check tag name
        if let Some(tag_name) = &selector.tag_name {
            if element.tag_name.to_lowercase() != tag_name.to_lowercase() {
                return Ok(false);
            }
        }

        // Check ID
        if let Some(id) = &selector.id {
            if let Some(element_id) = element.get_attribute("id") {
                if element_id != *id {
                    return Ok(false);
                }
            } else {
                return Ok(false);
            }
        }

        // Check classes
        for class in &selector.classes {
            if let Some(element_classes) = element.get_attribute("class") {
                if !element_classes.split_whitespace().any(|c| c == *class) {
                    return Ok(false);
                }
            } else {
                return Ok(false);
            }
        }

        // Check attributes
        for attr_selector in &selector.attributes {
            if let Some(element_attr) = element.get_attribute(&attr_selector.name) {
                match &attr_selector.operator {
                    Some(op) => match op {
                        css_parser::AttributeOperator::Equals => {
                            if element_attr != attr_selector.value {
                                return Ok(false);
                            }
                        }
                        css_parser::AttributeOperator::Includes => {
                            if !element_attr
                                .split_whitespace()
                                .any(|v| v == attr_selector.value)
                            {
                                return Ok(false);
                            }
                        }
                        css_parser::AttributeOperator::DashMatch => {
                            if !element_attr.starts_with(&attr_selector.value)
                                || (!element_attr.len() == attr_selector.value.len()
                                    && !element_attr
                                        .chars()
                                        .nth(attr_selector.value.len())
                                        .unwrap()
                                        .is_whitespace())
                            {
                                return Ok(false);
                            }
                        }
                        css_parser::AttributeOperator::PrefixMatch => {
                            if !element_attr.starts_with(&attr_selector.value) {
                                return Ok(false);
                            }
                        }
                        css_parser::AttributeOperator::SuffixMatch => {
                            if !element_attr.ends_with(&attr_selector.value) {
                                return Ok(false);
                            }
                        }
                        css_parser::AttributeOperator::SubstringMatch => {
                            if !element_attr.contains(&attr_selector.value) {
                                return Ok(false);
                            }
                        }
                    },
                    None => {
                        // Attribute exists check
                        if element_attr.is_empty() {
                            return Ok(false);
                        }
                    }
                }
            } else {
                return Ok(false);
            }
        }

        // Check pseudo-classes (simplified)
        for pseudo_class in &selector.pseudo_classes {
            match pseudo_class.name.as_str() {
                "first-child" => {
                    // Would need to check if this is the first child
                    // For now, just return true
                }
                "last-child" => {
                    // Would need to check if this is the last child
                    // For now, just return true
                }
                "nth-child" => {
                    // Would need to parse and evaluate nth-child expression
                    // For now, just return true
                }
                _ => {
                    // Other pseudo-classes
                }
            }
        }

        Ok(true)
    }

    /// Calculate specificity for a selector
    fn calculate_specificity(&self, selector: &Selector) -> Specificity {
        match selector {
            Selector::Simple(simple_selector) => {
                self.calculate_simple_selector_specificity(simple_selector)
            }
            Selector::Compound(compound_selector) => {
                let mut specificity = Specificity::new(0, 0, 0, 0);
                for simple_selector in compound_selector {
                    specificity =
                        specificity + self.calculate_simple_selector_specificity(simple_selector);
                }
                specificity
            }
            Selector::Complex(complex_selector) => {
                // For complex selectors, sum the specificity of all simple selectors
                let mut specificity = Specificity::new(0, 0, 0, 0);
                for simple_selector in complex_selector {
                    specificity =
                        specificity + self.calculate_simple_selector_specificity(simple_selector);
                }
                specificity
            }
        }
    }

    /// Calculate specificity for a simple selector
    fn calculate_simple_selector_specificity(
        &self,
        selector: &css_parser::SimpleSelector,
    ) -> Specificity {
        let mut a = 0; // Inline styles
        let mut b = 0; // IDs
        let mut c = 0; // Classes, attributes, pseudo-classes
        let mut d = 0; // Elements and pseudo-elements

        // Count IDs
        if selector.id.is_some() {
            b += 1;
        }

        // Count classes
        c += selector.classes.len();

        // Count attributes
        c += selector.attributes.len();

        // Count pseudo-classes
        c += selector.pseudo_classes.len();

        // Count pseudo-elements
        d += selector.pseudo_elements.len();

        // Count element names
        if selector.tag_name.is_some() {
            d += 1;
        }

        Specificity::new(a, b, c, d)
    }

    /// Apply declarations to computed style
    fn apply_declarations(
        &self,
        computed_style: &mut ComputedStyle,
        declarations: &[css_parser::Declaration],
    ) -> Result<(), String> {
        for declaration in declarations {
            self.apply_declaration(computed_style, declaration)?;
        }
        Ok(())
    }

    /// Apply a single declaration to computed style
    fn apply_declaration(
        &self,
        computed_style: &mut ComputedStyle,
        declaration: &css_parser::Declaration,
    ) -> Result<(), String> {
        match &declaration.property {
            Property::Display(value) => {
                computed_style.display = *value;
            }
            Property::Position(value) => {
                computed_style.position = *value;
            }
            Property::Float(value) => {
                computed_style.float = *value;
            }
            Property::Clear(value) => {
                computed_style.clear = *value;
            }
            Property::Visibility(value) => {
                computed_style.visibility = *value;
            }
            Property::ZIndex(value) => {
                computed_style.z_index = *value;
            }
            Property::Width(value) => {
                computed_style.width = *value;
            }
            Property::Height(value) => {
                computed_style.height = *value;
            }
            Property::MinWidth(value) => {
                computed_style.min_width = *value;
            }
            Property::MinHeight(value) => {
                computed_style.min_height = *value;
            }
            Property::MaxWidth(value) => {
                computed_style.max_width = *value;
            }
            Property::MaxHeight(value) => {
                computed_style.max_height = *value;
            }
            Property::MarginTop(value) => {
                computed_style.margin_top = *value;
            }
            Property::MarginRight(value) => {
                computed_style.margin_right = *value;
            }
            Property::MarginBottom(value) => {
                computed_style.margin_bottom = *value;
            }
            Property::MarginLeft(value) => {
                computed_style.margin_left = *value;
            }
            Property::PaddingTop(value) => {
                computed_style.padding_top = *value;
            }
            Property::PaddingRight(value) => {
                computed_style.padding_right = *value;
            }
            Property::PaddingBottom(value) => {
                computed_style.padding_bottom = *value;
            }
            Property::PaddingLeft(value) => {
                computed_style.padding_left = *value;
            }
            Property::BorderTopWidth(value) => {
                computed_style.border_top_width = *value;
            }
            Property::BorderRightWidth(value) => {
                computed_style.border_right_width = *value;
            }
            Property::BorderBottomWidth(value) => {
                computed_style.border_bottom_width = *value;
            }
            Property::BorderLeftWidth(value) => {
                computed_style.border_left_width = *value;
            }
            Property::BorderTopStyle(value) => {
                computed_style.border_top_style = *value;
            }
            Property::BorderRightStyle(value) => {
                computed_style.border_right_style = *value;
            }
            Property::BorderBottomStyle(value) => {
                computed_style.border_bottom_style = *value;
            }
            Property::BorderLeftStyle(value) => {
                computed_style.border_left_style = *value;
            }
            Property::BorderTopColor(value) => {
                computed_style.border_top_color = *value;
            }
            Property::BorderRightColor(value) => {
                computed_style.border_right_color = *value;
            }
            Property::BorderBottomColor(value) => {
                computed_style.border_bottom_color = *value;
            }
            Property::BorderLeftColor(value) => {
                computed_style.border_left_color = *value;
            }
            Property::BackgroundColor(value) => {
                computed_style.background_color = *value;
            }
            Property::BackgroundImage(value) => {
                computed_style.background_image = value.clone();
            }
            Property::BackgroundRepeat(value) => {
                computed_style.background_repeat = *value;
            }
            Property::BackgroundPosition(value) => {
                computed_style.background_position = *value;
            }
            Property::BackgroundSize(value) => {
                computed_style.background_size = *value;
            }
            Property::BackgroundAttachment(value) => {
                computed_style.background_attachment = value.clone();
            }
            Property::FontFamily(value) => {
                computed_style.font_family = value.clone();
            }
            Property::FontSize(value) => {
                computed_style.font_size = *value;
            }
            Property::FontWeight(value) => {
                computed_style.font_weight = *value;
            }
            Property::FontStyle(value) => {
                computed_style.font_style = *value;
            }
            Property::LineHeight(value) => {
                computed_style.line_height = *value;
            }
            Property::LetterSpacing(value) => {
                computed_style.letter_spacing = *value;
            }
            Property::WordSpacing(value) => {
                computed_style.word_spacing = *value;
            }
            Property::Color(value) => {
                computed_style.color = *value;
            }
            Property::TextAlign(value) => {
                computed_style.text_align = *value;
            }
            Property::TextDecoration(value) => {
                computed_style.text_decoration = *value;
            }
            Property::TextIndent(value) => {
                computed_style.text_indent = *value;
            }
            Property::TextTransform(value) => {
                computed_style.text_transform = value.clone();
            }
            Property::WhiteSpace(value) => {
                computed_style.white_space = value.clone();
            }
            Property::Opacity(value) => {
                computed_style.opacity = *value;
            }
            Property::Cursor(value) => {
                computed_style.cursor = value.clone();
            }
            Property::Overflow(value) => {
                computed_style.overflow = value.clone();
            }
            Property::OverflowX(value) => {
                computed_style.overflow_x = value.clone();
            }
            Property::OverflowY(value) => {
                computed_style.overflow_y = value.clone();
            }
            Property::Transform(value) => {
                computed_style.transform = value.clone();
            }
            Property::TransformOrigin(value) => {
                computed_style.transform_origin = value.clone();
            }
            Property::TransformStyle(value) => {
                computed_style.transform_style = value.clone();
            }
            Property::Perspective(value) => {
                computed_style.perspective = *value;
            }
            Property::PerspectiveOrigin(value) => {
                computed_style.perspective_origin = value.clone();
            }
            Property::TransitionProperty(value) => {
                computed_style.transition_property = value.clone();
            }
            Property::TransitionDuration(value) => {
                computed_style.transition_duration = *value;
            }
            Property::TransitionTimingFunction(value) => {
                computed_style.transition_timing_function = value.clone();
            }
            Property::TransitionDelay(value) => {
                computed_style.transition_delay = *value;
            }
            Property::AnimationName(value) => {
                computed_style.animation_name = value.clone();
            }
            Property::AnimationDuration(value) => {
                computed_style.animation_duration = *value;
            }
            Property::AnimationTimingFunction(value) => {
                computed_style.animation_timing_function = value.clone();
            }
            Property::AnimationDelay(value) => {
                computed_style.animation_delay = *value;
            }
            Property::AnimationIterationCount(value) => {
                computed_style.animation_iteration_count = value.clone();
            }
            Property::AnimationDirection(value) => {
                computed_style.animation_direction = value.clone();
            }
            Property::AnimationFillMode(value) => {
                computed_style.animation_fill_mode = value.clone();
            }
            Property::AnimationPlayState(value) => {
                computed_style.animation_play_state = value.clone();
            }
            Property::Clip(value) => {
                computed_style.clip = value.clone();
            }
            Property::ClipPath(value) => {
                computed_style.clip_path = value.clone();
            }
            Property::Filter(value) => {
                computed_style.filter = value.clone();
            }
            Property::BackfaceVisibility(value) => {
                computed_style.backface_visibility = value.clone();
            }
            Property::BoxShadow(value) => {
                computed_style.box_shadow = value.clone();
            }
            Property::TextShadow(value) => {
                computed_style.text_shadow = value.clone();
            }
            Property::FlexDirection(value) => {
                computed_style.flex_direction = value.clone();
            }
            Property::FlexWrap(value) => {
                computed_style.flex_wrap = value.clone();
            }
            Property::FlexGrow(value) => {
                computed_style.flex_grow = *value;
            }
            Property::FlexShrink(value) => {
                computed_style.flex_shrink = *value;
            }
            Property::FlexBasis(value) => {
                computed_style.flex_basis = *value;
            }
            Property::JustifyContent(value) => {
                computed_style.justify_content = value.clone();
            }
            Property::AlignItems(value) => {
                computed_style.align_items = value.clone();
            }
            Property::AlignSelf(value) => {
                computed_style.align_self = value.clone();
            }
            Property::AlignContent(value) => {
                computed_style.align_content = value.clone();
            }
            Property::GridTemplateColumns(value) => {
                computed_style.grid_template_columns = value.clone();
            }
            Property::GridTemplateRows(value) => {
                computed_style.grid_template_rows = value.clone();
            }
            Property::GridTemplateAreas(value) => {
                computed_style.grid_template_areas = value.clone();
            }
            Property::GridAutoColumns(value) => {
                computed_style.grid_auto_columns = value.clone();
            }
            Property::GridAutoRows(value) => {
                computed_style.grid_auto_rows = value.clone();
            }
            Property::GridAutoFlow(value) => {
                computed_style.grid_auto_flow = value.clone();
            }
            Property::GridColumnStart(value) => {
                computed_style.grid_column_start = value.clone();
            }
            Property::GridColumnEnd(value) => {
                computed_style.grid_column_end = value.clone();
            }
            Property::GridRowStart(value) => {
                computed_style.grid_row_start = value.clone();
            }
            Property::GridRowEnd(value) => {
                computed_style.grid_row_end = value.clone();
            }
            Property::GridGap(value) => {
                computed_style.grid_gap = *value;
            }
            Property::GridColumnGap(value) => {
                computed_style.grid_column_gap = *value;
            }
            Property::GridRowGap(value) => {
                computed_style.grid_row_gap = *value;
            }
            _ => {
                // Handle other properties as needed
            }
        }
        Ok(())
    }

    /// Get computed style for an element
    pub fn get_computed_style(&self, element_id: usize) -> Option<&ComputedStyle> {
        self.computed_styles.get(&element_id)
    }

    /// Update viewport information
    pub fn update_viewport(&mut self, width: f32, height: f32) {
        self.viewport.update(width, height);
    }
}

/// Style origin for cascade ordering
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StyleOrigin {
    UserAgent,
    User,
    Author,
}

/// An applicable rule with its specificity and origin
#[derive(Debug, Clone)]
struct ApplicableRule {
    rule: css_parser::StyleRule,
    specificity: Specificity,
    origin: StyleOrigin,
}

impl Default for StyleResolver {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use css_parser::{
        Declaration, Property, PropertyValue, Rule, Selector, SimpleSelector, StyleRule, Stylesheet,
    };
    use html_parser::dom::{DomTree, Element, Node};

    #[test]
    fn test_style_resolver_creation() {
        let resolver = StyleResolver::new();
        assert!(resolver.computed_styles.is_empty());
    }

    #[test]
    fn test_selector_matching() {
        let resolver = StyleResolver::new();
        let mut element = Element::new("div".to_string());
        element.set_attribute("id".to_string(), "test".to_string());
        element.set_attribute("class".to_string(), "container main".to_string());

        let selector = Selector::Simple(SimpleSelector {
            tag_name: Some("div".to_string()),
            id: Some("test".to_string()),
            classes: vec!["container".to_string()],
            attributes: vec![],
            pseudo_classes: vec![],
            pseudo_elements: vec![],
        });

        assert!(resolver.selector_matches(&selector, &element).unwrap());
    }

    #[test]
    fn test_specificity_calculation() {
        let resolver = StyleResolver::new();
        let selector = Selector::Simple(SimpleSelector {
            tag_name: Some("div".to_string()),
            id: Some("test".to_string()),
            classes: vec!["container".to_string(), "main".to_string()],
            attributes: vec![],
            pseudo_classes: vec![],
            pseudo_elements: vec![],
        });

        let specificity = resolver.calculate_specificity(&selector);
        assert_eq!(specificity.b, 1); // One ID
        assert_eq!(specificity.c, 2); // Two classes
        assert_eq!(specificity.d, 1); // One element
    }
}
