use css_parser::{Declaration, Property, PropertyValue};
use css_parser::specificity::Specificity;
use std::collections::HashMap;

/// Cascade implementation for resolving conflicting CSS declarations
pub struct Cascade {
    /// Map of property names to their winning declarations
    winning_declarations: HashMap<String, Declaration>,
}

impl Cascade {
    pub fn new() -> Self {
        Self {
            winning_declarations: HashMap::new(),
        }
    }
    
    /// Apply cascade to a list of applicable rules
    pub fn apply_cascade(&mut self, rules: Vec<super::style_resolver::ApplicableRule>) -> Result<Vec<Declaration>, String> {
        self.winning_declarations.clear();
        
        // Process rules in order (already sorted by specificity)
        for rule in rules {
            for declaration in &rule.rule.declarations {
                let property_name = self.get_property_name(&declaration.property);
                
                // Check if we already have a declaration for this property
                if let Some(existing_declaration) = self.winning_declarations.get(&property_name) {
                    // Compare specificity
                    let existing_specificity = self.get_declaration_specificity(existing_declaration);
                    let new_specificity = self.get_declaration_specificity(declaration);
                    
                    // If new declaration has higher specificity, it wins
                    if new_specificity > existing_specificity {
                        self.winning_declarations.insert(property_name, declaration.clone());
                    }
                    // If specificity is equal, the later one wins (cascade order)
                    else if new_specificity == existing_specificity {
                        self.winning_declarations.insert(property_name, declaration.clone());
                    }
                } else {
                    // First declaration for this property
                    self.winning_declarations.insert(property_name, declaration.clone());
                }
            }
        }
        
        // Convert to vector and return
        Ok(self.winning_declarations.values().cloned().collect())
    }
    
    /// Get the name of a property for cascade purposes
    fn get_property_name(&self, property: &Property) -> String {
        match property {
            Property::Display(_) => "display".to_string(),
            Property::Position(_) => "position".to_string(),
            Property::Float(_) => "float".to_string(),
            Property::Clear(_) => "clear".to_string(),
            Property::Visibility(_) => "visibility".to_string(),
            Property::ZIndex(_) => "z-index".to_string(),
            Property::Width(_) => "width".to_string(),
            Property::Height(_) => "height".to_string(),
            Property::MinWidth(_) => "min-width".to_string(),
            Property::MinHeight(_) => "min-height".to_string(),
            Property::MaxWidth(_) => "max-width".to_string(),
            Property::MaxHeight(_) => "max-height".to_string(),
            Property::MarginTop(_) => "margin-top".to_string(),
            Property::MarginRight(_) => "margin-right".to_string(),
            Property::MarginBottom(_) => "margin-bottom".to_string(),
            Property::MarginLeft(_) => "margin-left".to_string(),
            Property::PaddingTop(_) => "padding-top".to_string(),
            Property::PaddingRight(_) => "padding-right".to_string(),
            Property::PaddingBottom(_) => "padding-bottom".to_string(),
            Property::PaddingLeft(_) => "padding-left".to_string(),
            Property::BorderTopWidth(_) => "border-top-width".to_string(),
            Property::BorderRightWidth(_) => "border-right-width".to_string(),
            Property::BorderBottomWidth(_) => "border-bottom-width".to_string(),
            Property::BorderLeftWidth(_) => "border-left-width".to_string(),
            Property::BorderTopStyle(_) => "border-top-style".to_string(),
            Property::BorderRightStyle(_) => "border-right-style".to_string(),
            Property::BorderBottomStyle(_) => "border-bottom-style".to_string(),
            Property::BorderLeftStyle(_) => "border-left-style".to_string(),
            Property::BorderTopColor(_) => "border-top-color".to_string(),
            Property::BorderRightColor(_) => "border-right-color".to_string(),
            Property::BorderBottomColor(_) => "border-bottom-color".to_string(),
            Property::BorderLeftColor(_) => "border-left-color".to_string(),
            Property::BackgroundColor(_) => "background-color".to_string(),
            Property::BackgroundImage(_) => "background-image".to_string(),
            Property::BackgroundRepeat(_) => "background-repeat".to_string(),
            Property::BackgroundPosition(_) => "background-position".to_string(),
            Property::BackgroundSize(_) => "background-size".to_string(),
            Property::BackgroundAttachment(_) => "background-attachment".to_string(),
            Property::FontFamily(_) => "font-family".to_string(),
            Property::FontSize(_) => "font-size".to_string(),
            Property::FontWeight(_) => "font-weight".to_string(),
            Property::FontStyle(_) => "font-style".to_string(),
            Property::LineHeight(_) => "line-height".to_string(),
            Property::LetterSpacing(_) => "letter-spacing".to_string(),
            Property::WordSpacing(_) => "word-spacing".to_string(),
            Property::Color(_) => "color".to_string(),
            Property::TextAlign(_) => "text-align".to_string(),
            Property::TextDecoration(_) => "text-decoration".to_string(),
            Property::TextIndent(_) => "text-indent".to_string(),
            Property::TextTransform(_) => "text-transform".to_string(),
            Property::WhiteSpace(_) => "white-space".to_string(),
            Property::Opacity(_) => "opacity".to_string(),
            Property::Cursor(_) => "cursor".to_string(),
            Property::Overflow(_) => "overflow".to_string(),
            Property::OverflowX(_) => "overflow-x".to_string(),
            Property::OverflowY(_) => "overflow-y".to_string(),
            Property::Transform(_) => "transform".to_string(),
            Property::TransformOrigin(_) => "transform-origin".to_string(),
            Property::TransformStyle(_) => "transform-style".to_string(),
            Property::Perspective(_) => "perspective".to_string(),
            Property::PerspectiveOrigin(_) => "perspective-origin".to_string(),
            Property::TransitionProperty(_) => "transition-property".to_string(),
            Property::TransitionDuration(_) => "transition-duration".to_string(),
            Property::TransitionTimingFunction(_) => "transition-timing-function".to_string(),
            Property::TransitionDelay(_) => "transition-delay".to_string(),
            Property::AnimationName(_) => "animation-name".to_string(),
            Property::AnimationDuration(_) => "animation-duration".to_string(),
            Property::AnimationTimingFunction(_) => "animation-timing-function".to_string(),
            Property::AnimationDelay(_) => "animation-delay".to_string(),
            Property::AnimationIterationCount(_) => "animation-iteration-count".to_string(),
            Property::AnimationDirection(_) => "animation-direction".to_string(),
            Property::AnimationFillMode(_) => "animation-fill-mode".to_string(),
            Property::AnimationPlayState(_) => "animation-play-state".to_string(),
            Property::Clip(_) => "clip".to_string(),
            Property::ClipPath(_) => "clip-path".to_string(),
            Property::Filter(_) => "filter".to_string(),
            Property::BackfaceVisibility(_) => "backface-visibility".to_string(),
            Property::BoxShadow(_) => "box-shadow".to_string(),
            Property::TextShadow(_) => "text-shadow".to_string(),
            Property::FlexDirection(_) => "flex-direction".to_string(),
            Property::FlexWrap(_) => "flex-wrap".to_string(),
            Property::FlexGrow(_) => "flex-grow".to_string(),
            Property::FlexShrink(_) => "flex-shrink".to_string(),
            Property::FlexBasis(_) => "flex-basis".to_string(),
            Property::JustifyContent(_) => "justify-content".to_string(),
            Property::AlignItems(_) => "align-items".to_string(),
            Property::AlignSelf(_) => "align-self".to_string(),
            Property::AlignContent(_) => "align-content".to_string(),
            Property::GridTemplateColumns(_) => "grid-template-columns".to_string(),
            Property::GridTemplateRows(_) => "grid-template-rows".to_string(),
            Property::GridTemplateAreas(_) => "grid-template-areas".to_string(),
            Property::GridAutoColumns(_) => "grid-auto-columns".to_string(),
            Property::GridAutoRows(_) => "grid-auto-rows".to_string(),
            Property::GridAutoFlow(_) => "grid-auto-flow".to_string(),
            Property::GridColumnStart(_) => "grid-column-start".to_string(),
            Property::GridColumnEnd(_) => "grid-column-end".to_string(),
            Property::GridRowStart(_) => "grid-row-start".to_string(),
            Property::GridRowEnd(_) => "grid-row-end".to_string(),
            Property::GridGap(_) => "grid-gap".to_string(),
            Property::GridColumnGap(_) => "grid-column-gap".to_string(),
            Property::GridRowGap(_) => "grid-row-gap".to_string(),
        }
    }
    
    /// Get specificity for a declaration (simplified - in reality this would be more complex)
    fn get_declaration_specificity(&self, _declaration: &Declaration) -> Specificity {
        // For now, return a default specificity
        // In a real implementation, this would consider the selector's specificity
        Specificity::new(0, 0, 0, 0)
    }
    
    /// Check if a property is important
    fn is_important(&self, declaration: &Declaration) -> bool {
        declaration.important
    }
    
    /// Get cascade order for style origins
    fn get_origin_order(origin: super::style_resolver::StyleOrigin) -> u8 {
        match origin {
            super::style_resolver::StyleOrigin::UserAgent => 0,
            super::style_resolver::StyleOrigin::User => 1,
            super::style_resolver::StyleOrigin::Author => 2,
        }
    }
    
    /// Resolve shorthand properties
    pub fn resolve_shorthand_properties(&self, declarations: &mut Vec<Declaration>) -> Result<(), String> {
        // This would expand shorthand properties like margin, padding, border, etc.
        // For now, just return Ok
        Ok(())
    }
    
    /// Validate property values
    pub fn validate_property_values(&self, declarations: &[Declaration]) -> Result<(), String> {
        for declaration in declarations {
            self.validate_property_value(&declaration.property)?;
        }
        Ok(())
    }
    
    /// Validate a single property value
    fn validate_property_value(&self, property: &Property) -> Result<(), String> {
        match property {
            Property::Display(_) => Ok(()),
            Property::Position(_) => Ok(()),
            Property::Float(_) => Ok(()),
            Property::Clear(_) => Ok(()),
            Property::Visibility(_) => Ok(()),
            Property::ZIndex(_) => Ok(()),
            Property::Width(_) => Ok(()),
            Property::Height(_) => Ok(()),
            Property::MinWidth(_) => Ok(()),
            Property::MinHeight(_) => Ok(()),
            Property::MaxWidth(_) => Ok(()),
            Property::MaxHeight(_) => Ok(()),
            Property::MarginTop(_) => Ok(()),
            Property::MarginRight(_) => Ok(()),
            Property::MarginBottom(_) => Ok(()),
            Property::MarginLeft(_) => Ok(()),
            Property::PaddingTop(_) => Ok(()),
            Property::PaddingRight(_) => Ok(()),
            Property::PaddingBottom(_) => Ok(()),
            Property::PaddingLeft(_) => Ok(()),
            Property::BorderTopWidth(_) => Ok(()),
            Property::BorderRightWidth(_) => Ok(()),
            Property::BorderBottomWidth(_) => Ok(()),
            Property::BorderLeftWidth(_) => Ok(()),
            Property::BorderTopStyle(_) => Ok(()),
            Property::BorderRightStyle(_) => Ok(()),
            Property::BorderBottomStyle(_) => Ok(()),
            Property::BorderLeftStyle(_) => Ok(()),
            Property::BorderTopColor(_) => Ok(()),
            Property::BorderRightColor(_) => Ok(()),
            Property::BorderBottomColor(_) => Ok(()),
            Property::BorderLeftColor(_) => Ok(()),
            Property::BackgroundColor(_) => Ok(()),
            Property::BackgroundImage(_) => Ok(()),
            Property::BackgroundRepeat(_) => Ok(()),
            Property::BackgroundPosition(_) => Ok(()),
            Property::BackgroundSize(_) => Ok(()),
            Property::BackgroundAttachment(_) => Ok(()),
            Property::FontFamily(_) => Ok(()),
            Property::FontSize(_) => Ok(()),
            Property::FontWeight(_) => Ok(()),
            Property::FontStyle(_) => Ok(()),
            Property::LineHeight(_) => Ok(()),
            Property::LetterSpacing(_) => Ok(()),
            Property::WordSpacing(_) => Ok(()),
            Property::Color(_) => Ok(()),
            Property::TextAlign(_) => Ok(()),
            Property::TextDecoration(_) => Ok(()),
            Property::TextIndent(_) => Ok(()),
            Property::TextTransform(_) => Ok(()),
            Property::WhiteSpace(_) => Ok(()),
            Property::Opacity(_) => Ok(()),
            Property::Cursor(_) => Ok(()),
            Property::Overflow(_) => Ok(()),
            Property::OverflowX(_) => Ok(()),
            Property::OverflowY(_) => Ok(()),
            Property::Transform(_) => Ok(()),
            Property::TransformOrigin(_) => Ok(()),
            Property::TransformStyle(_) => Ok(()),
            Property::Perspective(_) => Ok(()),
            Property::PerspectiveOrigin(_) => Ok(()),
            Property::TransitionProperty(_) => Ok(()),
            Property::TransitionDuration(_) => Ok(()),
            Property::TransitionTimingFunction(_) => Ok(()),
            Property::TransitionDelay(_) => Ok(()),
            Property::AnimationName(_) => Ok(()),
            Property::AnimationDuration(_) => Ok(()),
            Property::AnimationTimingFunction(_) => Ok(()),
            Property::AnimationDelay(_) => Ok(()),
            Property::AnimationIterationCount(_) => Ok(()),
            Property::AnimationDirection(_) => Ok(()),
            Property::AnimationFillMode(_) => Ok(()),
            Property::AnimationPlayState(_) => Ok(()),
            Property::Clip(_) => Ok(()),
            Property::ClipPath(_) => Ok(()),
            Property::Filter(_) => Ok(()),
            Property::BackfaceVisibility(_) => Ok(()),
            Property::BoxShadow(_) => Ok(()),
            Property::TextShadow(_) => Ok(()),
            Property::FlexDirection(_) => Ok(()),
            Property::FlexWrap(_) => Ok(()),
            Property::FlexGrow(_) => Ok(()),
            Property::FlexShrink(_) => Ok(()),
            Property::FlexBasis(_) => Ok(()),
            Property::JustifyContent(_) => Ok(()),
            Property::AlignItems(_) => Ok(()),
            Property::AlignSelf(_) => Ok(()),
            Property::AlignContent(_) => Ok(()),
            Property::GridTemplateColumns(_) => Ok(()),
            Property::GridTemplateRows(_) => Ok(()),
            Property::GridTemplateAreas(_) => Ok(()),
            Property::GridAutoColumns(_) => Ok(()),
            Property::GridAutoRows(_) => Ok(()),
            Property::GridAutoFlow(_) => Ok(()),
            Property::GridColumnStart(_) => Ok(()),
            Property::GridColumnEnd(_) => Ok(()),
            Property::GridRowStart(_) => Ok(()),
            Property::GridRowEnd(_) => Ok(()),
            Property::GridGap(_) => Ok(()),
            Property::GridColumnGap(_) => Ok(()),
            Property::GridRowGap(_) => Ok(()),
        }
    }
}

impl Default for Cascade {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use css_parser::{Declaration, Property, PropertyValue};

    #[test]
    fn test_cascade_creation() {
        let cascade = Cascade::new();
        assert!(cascade.winning_declarations.is_empty());
    }

    #[test]
    fn test_property_name_extraction() {
        let cascade = Cascade::new();
        let property = Property::Display(css_parser::properties::Display::Block);
        let name = cascade.get_property_name(&property);
        assert_eq!(name, "display");
    }

    #[test]
    fn test_origin_order() {
        assert_eq!(Cascade::get_origin_order(super::super::style_resolver::StyleOrigin::UserAgent), 0);
        assert_eq!(Cascade::get_origin_order(super::super::style_resolver::StyleOrigin::User), 1);
        assert_eq!(Cascade::get_origin_order(super::super::style_resolver::StyleOrigin::Author), 2);
    }
}
