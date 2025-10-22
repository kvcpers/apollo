use css_parser::properties::{Color, Length, Percentage, Angle, Time};
use css_parser::properties::{BorderStyle, Display, Position, Float, Clear, Visibility};
use css_parser::properties::{FontWeight, FontStyle, TextAlign, TextDecoration, LineHeight};
use css_parser::properties::{BackgroundSize, BackgroundRepeat, BackgroundPosition};
use serde::{Serialize, Deserialize};

/// Computed style values for an element
#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ComputedStyle {
    // Display and positioning
    pub display: Display,
    pub position: Position,
    pub float: Float,
    pub clear: Clear,
    pub visibility: Visibility,
    pub z_index: Option<i32>,
    
    // Box model
    pub width: Length,
    pub height: Length,
    pub min_width: Length,
    pub min_height: Length,
    pub max_width: Length,
    pub max_height: Length,
    
    // Margins
    pub margin_top: Length,
    pub margin_right: Length,
    pub margin_bottom: Length,
    pub margin_left: Length,
    
    // Padding
    pub padding_top: Length,
    pub padding_right: Length,
    pub padding_bottom: Length,
    pub padding_left: Length,
    
    // Borders
    pub border_top_width: Length,
    pub border_right_width: Length,
    pub border_bottom_width: Length,
    pub border_left_width: Length,
    
    pub border_top_style: BorderStyle,
    pub border_right_style: BorderStyle,
    pub border_bottom_style: BorderStyle,
    pub border_left_style: BorderStyle,
    
    pub border_top_color: Color,
    pub border_right_color: Color,
    pub border_bottom_color: Color,
    pub border_left_color: Color,
    
    // Background
    pub background_color: Color,
    pub background_image: Option<String>,
    pub background_repeat: BackgroundRepeat,
    pub background_position: BackgroundPosition,
    pub background_size: BackgroundSize,
    pub background_attachment: String,
    
    // Typography
    pub font_family: Vec<String>,
    pub font_size: Length,
    pub font_weight: FontWeight,
    pub font_style: FontStyle,
    pub line_height: LineHeight,
    pub letter_spacing: Length,
    pub word_spacing: Length,
    
    // Text
    pub color: Color,
    pub text_align: TextAlign,
    pub text_decoration: TextDecoration,
    pub text_indent: Length,
    pub text_transform: String,
    pub white_space: String,
    
    // Flexbox
    pub flex_direction: String,
    pub flex_wrap: String,
    pub flex_grow: f32,
    pub flex_shrink: f32,
    pub flex_basis: Length,
    pub justify_content: String,
    pub align_items: String,
    pub align_self: String,
    pub align_content: String,
    
    // Grid
    pub grid_template_columns: String,
    pub grid_template_rows: String,
    pub grid_template_areas: String,
    pub grid_auto_columns: String,
    pub grid_auto_rows: String,
    pub grid_auto_flow: String,
    pub grid_column_start: String,
    pub grid_column_end: String,
    pub grid_row_start: String,
    pub grid_row_end: String,
    pub grid_gap: Length,
    pub grid_column_gap: Length,
    pub grid_row_gap: Length,
    
    // Transforms
    pub transform: String,
    pub transform_origin: String,
    pub transform_style: String,
    pub perspective: Length,
    pub perspective_origin: String,
    
    // Transitions and animations
    pub transition_property: String,
    pub transition_duration: Time,
    pub transition_timing_function: String,
    pub transition_delay: Time,
    pub animation_name: String,
    pub animation_duration: Time,
    pub animation_timing_function: String,
    pub animation_delay: Time,
    pub animation_iteration_count: String,
    pub animation_direction: String,
    pub animation_fill_mode: String,
    pub animation_play_state: String,
    
    // Other
    pub opacity: f32,
    pub cursor: String,
    pub overflow: String,
    pub overflow_x: String,
    pub overflow_y: String,
    pub clip: String,
    pub clip_path: String,
    pub filter: String,
    pub backface_visibility: String,
    pub box_shadow: String,
    pub text_shadow: String,
}

impl Default for ComputedStyle {
    fn default() -> Self {
        Self {
            display: Display::Inline,
            position: Position::Static,
            float: Float::None,
            clear: Clear::None,
            visibility: Visibility::Visible,
            z_index: None,
            
            width: Length::Auto,
            height: Length::Auto,
            min_width: Length::Auto,
            min_height: Length::Auto,
            max_width: Length::Auto,
            max_height: Length::Auto,
            
            margin_top: Length::Px(0.0),
            margin_right: Length::Px(0.0),
            margin_bottom: Length::Px(0.0),
            margin_left: Length::Px(0.0),
            
            padding_top: Length::Px(0.0),
            padding_right: Length::Px(0.0),
            padding_bottom: Length::Px(0.0),
            padding_left: Length::Px(0.0),
            
            border_top_width: Length::Px(0.0),
            border_right_width: Length::Px(0.0),
            border_bottom_width: Length::Px(0.0),
            border_left_width: Length::Px(0.0),
            
            border_top_style: BorderStyle::None,
            border_right_style: BorderStyle::None,
            border_bottom_style: BorderStyle::None,
            border_left_style: BorderStyle::None,
            
            border_top_color: Color::Transparent,
            border_right_color: Color::Transparent,
            border_bottom_color: Color::Transparent,
            border_left_color: Color::Transparent,
            
            background_color: Color::Transparent,
            background_image: None,
            background_repeat: BackgroundRepeat::Repeat,
            background_position: BackgroundPosition::Percentage(Percentage::new(0.0), Percentage::new(0.0)),
            background_size: BackgroundSize::Auto,
            background_attachment: "scroll".to_string(),
            
            font_family: vec!["serif".to_string()],
            font_size: Length::Px(16.0),
            font_weight: FontWeight::Normal,
            font_style: FontStyle::Normal,
            line_height: LineHeight::Normal,
            letter_spacing: Length::Px(0.0),
            word_spacing: Length::Px(0.0),
            
            color: Color::Rgb(0, 0, 0),
            text_align: TextAlign::Start,
            text_decoration: TextDecoration::None,
            text_indent: Length::Px(0.0),
            text_transform: "none".to_string(),
            white_space: "normal".to_string(),
            
            flex_direction: "row".to_string(),
            flex_wrap: "nowrap".to_string(),
            flex_grow: 0.0,
            flex_shrink: 1.0,
            flex_basis: Length::Auto,
            justify_content: "flex-start".to_string(),
            align_items: "stretch".to_string(),
            align_self: "auto".to_string(),
            align_content: "stretch".to_string(),
            
            grid_template_columns: "none".to_string(),
            grid_template_rows: "none".to_string(),
            grid_template_areas: "none".to_string(),
            grid_auto_columns: "auto".to_string(),
            grid_auto_rows: "auto".to_string(),
            grid_auto_flow: "row".to_string(),
            grid_column_start: "auto".to_string(),
            grid_column_end: "auto".to_string(),
            grid_row_start: "auto".to_string(),
            grid_row_end: "auto".to_string(),
            grid_gap: Length::Px(0.0),
            grid_column_gap: Length::Px(0.0),
            grid_row_gap: Length::Px(0.0),
            
            transform: "none".to_string(),
            transform_origin: "50% 50% 0".to_string(),
            transform_style: "flat".to_string(),
            perspective: Length::None,
            perspective_origin: "50% 50%".to_string(),
            
            transition_property: "all".to_string(),
            transition_duration: Time::Ms(0.0),
            transition_timing_function: "ease".to_string(),
            transition_delay: Time::Ms(0.0),
            animation_name: "none".to_string(),
            animation_duration: Time::Ms(0.0),
            animation_timing_function: "ease".to_string(),
            animation_delay: Time::Ms(0.0),
            animation_iteration_count: "1".to_string(),
            animation_direction: "normal".to_string(),
            animation_fill_mode: "none".to_string(),
            animation_play_state: "running".to_string(),
            
            opacity: 1.0,
            cursor: "auto".to_string(),
            overflow: "visible".to_string(),
            overflow_x: "visible".to_string(),
            overflow_y: "visible".to_string(),
            clip: "auto".to_string(),
            clip_path: "none".to_string(),
            filter: "none".to_string(),
            backface_visibility: "visible".to_string(),
            box_shadow: "none".to_string(),
            text_shadow: "none".to_string(),
        }
    }
}

impl ComputedStyle {
    /// Create a new computed style with default values
    pub fn new() -> Self {
        Self::default()
    }
    
    /// Get the computed margin for a given side
    pub fn margin(&self, side: &str) -> Length {
        match side {
            "top" => self.margin_top,
            "right" => self.margin_right,
            "bottom" => self.margin_bottom,
            "left" => self.margin_left,
            _ => Length::Px(0.0),
        }
    }
    
    /// Get the computed padding for a given side
    pub fn padding(&self, side: &str) -> Length {
        match side {
            "top" => self.padding_top,
            "right" => self.padding_right,
            "bottom" => self.padding_bottom,
            "left" => self.padding_left,
            _ => Length::Px(0.0),
        }
    }
    
    /// Get the computed border width for a given side
    pub fn border_width(&self, side: &str) -> Length {
        match side {
            "top" => self.border_top_width,
            "right" => self.border_right_width,
            "bottom" => self.border_bottom_width,
            "left" => self.border_left_width,
            _ => Length::Px(0.0),
        }
    }
    
    /// Get the computed border style for a given side
    pub fn border_style(&self, side: &str) -> BorderStyle {
        match side {
            "top" => self.border_top_style,
            "right" => self.border_right_style,
            "bottom" => self.border_bottom_style,
            "left" => self.border_left_style,
            _ => BorderStyle::None,
        }
    }
    
    /// Get the computed border color for a given side
    pub fn border_color(&self, side: &str) -> Color {
        match side {
            "top" => self.border_top_color,
            "right" => self.border_right_color,
            "bottom" => self.border_bottom_color,
            "left" => self.border_left_color,
            _ => Color::Transparent,
        }
    }
    
    /// Check if the element is positioned
    pub fn is_positioned(&self) -> bool {
        matches!(self.position, Position::Absolute | Position::Fixed | Position::Relative | Position::Sticky)
    }
    
    /// Check if the element is floating
    pub fn is_floating(&self) -> bool {
        !matches!(self.float, Float::None)
    }
    
    /// Check if the element is block-level
    pub fn is_block_level(&self) -> bool {
        matches!(self.display, Display::Block | Display::ListItem | Display::Table | Display::Flex | Display::Grid)
    }
    
    /// Check if the element is inline-level
    pub fn is_inline_level(&self) -> bool {
        matches!(self.display, Display::Inline | Display::InlineBlock | Display::InlineTable | Display::InlineFlex | Display::InlineGrid)
    }
    
    /// Get the total horizontal margin (left + right)
    pub fn total_horizontal_margin(&self) -> Length {
        match (self.margin_left, self.margin_right) {
            (Length::Px(left), Length::Px(right)) => Length::Px(left + right),
            _ => Length::Auto, // Can't compute if margins are auto
        }
    }
    
    /// Get the total vertical margin (top + bottom)
    pub fn total_vertical_margin(&self) -> Length {
        match (self.margin_top, self.margin_bottom) {
            (Length::Px(top), Length::Px(bottom)) => Length::Px(top + bottom),
            _ => Length::Auto, // Can't compute if margins are auto
        }
    }
    
    /// Get the total horizontal padding (left + right)
    pub fn total_horizontal_padding(&self) -> Length {
        match (self.padding_left, self.padding_right) {
            (Length::Px(left), Length::Px(right)) => Length::Px(left + right),
            _ => Length::Px(0.0),
        }
    }
    
    /// Get the total vertical padding (top + bottom)
    pub fn total_vertical_padding(&self) -> Length {
        match (self.padding_top, self.padding_bottom) {
            (Length::Px(top), Length::Px(bottom)) => Length::Px(top + bottom),
            _ => Length::Px(0.0),
        }
    }
    
    /// Get the total horizontal border width (left + right)
    pub fn total_horizontal_border(&self) -> Length {
        match (self.border_left_width, self.border_right_width) {
            (Length::Px(left), Length::Px(right)) => Length::Px(left + right),
            _ => Length::Px(0.0),
        }
    }
    
    /// Get the total vertical border width (top + bottom)
    pub fn total_vertical_border(&self) -> Length {
        match (self.border_top_width, self.border_bottom_width) {
            (Length::Px(top), Length::Px(bottom)) => Length::Px(top + bottom),
            _ => Length::Px(0.0),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_computed_style_default() {
        let style = ComputedStyle::new();
        assert_eq!(style.display, Display::Inline);
        assert_eq!(style.position, Position::Static);
        assert_eq!(style.color, Color::Rgb(0, 0, 0));
    }

    #[test]
    fn test_margin_getters() {
        let mut style = ComputedStyle::new();
        style.margin_top = Length::Px(10.0);
        style.margin_right = Length::Px(20.0);
        style.margin_bottom = Length::Px(30.0);
        style.margin_left = Length::Px(40.0);
        
        assert_eq!(style.margin("top"), Length::Px(10.0));
        assert_eq!(style.margin("right"), Length::Px(20.0));
        assert_eq!(style.margin("bottom"), Length::Px(30.0));
        assert_eq!(style.margin("left"), Length::Px(40.0));
    }

    #[test]
    fn test_positioning_checks() {
        let mut style = ComputedStyle::new();
        
        assert!(!style.is_positioned());
        assert!(!style.is_floating());
        
        style.position = Position::Relative;
        assert!(style.is_positioned());
        
        style.float = Float::Left;
        assert!(style.is_floating());
    }

    #[test]
    fn test_block_level_checks() {
        let mut style = ComputedStyle::new();
        
        assert!(!style.is_block_level());
        assert!(style.is_inline_level());
        
        style.display = Display::Block;
        assert!(style.is_block_level());
        assert!(!style.is_inline_level());
    }

    #[test]
    fn test_total_calculations() {
        let mut style = ComputedStyle::new();
        style.margin_left = Length::Px(10.0);
        style.margin_right = Length::Px(20.0);
        style.padding_left = Length::Px(5.0);
        style.padding_right = Length::Px(15.0);
        style.border_left_width = Length::Px(2.0);
        style.border_right_width = Length::Px(3.0);
        
        assert_eq!(style.total_horizontal_margin(), Length::Px(30.0));
        assert_eq!(style.total_horizontal_padding(), Length::Px(20.0));
        assert_eq!(style.total_horizontal_border(), Length::Px(5.0));
    }
}
