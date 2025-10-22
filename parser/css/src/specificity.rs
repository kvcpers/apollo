use serde::{Deserialize, Serialize};
use std::cmp::Ordering;

/// CSS selector specificity
/// 
/// Specificity is calculated as a 3-tuple (a, b, c) where:
/// - a: number of ID selectors
/// - b: number of class selectors, attribute selectors, and pseudo-class selectors
/// - c: number of element selectors and pseudo-element selectors
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Serialize, Deserialize)]
pub struct Specificity {
    pub a: u32,
    pub b: u32,
    pub c: u32,
}

impl Specificity {
    pub fn new(a: u32, b: u32, c: u32) -> Self {
        Self { a, b, c }
    }

    /// Create specificity for inline styles (highest priority)
    pub fn inline() -> Self {
        Self::new(1, 0, 0) // Inline styles have specificity (1,0,0)
    }

    /// Create specificity for important declarations
    pub fn important() -> Self {
        Self::new(1, 0, 0) // !important has the same specificity as inline styles
    }

    /// Check if this specificity is more specific than another
    pub fn is_more_specific_than(&self, other: &Specificity) -> bool {
        self.cmp(other) == Ordering::Greater
    }

    /// Check if this specificity is less specific than another
    pub fn is_less_specific_than(&self, other: &Specificity) -> bool {
        self.cmp(other) == Ordering::Less
    }

    /// Check if this specificity equals another
    pub fn equals(&self, other: &Specificity) -> bool {
        self.cmp(other) == Ordering::Equal
    }

    /// Add another specificity to this one
    pub fn add(&mut self, other: &Specificity) {
        self.a += other.a;
        self.b += other.b;
        self.c += other.c;
    }

    /// Calculate the numeric value for comparison
    /// This is used for sorting when a, b, c values are equal
    pub fn numeric_value(&self) -> u64 {
        (self.a as u64) * 1_000_000 + (self.b as u64) * 1_000 + (self.c as u64)
    }
}

impl Default for Specificity {
    fn default() -> Self {
        Self::new(0, 0, 0)
    }
}

impl std::fmt::Display for Specificity {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "({}, {}, {})", self.a, self.b, self.c)
    }
}

/// CSS cascade origin
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Serialize, Deserialize)]
pub enum CascadeOrigin {
    UserAgent = 0,    // Browser default styles
    User = 1,         // User stylesheets
    Author = 2,       // Author stylesheets
    AuthorImportant = 3, // Author stylesheets with !important
    UserImportant = 4,   // User stylesheets with !important
}

impl CascadeOrigin {
    /// Get the numeric value for comparison
    pub fn numeric_value(&self) -> u32 {
        *self as u32
    }
}

/// CSS cascade layer
#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Serialize, Deserialize)]
pub struct CascadeLayer {
    pub name: String,
    pub order: u32,
}

impl CascadeLayer {
    pub fn new(name: String, order: u32) -> Self {
        Self { name, order }
    }

    /// Get the numeric value for comparison
    pub fn numeric_value(&self) -> u32 {
        self.order
    }
}

/// CSS cascade context for determining which rule wins
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CascadeContext {
    pub origin: CascadeOrigin,
    pub layer: Option<CascadeLayer>,
    pub specificity: Specificity,
    pub source_order: u32, // Order in which rules appear in the stylesheet
}

impl CascadeContext {
    pub fn new(origin: CascadeOrigin, specificity: Specificity, source_order: u32) -> Self {
        Self {
            origin,
            layer: None,
            specificity,
            source_order,
        }
    }

    pub fn with_layer(mut self, layer: CascadeLayer) -> Self {
        self.layer = Some(layer);
        self
    }

    /// Compare this context with another to determine which rule wins
    /// Returns true if this context wins, false if the other wins
    pub fn wins_over(&self, other: &CascadeContext) -> bool {
        // 1. Origin comparison
        match self.origin.cmp(&other.origin) {
            Ordering::Greater => return true,
            Ordering::Less => return false,
            Ordering::Equal => {}
        }

        // 2. Layer comparison (if both have layers)
        if let (Some(self_layer), Some(other_layer)) = (&self.layer, &other.layer) {
            match self_layer.cmp(other_layer) {
                Ordering::Greater => return true,
                Ordering::Less => return false,
                Ordering::Equal => {}
            }
        } else if self.layer.is_some() && other.layer.is_none() {
            return true; // Layer beats no layer
        } else if self.layer.is_none() && other.layer.is_some() {
            return false; // No layer loses to layer
        }

        // 3. Specificity comparison
        match self.specificity.cmp(&other.specificity) {
            Ordering::Greater => return true,
            Ordering::Less => return false,
            Ordering::Equal => {}
        }

        // 4. Source order comparison (later rules win)
        self.source_order > other.source_order
    }
}

/// CSS cascade result
#[derive(Debug, Clone, PartialEq)]
pub struct CascadeResult<T> {
    pub value: T,
    pub context: CascadeContext,
    pub is_important: bool,
}

impl<T> CascadeResult<T> {
    pub fn new(value: T, context: CascadeContext, is_important: bool) -> Self {
        Self {
            value,
            context,
            is_important,
        }
    }

    /// Check if this result wins over another
    pub fn wins_over(&self, other: &CascadeResult<T>) -> bool {
        // Important declarations always win over non-important ones
        if self.is_important && !other.is_important {
            return true;
        }
        if !self.is_important && other.is_important {
            return false;
        }

        // If both are important or both are not important, use cascade context
        self.context.wins_over(&other.context)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_specificity_comparison() {
        let spec1 = Specificity::new(0, 0, 1); // div
        let spec2 = Specificity::new(0, 1, 0); // .class
        let spec3 = Specificity::new(1, 0, 0); // #id
        let spec4 = Specificity::new(0, 1, 1); // div.class

        assert!(spec3 > spec2);
        assert!(spec2 > spec1);
        assert!(spec4 > spec1);
        assert!(spec4 > spec2);
        assert!(spec3 > spec4);
    }

    #[test]
    fn test_specificity_addition() {
        let mut spec = Specificity::new(0, 1, 1); // div.class
        spec.add(&Specificity::new(0, 1, 0)); // .another-class
        
        assert_eq!(spec, Specificity::new(0, 2, 1));
    }

    #[test]
    fn test_cascade_context_comparison() {
        let context1 = CascadeContext::new(
            CascadeOrigin::Author,
            Specificity::new(0, 1, 0),
            1
        );
        
        let context2 = CascadeContext::new(
            CascadeOrigin::Author,
            Specificity::new(1, 0, 0),
            0
        );

        // Higher specificity wins
        assert!(context2.wins_over(&context1));

        let context3 = CascadeContext::new(
            CascadeOrigin::UserAgent,
            Specificity::new(0, 1, 0),
            0
        );

        // Author origin wins over user agent
        assert!(context1.wins_over(&context3));
    }

    #[test]
    fn test_cascade_result_comparison() {
        let result1 = CascadeResult::new(
            "red",
            CascadeContext::new(CascadeOrigin::Author, Specificity::new(0, 1, 0), 1),
            false
        );
        
        let result2 = CascadeResult::new(
            "blue",
            CascadeContext::new(CascadeOrigin::Author, Specificity::new(1, 0, 0), 0),
            false
        );

        // Higher specificity wins
        assert!(result2.wins_over(&result1));

        let result3 = CascadeResult::new(
            "green",
            CascadeContext::new(CascadeOrigin::Author, Specificity::new(0, 1, 0), 0),
            true
        );

        // Important wins over non-important
        assert!(result3.wins_over(&result1));
        assert!(result3.wins_over(&result2));
    }

    #[test]
    fn test_cascade_layer_comparison() {
        let layer1 = CascadeLayer::new("base".to_string(), 1);
        let layer2 = CascadeLayer::new("theme".to_string(), 2);
        let layer3 = CascadeLayer::new("utilities".to_string(), 3);

        assert!(layer2 > layer1);
        assert!(layer3 > layer2);

        let context1 = CascadeContext::new(
            CascadeOrigin::Author,
            Specificity::new(0, 1, 0),
            0
        ).with_layer(layer1);

        let context2 = CascadeContext::new(
            CascadeOrigin::Author,
            Specificity::new(0, 1, 0),
            0
        ).with_layer(layer2);

        // Later layer wins
        assert!(context2.wins_over(&context1));
    }
}
