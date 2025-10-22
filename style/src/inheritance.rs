use crate::computed_style::ComputedStyle;
use css_parser::properties::{Color, Length, FontWeight, FontStyle, TextAlign, TextDecoration, LineHeight};

/// Inheritance implementation for CSS properties
pub struct Inheritance {
    /// Properties that inherit by default
    inheritable_properties: std::collections::HashSet<String>,
}

impl Inheritance {
    pub fn new() -> Self {
        let mut inheritable_properties = std::collections::HashSet::new();
        
        // Typography properties
        inheritable_properties.insert("font-family".to_string());
        inheritable_properties.insert("font-size".to_string());
        inheritable_properties.insert("font-weight".to_string());
        inheritable_properties.insert("font-style".to_string());
        inheritable_properties.insert("font-variant".to_string());
        inheritable_properties.insert("font-stretch".to_string());
        inheritable_properties.insert("font-size-adjust".to_string());
        inheritable_properties.insert("font-kerning".to_string());
        inheritable_properties.insert("font-language-override".to_string());
        inheritable_properties.insert("font-feature-settings".to_string());
        inheritable_properties.insert("font-variation-settings".to_string());
        
        // Text properties
        inheritable_properties.insert("color".to_string());
        inheritable_properties.insert("text-align".to_string());
        inheritable_properties.insert("text-decoration".to_string());
        inheritable_properties.insert("text-decoration-line".to_string());
        inheritable_properties.insert("text-decoration-style".to_string());
        inheritable_properties.insert("text-decoration-color".to_string());
        inheritable_properties.insert("text-decoration-skip".to_string());
        inheritable_properties.insert("text-decoration-skip-ink".to_string());
        inheritable_properties.insert("text-underline-position".to_string());
        inheritable_properties.insert("text-underline-offset".to_string());
        inheritable_properties.insert("text-decoration-thickness".to_string());
        inheritable_properties.insert("text-indent".to_string());
        inheritable_properties.insert("text-transform".to_string());
        inheritable_properties.insert("text-shadow".to_string());
        inheritable_properties.insert("text-rendering".to_string());
        inheritable_properties.insert("text-orientation".to_string());
        inheritable_properties.insert("text-combine-upright".to_string());
        inheritable_properties.insert("text-emphasis".to_string());
        inheritable_properties.insert("text-emphasis-color".to_string());
        inheritable_properties.insert("text-emphasis-style".to_string());
        inheritable_properties.insert("text-emphasis-position".to_string());
        inheritable_properties.insert("text-underline-position".to_string());
        inheritable_properties.insert("text-underline-offset".to_string());
        inheritable_properties.insert("text-decoration-thickness".to_string());
        
        // Spacing properties
        inheritable_properties.insert("letter-spacing".to_string());
        inheritable_properties.insert("word-spacing".to_string());
        inheritable_properties.insert("line-height".to_string());
        inheritable_properties.insert("white-space".to_string());
        inheritable_properties.insert("word-break".to_string());
        inheritable_properties.insert("word-wrap".to_string());
        inheritable_properties.insert("overflow-wrap".to_string());
        inheritable_properties.insert("hyphens".to_string());
        inheritable_properties.insert("hyphenate-character".to_string());
        inheritable_properties.insert("hyphenate-limit-chars".to_string());
        inheritable_properties.insert("hyphenate-limit-lines".to_string());
        inheritable_properties.insert("hyphenate-limit-zone".to_string());
        inheritable_properties.insert("hyphenate-limit-last".to_string());
        inheritable_properties.insert("hyphenate-limit-zone".to_string());
        
        // List properties
        inheritable_properties.insert("list-style-type".to_string());
        inheritable_properties.insert("list-style-position".to_string());
        inheritable_properties.insert("list-style-image".to_string());
        
        // Table properties
        inheritable_properties.insert("border-collapse".to_string());
        inheritable_properties.insert("border-spacing".to_string());
        inheritable_properties.insert("caption-side".to_string());
        inheritable_properties.insert("empty-cells".to_string());
        inheritable_properties.insert("table-layout".to_string());
        
        // Other properties
        inheritable_properties.insert("cursor".to_string());
        inheritable_properties.insert("visibility".to_string());
        inheritable_properties.insert("direction".to_string());
        inheritable_properties.insert("unicode-bidi".to_string());
        inheritable_properties.insert("quotes".to_string());
        inheritable_properties.insert("counter-reset".to_string());
        inheritable_properties.insert("counter-increment".to_string());
        inheritable_properties.insert("resize".to_string());
        inheritable_properties.insert("text-size-adjust".to_string());
        inheritable_properties.insert("text-overflow".to_string());
        inheritable_properties.insert("text-justify".to_string());
        inheritable_properties.insert("text-align-last".to_string());
        inheritable_properties.insert("text-anchor".to_string());
        inheritable_properties.insert("text-orientation".to_string());
        inheritable_properties.insert("text-combine-upright".to_string());
        inheritable_properties.insert("text-emphasis".to_string());
        inheritable_properties.insert("text-emphasis-color".to_string());
        inheritable_properties.insert("text-emphasis-style".to_string());
        inheritable_properties.insert("text-emphasis-position".to_string());
        inheritable_properties.insert("text-underline-position".to_string());
        inheritable_properties.insert("text-underline-offset".to_string());
        inheritable_properties.insert("text-decoration-thickness".to_string());
        
        Self {
            inheritable_properties,
        }
    }
    
    /// Apply inheritance from parent to child computed style
    pub fn apply_inheritance(&self, child_style: &mut ComputedStyle, parent_style: &ComputedStyle) {
        // Typography properties
        if self.inheritable_properties.contains("font-family") {
            child_style.font_family = parent_style.font_family.clone();
        }
        if self.inheritable_properties.contains("font-size") {
            child_style.font_size = parent_style.font_size;
        }
        if self.inheritable_properties.contains("font-weight") {
            child_style.font_weight = parent_style.font_weight;
        }
        if self.inheritable_properties.contains("font-style") {
            child_style.font_style = parent_style.font_style;
        }
        if self.inheritable_properties.contains("line-height") {
            child_style.line_height = parent_style.line_height;
        }
        if self.inheritable_properties.contains("letter-spacing") {
            child_style.letter_spacing = parent_style.letter_spacing;
        }
        if self.inheritable_properties.contains("word-spacing") {
            child_style.word_spacing = parent_style.word_spacing;
        }
        
        // Text properties
        if self.inheritable_properties.contains("color") {
            child_style.color = parent_style.color;
        }
        if self.inheritable_properties.contains("text-align") {
            child_style.text_align = parent_style.text_align;
        }
        if self.inheritable_properties.contains("text-decoration") {
            child_style.text_decoration = parent_style.text_decoration;
        }
        if self.inheritable_properties.contains("text-indent") {
            child_style.text_indent = parent_style.text_indent;
        }
        if self.inheritable_properties.contains("text-transform") {
            child_style.text_transform = parent_style.text_transform.clone();
        }
        if self.inheritable_properties.contains("white-space") {
            child_style.white_space = parent_style.white_space.clone();
        }
        if self.inheritable_properties.contains("text-shadow") {
            child_style.text_shadow = parent_style.text_shadow.clone();
        }
        
        // Other properties
        if self.inheritable_properties.contains("cursor") {
            child_style.cursor = parent_style.cursor.clone();
        }
        if self.inheritable_properties.contains("visibility") {
            child_style.visibility = parent_style.visibility;
        }
    }
    
    /// Check if a property inherits by default
    pub fn is_inheritable(&self, property_name: &str) -> bool {
        self.inheritable_properties.contains(property_name)
    }
    
    /// Get all inheritable properties
    pub fn get_inheritable_properties(&self) -> &std::collections::HashSet<String> {
        &self.inheritable_properties
    }
    
    /// Add a property to the inheritable set
    pub fn add_inheritable_property(&mut self, property_name: String) {
        self.inheritable_properties.insert(property_name);
    }
    
    /// Remove a property from the inheritable set
    pub fn remove_inheritable_property(&mut self, property_name: &str) {
        self.inheritable_properties.remove(property_name);
    }
    
    /// Apply inheritance for specific properties
    pub fn apply_specific_inheritance(
        &self,
        child_style: &mut ComputedStyle,
        parent_style: &ComputedStyle,
        properties: &[String],
    ) {
        for property_name in properties {
            if self.inheritable_properties.contains(property_name) {
                match property_name.as_str() {
                    "font-family" => {
                        child_style.font_family = parent_style.font_family.clone();
                    }
                    "font-size" => {
                        child_style.font_size = parent_style.font_size;
                    }
                    "font-weight" => {
                        child_style.font_weight = parent_style.font_weight;
                    }
                    "font-style" => {
                        child_style.font_style = parent_style.font_style;
                    }
                    "line-height" => {
                        child_style.line_height = parent_style.line_height;
                    }
                    "letter-spacing" => {
                        child_style.letter_spacing = parent_style.letter_spacing;
                    }
                    "word-spacing" => {
                        child_style.word_spacing = parent_style.word_spacing;
                    }
                    "color" => {
                        child_style.color = parent_style.color;
                    }
                    "text-align" => {
                        child_style.text_align = parent_style.text_align;
                    }
                    "text-decoration" => {
                        child_style.text_decoration = parent_style.text_decoration;
                    }
                    "text-indent" => {
                        child_style.text_indent = parent_style.text_indent;
                    }
                    "text-transform" => {
                        child_style.text_transform = parent_style.text_transform.clone();
                    }
                    "white-space" => {
                        child_style.white_space = parent_style.white_space.clone();
                    }
                    "text-shadow" => {
                        child_style.text_shadow = parent_style.text_shadow.clone();
                    }
                    "cursor" => {
                        child_style.cursor = parent_style.cursor.clone();
                    }
                    "visibility" => {
                        child_style.visibility = parent_style.visibility;
                    }
                    _ => {
                        // Handle other properties as needed
                    }
                }
            }
        }
    }
    
    /// Apply inheritance for computed values (after cascade)
    pub fn apply_computed_inheritance(
        &self,
        child_style: &mut ComputedStyle,
        parent_style: &ComputedStyle,
    ) {
        // Apply inheritance for all inheritable properties
        self.apply_inheritance(child_style, parent_style);
        
        // Apply special inheritance rules
        self.apply_special_inheritance(child_style, parent_style);
    }
    
    /// Apply special inheritance rules
    fn apply_special_inheritance(
        &self,
        child_style: &mut ComputedStyle,
        parent_style: &ComputedStyle,
    ) {
        // Font-size inheritance with relative units
        if matches!(child_style.font_size, Length::Em(_) | Length::Rem(_) | Length::Percentage(_)) {
            // Font-size with relative units inherits the computed value
            child_style.font_size = parent_style.font_size;
        }
        
        // Line-height inheritance with relative units
        if matches!(child_style.line_height, LineHeight::Percentage(_)) {
            // Line-height with percentage inherits the computed value
            child_style.line_height = parent_style.line_height;
        }
        
        // Letter-spacing and word-spacing inheritance
        if matches!(child_style.letter_spacing, Length::Em(_) | Length::Rem(_)) {
            child_style.letter_spacing = parent_style.letter_spacing;
        }
        if matches!(child_style.word_spacing, Length::Em(_) | Length::Rem(_)) {
            child_style.word_spacing = parent_style.word_spacing;
        }
        
        // Text-indent inheritance with relative units
        if matches!(child_style.text_indent, Length::Em(_) | Length::Rem(_) | Length::Percentage(_)) {
            child_style.text_indent = parent_style.text_indent;
        }
    }
    
    /// Check if a property value inherits
    pub fn property_value_inherits(&self, property_name: &str, value: &str) -> bool {
        // Check if the property is inheritable
        if !self.inheritable_properties.contains(property_name) {
            return false;
        }
        
        // Check if the value is 'inherit'
        value == "inherit"
    }
    
    /// Check if a property value is 'initial'
    pub fn property_value_is_initial(&self, property_name: &str, value: &str) -> bool {
        value == "initial"
    }
    
    /// Check if a property value is 'unset'
    pub fn property_value_is_unset(&self, property_name: &str, value: &str) -> bool {
        value == "unset"
    }
    
    /// Check if a property value is 'revert'
    pub fn property_value_is_revert(&self, property_name: &str, value: &str) -> bool {
        value == "revert"
    }
}

impl Default for Inheritance {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use css_parser::properties::{Display, Position, Float, Clear, Visibility};

    #[test]
    fn test_inheritance_creation() {
        let inheritance = Inheritance::new();
        assert!(!inheritance.inheritable_properties.is_empty());
    }

    #[test]
    fn test_inheritable_property_check() {
        let inheritance = Inheritance::new();
        assert!(inheritance.is_inheritable("color"));
        assert!(inheritance.is_inheritable("font-family"));
        assert!(inheritance.is_inheritable("font-size"));
        assert!(!inheritance.is_inheritable("display"));
        assert!(!inheritance.is_inheritable("position"));
    }

    #[test]
    fn test_inheritance_application() {
        let inheritance = Inheritance::new();
        let mut child_style = ComputedStyle::new();
        let mut parent_style = ComputedStyle::new();
        
        parent_style.color = Color::Rgb(255, 0, 0);
        parent_style.font_size = Length::Px(18.0);
        parent_style.font_weight = FontWeight::Bold;
        
        inheritance.apply_inheritance(&mut child_style, &parent_style);
        
        assert_eq!(child_style.color, Color::Rgb(255, 0, 0));
        assert_eq!(child_style.font_size, Length::Px(18.0));
        assert_eq!(child_style.font_weight, FontWeight::Bold);
    }

    #[test]
    fn test_specific_inheritance() {
        let inheritance = Inheritance::new();
        let mut child_style = ComputedStyle::new();
        let mut parent_style = ComputedStyle::new();
        
        parent_style.color = Color::Rgb(255, 0, 0);
        parent_style.font_size = Length::Px(18.0);
        parent_style.display = Display::Block;
        
        inheritance.apply_specific_inheritance(&mut child_style, &parent_style, &["color".to_string()]);
        
        assert_eq!(child_style.color, Color::Rgb(255, 0, 0));
        assert_ne!(child_style.font_size, Length::Px(18.0)); // Should not inherit
        assert_ne!(child_style.display, Display::Block); // Should not inherit
    }

    #[test]
    fn test_property_value_checks() {
        let inheritance = Inheritance::new();
        
        assert!(inheritance.property_value_inherits("color", "inherit"));
        assert!(!inheritance.property_value_inherits("color", "red"));
        assert!(!inheritance.property_value_inherits("display", "inherit"));
        
        assert!(inheritance.property_value_is_initial("color", "initial"));
        assert!(!inheritance.property_value_is_initial("color", "red"));
        
        assert!(inheritance.property_value_is_unset("color", "unset"));
        assert!(!inheritance.property_value_is_unset("color", "red"));
        
        assert!(inheritance.property_value_is_revert("color", "revert"));
        assert!(!inheritance.property_value_is_revert("color", "red"));
    }
}
