use css_parser::specificity::Specificity as CssSpecificity;

/// Re-export CSS specificity for convenience
pub type Specificity = CssSpecificity;

/// Specificity utilities for style computation
pub struct SpecificityUtils;

impl SpecificityUtils {
    /// Calculate specificity for a selector
    pub fn calculate_specificity(selector: &css_parser::Selector) -> Specificity {
        match selector {
            css_parser::Selector::Simple(simple_selector) => {
                Self::calculate_simple_selector_specificity(simple_selector)
            }
            css_parser::Selector::Compound(compound_selector) => {
                let mut specificity = Specificity::new(0, 0, 0, 0);
                for simple_selector in compound_selector {
                    specificity =
                        specificity + Self::calculate_simple_selector_specificity(simple_selector);
                }
                specificity
            }
            css_parser::Selector::Complex(complex_selector) => {
                // For complex selectors, sum the specificity of all simple selectors
                let mut specificity = Specificity::new(0, 0, 0, 0);
                for simple_selector in complex_selector {
                    specificity =
                        specificity + Self::calculate_simple_selector_specificity(simple_selector);
                }
                specificity
            }
        }
    }

    /// Calculate specificity for a simple selector
    pub fn calculate_simple_selector_specificity(
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

    /// Compare specificities
    pub fn compare_specificities(spec1: &Specificity, spec2: &Specificity) -> std::cmp::Ordering {
        spec1.cmp(spec2)
    }

    /// Check if one specificity is greater than another
    pub fn is_greater_than(spec1: &Specificity, spec2: &Specificity) -> bool {
        spec1 > spec2
    }

    /// Check if one specificity is less than another
    pub fn is_less_than(spec1: &Specificity, spec2: &Specificity) -> bool {
        spec1 < spec2
    }

    /// Check if two specificities are equal
    pub fn is_equal(spec1: &Specificity, spec2: &Specificity) -> bool {
        spec1 == spec2
    }

    /// Get the highest specificity from a list
    pub fn get_highest_specificity(specificities: &[Specificity]) -> Option<Specificity> {
        specificities.iter().max().copied()
    }

    /// Get the lowest specificity from a list
    pub fn get_lowest_specificity(specificities: &[Specificity]) -> Option<Specificity> {
        specificities.iter().min().copied()
    }

    /// Sort specificities in descending order
    pub fn sort_specificities_descending(mut specificities: Vec<Specificity>) -> Vec<Specificity> {
        specificities.sort_by(|a, b| b.cmp(a));
        specificities
    }

    /// Sort specificities in ascending order
    pub fn sort_specificities_ascending(mut specificities: Vec<Specificity>) -> Vec<Specificity> {
        specificities.sort_by(|a, b| a.cmp(b));
        specificities
    }

    /// Calculate specificity for a CSS rule
    pub fn calculate_rule_specificity(rule: &css_parser::StyleRule) -> Specificity {
        let mut max_specificity = Specificity::new(0, 0, 0, 0);

        for selector in &rule.selectors {
            let specificity = Self::calculate_specificity(selector);
            if specificity > max_specificity {
                max_specificity = specificity;
            }
        }

        max_specificity
    }

    /// Calculate specificity for a declaration
    pub fn calculate_declaration_specificity(declaration: &css_parser::Declaration) -> Specificity {
        // For now, return a default specificity
        // In a real implementation, this would consider the selector's specificity
        Specificity::new(0, 0, 0, 0)
    }

    /// Calculate specificity for an inline style
    pub fn calculate_inline_style_specificity() -> Specificity {
        // Inline styles have the highest specificity (1,0,0,0)
        Specificity::new(1, 0, 0, 0)
    }

    /// Calculate specificity for a user agent style
    pub fn calculate_user_agent_specificity() -> Specificity {
        // User agent styles have the lowest specificity (0,0,0,0)
        Specificity::new(0, 0, 0, 0)
    }

    /// Calculate specificity for a user style
    pub fn calculate_user_specificity() -> Specificity {
        // User styles have low specificity (0,0,0,0)
        Specificity::new(0, 0, 0, 0)
    }

    /// Calculate specificity for an author style
    pub fn calculate_author_specificity(selector: &css_parser::Selector) -> Specificity {
        Self::calculate_specificity(selector)
    }

    /// Calculate specificity for an important declaration
    pub fn calculate_important_specificity(selector: &css_parser::Selector) -> Specificity {
        let mut specificity = Self::calculate_specificity(selector);
        // Important declarations have higher specificity
        specificity.a += 1;
        specificity
    }

    /// Check if a specificity is important
    pub fn is_important_specificity(specificity: &Specificity) -> bool {
        specificity.a > 0
    }

    /// Get the specificity value as a string
    pub fn specificity_to_string(specificity: &Specificity) -> String {
        format!(
            "{},{},{},{}",
            specificity.a, specificity.b, specificity.c, specificity.d
        )
    }

    /// Parse specificity from a string
    pub fn specificity_from_string(s: &str) -> Result<Specificity, String> {
        let parts: Vec<&str> = s.split(',').collect();
        if parts.len() != 4 {
            return Err("Invalid specificity format".to_string());
        }

        let a = parts[0]
            .parse::<u32>()
            .map_err(|_| "Invalid specificity value")?;
        let b = parts[1]
            .parse::<u32>()
            .map_err(|_| "Invalid specificity value")?;
        let c = parts[2]
            .parse::<u32>()
            .map_err(|_| "Invalid specificity value")?;
        let d = parts[3]
            .parse::<u32>()
            .map_err(|_| "Invalid specificity value")?;

        Ok(Specificity::new(a, b, c, d))
    }

    /// Calculate specificity for a CSS selector string
    pub fn calculate_specificity_from_string(selector: &str) -> Result<Specificity, String> {
        // This would require parsing the selector string
        // For now, return a default specificity
        Ok(Specificity::new(0, 0, 0, 0))
    }

    /// Calculate specificity for a CSS rule string
    pub fn calculate_rule_specificity_from_string(rule: &str) -> Result<Specificity, String> {
        // This would require parsing the rule string
        // For now, return a default specificity
        Ok(Specificity::new(0, 0, 0, 0))
    }

    /// Calculate specificity for a CSS declaration string
    pub fn calculate_declaration_specificity_from_string(
        declaration: &str,
    ) -> Result<Specificity, String> {
        // This would require parsing the declaration string
        // For now, return a default specificity
        Ok(Specificity::new(0, 0, 0, 0))
    }

    /// Calculate specificity for a CSS property
    pub fn calculate_property_specificity(property: &css_parser::Property) -> Specificity {
        // For now, return a default specificity
        Specificity::new(0, 0, 0, 0)
    }

    /// Calculate specificity for a CSS value
    pub fn calculate_value_specificity(value: &css_parser::PropertyValue) -> Specificity {
        // For now, return a default specificity
        Specificity::new(0, 0, 0, 0)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use css_parser::{Selector, SimpleSelector};

    #[test]
    fn test_simple_selector_specificity() {
        let selector = SimpleSelector {
            tag_name: Some("div".to_string()),
            id: Some("test".to_string()),
            classes: vec!["container".to_string(), "main".to_string()],
            attributes: vec![],
            pseudo_classes: vec![],
            pseudo_elements: vec![],
        };

        let specificity = SpecificityUtils::calculate_simple_selector_specificity(&selector);
        assert_eq!(specificity.b, 1); // One ID
        assert_eq!(specificity.c, 2); // Two classes
        assert_eq!(specificity.d, 1); // One element
    }

    #[test]
    fn test_selector_specificity() {
        let selector = Selector::Simple(SimpleSelector {
            tag_name: Some("div".to_string()),
            id: Some("test".to_string()),
            classes: vec!["container".to_string()],
            attributes: vec![],
            pseudo_classes: vec![],
            pseudo_elements: vec![],
        });

        let specificity = SpecificityUtils::calculate_specificity(&selector);
        assert_eq!(specificity.b, 1); // One ID
        assert_eq!(specificity.c, 1); // One class
        assert_eq!(specificity.d, 1); // One element
    }

    #[test]
    fn test_specificity_comparison() {
        let spec1 = Specificity::new(0, 1, 0, 0);
        let spec2 = Specificity::new(0, 0, 1, 0);

        assert!(SpecificityUtils::is_greater_than(&spec1, &spec2));
        assert!(SpecificityUtils::is_less_than(&spec2, &spec1));
        assert!(SpecificityUtils::is_equal(&spec1, &spec1));
    }

    #[test]
    fn test_specificity_sorting() {
        let specificities = vec![
            Specificity::new(0, 0, 0, 1),
            Specificity::new(0, 0, 1, 0),
            Specificity::new(0, 1, 0, 0),
            Specificity::new(1, 0, 0, 0),
        ];

        let sorted_desc = SpecificityUtils::sort_specificities_descending(specificities.clone());
        assert_eq!(sorted_desc[0], Specificity::new(1, 0, 0, 0));
        assert_eq!(sorted_desc[3], Specificity::new(0, 0, 0, 1));

        let sorted_asc = SpecificityUtils::sort_specificities_ascending(specificities);
        assert_eq!(sorted_asc[0], Specificity::new(0, 0, 0, 1));
        assert_eq!(sorted_asc[3], Specificity::new(1, 0, 0, 0));
    }

    #[test]
    fn test_inline_style_specificity() {
        let specificity = SpecificityUtils::calculate_inline_style_specificity();
        assert_eq!(specificity.a, 1);
        assert_eq!(specificity.b, 0);
        assert_eq!(specificity.c, 0);
        assert_eq!(specificity.d, 0);
    }

    #[test]
    fn test_user_agent_specificity() {
        let specificity = SpecificityUtils::calculate_user_agent_specificity();
        assert_eq!(specificity.a, 0);
        assert_eq!(specificity.b, 0);
        assert_eq!(specificity.c, 0);
        assert_eq!(specificity.d, 0);
    }

    #[test]
    fn test_important_specificity() {
        let selector = Selector::Simple(SimpleSelector {
            tag_name: Some("div".to_string()),
            id: None,
            classes: vec![],
            attributes: vec![],
            pseudo_classes: vec![],
            pseudo_elements: vec![],
        });

        let specificity = SpecificityUtils::calculate_important_specificity(&selector);
        assert_eq!(specificity.a, 1);
        assert_eq!(specificity.b, 0);
        assert_eq!(specificity.c, 0);
        assert_eq!(specificity.d, 1);
    }

    #[test]
    fn test_specificity_string_conversion() {
        let specificity = Specificity::new(1, 2, 3, 4);
        let string = SpecificityUtils::specificity_to_string(&specificity);
        assert_eq!(string, "1,2,3,4");

        let parsed = SpecificityUtils::specificity_from_string(&string).unwrap();
        assert_eq!(parsed, specificity);
    }
}
