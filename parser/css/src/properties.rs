use serde::{Deserialize, Serialize};
use std::collections::HashMap;

/// CSS property name
#[derive(Debug, Clone, PartialEq, Eq, Hash, Serialize, Deserialize)]
pub enum Property {
    // Layout properties
    Display,
    Position,
    Top,
    Right,
    Bottom,
    Left,
    ZIndex,
    Float,
    Clear,

    // Box model properties
    Width,
    Height,
    MinWidth,
    MinHeight,
    MaxWidth,
    MaxHeight,
    Margin,
    MarginTop,
    MarginRight,
    MarginBottom,
    MarginLeft,
    Padding,
    PaddingTop,
    PaddingRight,
    PaddingBottom,
    PaddingLeft,
    Border,
    BorderTop,
    BorderRight,
    BorderBottom,
    BorderLeft,
    BorderWidth,
    BorderTopWidth,
    BorderRightWidth,
    BorderBottomWidth,
    BorderLeftWidth,
    BorderStyle,
    BorderTopStyle,
    BorderRightStyle,
    BorderBottomStyle,
    BorderLeftStyle,
    BorderColor,
    BorderTopColor,
    BorderRightColor,
    BorderBottomColor,
    BorderLeftColor,
    BorderRadius,
    BorderTopLeftRadius,
    BorderTopRightRadius,
    BorderBottomLeftRadius,
    BorderBottomRightRadius,
    BoxSizing,
    Outline,
    OutlineWidth,
    OutlineStyle,
    OutlineColor,
    OutlineOffset,

    // Flexbox properties
    FlexDirection,
    FlexWrap,
    FlexFlow,
    JustifyContent,
    AlignItems,
    AlignContent,
    AlignSelf,
    Flex,
    FlexGrow,
    FlexShrink,
    FlexBasis,
    Order,

    // Grid properties
    Grid,
    GridTemplate,
    GridTemplateColumns,
    GridTemplateRows,
    GridTemplateAreas,
    GridGap,
    GridColumnGap,
    GridRowGap,
    GridArea,
    GridColumn,
    GridColumnStart,
    GridColumnEnd,
    GridRow,
    GridRowStart,
    GridRowEnd,
    JustifySelf,

    // Typography properties
    FontFamily,
    FontSize,
    FontWeight,
    FontStyle,
    FontVariant,
    FontStretch,
    Font,
    LineHeight,
    LetterSpacing,
    WordSpacing,
    TextAlign,
    TextDecoration,
    TextDecorationLine,
    TextDecorationStyle,
    TextDecorationColor,
    TextIndent,
    TextShadow,
    TextTransform,
    VerticalAlign,
    WhiteSpace,
    WordBreak,
    WordWrap,
    OverflowWrap,

    // Color properties
    Color,
    BackgroundColor,
    BackgroundImage,
    BackgroundRepeat,
    BackgroundPosition,
    BackgroundSize,
    BackgroundAttachment,
    Background,
    BackgroundClip,
    BackgroundOrigin,

    // Visual effects
    Opacity,
    Visibility,
    Transform,
    TransformOrigin,
    TransformStyle,
    Perspective,
    PerspectiveOrigin,
    BackfaceVisibility,
    Filter,
    BackdropFilter,
    BoxShadow,

    // Animation properties
    Transition,
    TransitionProperty,
    TransitionDuration,
    TransitionTimingFunction,
    TransitionDelay,
    Animation,
    AnimationName,
    AnimationDuration,
    AnimationTimingFunction,
    AnimationDelay,
    AnimationIterationCount,
    AnimationDirection,
    AnimationFillMode,
    AnimationPlayState,

    // Print properties
    PageBreakBefore,
    PageBreakAfter,
    PageBreakInside,
    BreakBefore,
    BreakAfter,
    BreakInside,
    Orphans,
    Widows,

    // Custom properties (CSS variables)
    Custom(String),

    // Unknown property
    Unknown(String),
}

impl Property {
    pub fn from_string(name: &str) -> Self {
        match name.to_lowercase().as_str() {
            "display" => Property::Display,
            "position" => Property::Position,
            "top" => Property::Top,
            "right" => Property::Right,
            "bottom" => Property::Bottom,
            "left" => Property::Left,
            "z-index" => Property::ZIndex,
            "float" => Property::Float,
            "clear" => Property::Clear,

            "width" => Property::Width,
            "height" => Property::Height,
            "min-width" => Property::MinWidth,
            "min-height" => Property::MinHeight,
            "max-width" => Property::MaxWidth,
            "max-height" => Property::MaxHeight,
            "margin" => Property::Margin,
            "margin-top" => Property::MarginTop,
            "margin-right" => Property::MarginRight,
            "margin-bottom" => Property::MarginBottom,
            "margin-left" => Property::MarginLeft,
            "padding" => Property::Padding,
            "padding-top" => Property::PaddingTop,
            "padding-right" => Property::PaddingRight,
            "padding-bottom" => Property::PaddingBottom,
            "padding-left" => Property::PaddingLeft,
            "border" => Property::Border,
            "border-top" => Property::BorderTop,
            "border-right" => Property::BorderRight,
            "border-bottom" => Property::BorderBottom,
            "border-left" => Property::BorderLeft,
            "border-width" => Property::BorderWidth,
            "border-top-width" => Property::BorderTopWidth,
            "border-right-width" => Property::BorderRightWidth,
            "border-bottom-width" => Property::BorderBottomWidth,
            "border-left-width" => Property::BorderLeftWidth,
            "border-style" => Property::BorderStyle,
            "border-top-style" => Property::BorderTopStyle,
            "border-right-style" => Property::BorderRightStyle,
            "border-bottom-style" => Property::BorderBottomStyle,
            "border-left-style" => Property::BorderLeftStyle,
            "border-color" => Property::BorderColor,
            "border-top-color" => Property::BorderTopColor,
            "border-right-color" => Property::BorderRightColor,
            "border-bottom-color" => Property::BorderBottomColor,
            "border-left-color" => Property::BorderLeftColor,
            "border-radius" => Property::BorderRadius,
            "border-top-left-radius" => Property::BorderTopLeftRadius,
            "border-top-right-radius" => Property::BorderTopRightRadius,
            "border-bottom-left-radius" => Property::BorderBottomLeftRadius,
            "border-bottom-right-radius" => Property::BorderBottomRightRadius,
            "box-sizing" => Property::BoxSizing,
            "outline" => Property::Outline,
            "outline-width" => Property::OutlineWidth,
            "outline-style" => Property::OutlineStyle,
            "outline-color" => Property::OutlineColor,
            "outline-offset" => Property::OutlineOffset,

            "flex-direction" => Property::FlexDirection,
            "flex-wrap" => Property::FlexWrap,
            "flex-flow" => Property::FlexFlow,
            "justify-content" => Property::JustifyContent,
            "align-items" => Property::AlignItems,
            "align-content" => Property::AlignContent,
            "align-self" => Property::AlignSelf,
            "flex" => Property::Flex,
            "flex-grow" => Property::FlexGrow,
            "flex-shrink" => Property::FlexShrink,
            "flex-basis" => Property::FlexBasis,
            "order" => Property::Order,

            "grid" => Property::Grid,
            "grid-template" => Property::GridTemplate,
            "grid-template-columns" => Property::GridTemplateColumns,
            "grid-template-rows" => Property::GridTemplateRows,
            "grid-template-areas" => Property::GridTemplateAreas,
            "grid-gap" => Property::GridGap,
            "grid-column-gap" => Property::GridColumnGap,
            "grid-row-gap" => Property::GridRowGap,
            "grid-area" => Property::GridArea,
            "grid-column" => Property::GridColumn,
            "grid-column-start" => Property::GridColumnStart,
            "grid-column-end" => Property::GridColumnEnd,
            "grid-row" => Property::GridRow,
            "grid-row-start" => Property::GridRowStart,
            "grid-row-end" => Property::GridRowEnd,
            "justify-self" => Property::JustifySelf,

            "font-family" => Property::FontFamily,
            "font-size" => Property::FontSize,
            "font-weight" => Property::FontWeight,
            "font-style" => Property::FontStyle,
            "font-variant" => Property::FontVariant,
            "font-stretch" => Property::FontStretch,
            "font" => Property::Font,
            "line-height" => Property::LineHeight,
            "letter-spacing" => Property::LetterSpacing,
            "word-spacing" => Property::WordSpacing,
            "text-align" => Property::TextAlign,
            "text-decoration" => Property::TextDecoration,
            "text-decoration-line" => Property::TextDecorationLine,
            "text-decoration-style" => Property::TextDecorationStyle,
            "text-decoration-color" => Property::TextDecorationColor,
            "text-indent" => Property::TextIndent,
            "text-shadow" => Property::TextShadow,
            "text-transform" => Property::TextTransform,
            "vertical-align" => Property::VerticalAlign,
            "white-space" => Property::WhiteSpace,
            "word-break" => Property::WordBreak,
            "word-wrap" => Property::WordWrap,
            "overflow-wrap" => Property::OverflowWrap,

            "color" => Property::Color,
            "background-color" => Property::BackgroundColor,
            "background-image" => Property::BackgroundImage,
            "background-repeat" => Property::BackgroundRepeat,
            "background-position" => Property::BackgroundPosition,
            "background-size" => Property::BackgroundSize,
            "background-attachment" => Property::BackgroundAttachment,
            "background" => Property::Background,
            "background-clip" => Property::BackgroundClip,
            "background-origin" => Property::BackgroundOrigin,

            "opacity" => Property::Opacity,
            "visibility" => Property::Visibility,
            "transform" => Property::Transform,
            "transform-origin" => Property::TransformOrigin,
            "transform-style" => Property::TransformStyle,
            "perspective" => Property::Perspective,
            "perspective-origin" => Property::PerspectiveOrigin,
            "backface-visibility" => Property::BackfaceVisibility,
            "filter" => Property::Filter,
            "backdrop-filter" => Property::BackdropFilter,
            "box-shadow" => Property::BoxShadow,

            "transition" => Property::Transition,
            "transition-property" => Property::TransitionProperty,
            "transition-duration" => Property::TransitionDuration,
            "transition-timing-function" => Property::TransitionTimingFunction,
            "transition-delay" => Property::TransitionDelay,
            "animation" => Property::Animation,
            "animation-name" => Property::AnimationName,
            "animation-duration" => Property::AnimationDuration,
            "animation-timing-function" => Property::AnimationTimingFunction,
            "animation-delay" => Property::AnimationDelay,
            "animation-iteration-count" => Property::AnimationIterationCount,
            "animation-direction" => Property::AnimationDirection,
            "animation-fill-mode" => Property::AnimationFillMode,
            "animation-play-state" => Property::AnimationPlayState,

            "page-break-before" => Property::PageBreakBefore,
            "page-break-after" => Property::PageBreakAfter,
            "page-break-inside" => Property::PageBreakInside,
            "break-before" => Property::BreakBefore,
            "break-after" => Property::BreakAfter,
            "break-inside" => Property::BreakInside,
            "orphans" => Property::Orphans,
            "widows" => Property::Widows,

            name if name.starts_with("--") => Property::Custom(name.to_string()),
            name => Property::Unknown(name.to_string()),
        }
    }

    #[allow(clippy::inherent_to_string)]
    pub fn to_string(&self) -> String {
        match self {
            Property::Display => "display".to_string(),
            Property::Position => "position".to_string(),
            Property::Top => "top".to_string(),
            Property::Right => "right".to_string(),
            Property::Bottom => "bottom".to_string(),
            Property::Left => "left".to_string(),
            Property::ZIndex => "z-index".to_string(),
            Property::Float => "float".to_string(),
            Property::Clear => "clear".to_string(),

            Property::Width => "width".to_string(),
            Property::Height => "height".to_string(),
            Property::MinWidth => "min-width".to_string(),
            Property::MinHeight => "min-height".to_string(),
            Property::MaxWidth => "max-width".to_string(),
            Property::MaxHeight => "max-height".to_string(),
            Property::Margin => "margin".to_string(),
            Property::MarginTop => "margin-top".to_string(),
            Property::MarginRight => "margin-right".to_string(),
            Property::MarginBottom => "margin-bottom".to_string(),
            Property::MarginLeft => "margin-left".to_string(),
            Property::Padding => "padding".to_string(),
            Property::PaddingTop => "padding-top".to_string(),
            Property::PaddingRight => "padding-right".to_string(),
            Property::PaddingBottom => "padding-bottom".to_string(),
            Property::PaddingLeft => "padding-left".to_string(),
            Property::Border => "border".to_string(),
            Property::BorderTop => "border-top".to_string(),
            Property::BorderRight => "border-right".to_string(),
            Property::BorderBottom => "border-bottom".to_string(),
            Property::BorderLeft => "border-left".to_string(),
            Property::BorderWidth => "border-width".to_string(),
            Property::BorderTopWidth => "border-top-width".to_string(),
            Property::BorderRightWidth => "border-right-width".to_string(),
            Property::BorderBottomWidth => "border-bottom-width".to_string(),
            Property::BorderLeftWidth => "border-left-width".to_string(),
            Property::BorderStyle => "border-style".to_string(),
            Property::BorderTopStyle => "border-top-style".to_string(),
            Property::BorderRightStyle => "border-right-style".to_string(),
            Property::BorderBottomStyle => "border-bottom-style".to_string(),
            Property::BorderLeftStyle => "border-left-style".to_string(),
            Property::BorderColor => "border-color".to_string(),
            Property::BorderTopColor => "border-top-color".to_string(),
            Property::BorderRightColor => "border-right-color".to_string(),
            Property::BorderBottomColor => "border-bottom-color".to_string(),
            Property::BorderLeftColor => "border-left-color".to_string(),
            Property::BorderRadius => "border-radius".to_string(),
            Property::BorderTopLeftRadius => "border-top-left-radius".to_string(),
            Property::BorderTopRightRadius => "border-top-right-radius".to_string(),
            Property::BorderBottomLeftRadius => "border-bottom-left-radius".to_string(),
            Property::BorderBottomRightRadius => "border-bottom-right-radius".to_string(),
            Property::BoxSizing => "box-sizing".to_string(),
            Property::Outline => "outline".to_string(),
            Property::OutlineWidth => "outline-width".to_string(),
            Property::OutlineStyle => "outline-style".to_string(),
            Property::OutlineColor => "outline-color".to_string(),
            Property::OutlineOffset => "outline-offset".to_string(),

            Property::FlexDirection => "flex-direction".to_string(),
            Property::FlexWrap => "flex-wrap".to_string(),
            Property::FlexFlow => "flex-flow".to_string(),
            Property::JustifyContent => "justify-content".to_string(),
            Property::AlignItems => "align-items".to_string(),
            Property::AlignContent => "align-content".to_string(),
            Property::AlignSelf => "align-self".to_string(),
            Property::Flex => "flex".to_string(),
            Property::FlexGrow => "flex-grow".to_string(),
            Property::FlexShrink => "flex-shrink".to_string(),
            Property::FlexBasis => "flex-basis".to_string(),
            Property::Order => "order".to_string(),

            Property::Grid => "grid".to_string(),
            Property::GridTemplate => "grid-template".to_string(),
            Property::GridTemplateColumns => "grid-template-columns".to_string(),
            Property::GridTemplateRows => "grid-template-rows".to_string(),
            Property::GridTemplateAreas => "grid-template-areas".to_string(),
            Property::GridGap => "grid-gap".to_string(),
            Property::GridColumnGap => "grid-column-gap".to_string(),
            Property::GridRowGap => "grid-row-gap".to_string(),
            Property::GridArea => "grid-area".to_string(),
            Property::GridColumn => "grid-column".to_string(),
            Property::GridColumnStart => "grid-column-start".to_string(),
            Property::GridColumnEnd => "grid-column-end".to_string(),
            Property::GridRow => "grid-row".to_string(),
            Property::GridRowStart => "grid-row-start".to_string(),
            Property::GridRowEnd => "grid-row-end".to_string(),
            Property::JustifySelf => "justify-self".to_string(),

            Property::FontFamily => "font-family".to_string(),
            Property::FontSize => "font-size".to_string(),
            Property::FontWeight => "font-weight".to_string(),
            Property::FontStyle => "font-style".to_string(),
            Property::FontVariant => "font-variant".to_string(),
            Property::FontStretch => "font-stretch".to_string(),
            Property::Font => "font".to_string(),
            Property::LineHeight => "line-height".to_string(),
            Property::LetterSpacing => "letter-spacing".to_string(),
            Property::WordSpacing => "word-spacing".to_string(),
            Property::TextAlign => "text-align".to_string(),
            Property::TextDecoration => "text-decoration".to_string(),
            Property::TextDecorationLine => "text-decoration-line".to_string(),
            Property::TextDecorationStyle => "text-decoration-style".to_string(),
            Property::TextDecorationColor => "text-decoration-color".to_string(),
            Property::TextIndent => "text-indent".to_string(),
            Property::TextShadow => "text-shadow".to_string(),
            Property::TextTransform => "text-transform".to_string(),
            Property::VerticalAlign => "vertical-align".to_string(),
            Property::WhiteSpace => "white-space".to_string(),
            Property::WordBreak => "word-break".to_string(),
            Property::WordWrap => "word-wrap".to_string(),
            Property::OverflowWrap => "overflow-wrap".to_string(),

            Property::Color => "color".to_string(),
            Property::BackgroundColor => "background-color".to_string(),
            Property::BackgroundImage => "background-image".to_string(),
            Property::BackgroundRepeat => "background-repeat".to_string(),
            Property::BackgroundPosition => "background-position".to_string(),
            Property::BackgroundSize => "background-size".to_string(),
            Property::BackgroundAttachment => "background-attachment".to_string(),
            Property::Background => "background".to_string(),
            Property::BackgroundClip => "background-clip".to_string(),
            Property::BackgroundOrigin => "background-origin".to_string(),

            Property::Opacity => "opacity".to_string(),
            Property::Visibility => "visibility".to_string(),
            Property::Transform => "transform".to_string(),
            Property::TransformOrigin => "transform-origin".to_string(),
            Property::TransformStyle => "transform-style".to_string(),
            Property::Perspective => "perspective".to_string(),
            Property::PerspectiveOrigin => "perspective-origin".to_string(),
            Property::BackfaceVisibility => "backface-visibility".to_string(),
            Property::Filter => "filter".to_string(),
            Property::BackdropFilter => "backdrop-filter".to_string(),
            Property::BoxShadow => "box-shadow".to_string(),

            Property::Transition => "transition".to_string(),
            Property::TransitionProperty => "transition-property".to_string(),
            Property::TransitionDuration => "transition-duration".to_string(),
            Property::TransitionTimingFunction => "transition-timing-function".to_string(),
            Property::TransitionDelay => "transition-delay".to_string(),
            Property::Animation => "animation".to_string(),
            Property::AnimationName => "animation-name".to_string(),
            Property::AnimationDuration => "animation-duration".to_string(),
            Property::AnimationTimingFunction => "animation-timing-function".to_string(),
            Property::AnimationDelay => "animation-delay".to_string(),
            Property::AnimationIterationCount => "animation-iteration-count".to_string(),
            Property::AnimationDirection => "animation-direction".to_string(),
            Property::AnimationFillMode => "animation-fill-mode".to_string(),
            Property::AnimationPlayState => "animation-play-state".to_string(),

            Property::PageBreakBefore => "page-break-before".to_string(),
            Property::PageBreakAfter => "page-break-after".to_string(),
            Property::PageBreakInside => "page-break-inside".to_string(),
            Property::BreakBefore => "break-before".to_string(),
            Property::BreakAfter => "break-after".to_string(),
            Property::BreakInside => "break-inside".to_string(),
            Property::Orphans => "orphans".to_string(),
            Property::Widows => "widows".to_string(),

            Property::Custom(name) => name.clone(),
            Property::Unknown(name) => name.clone(),
        }
    }
}

/// CSS property value
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum PropertyValue {
    // Length values
    Length(f32, LengthUnit),
    Percentage(f32),
    Auto,
    None,
    Initial,
    Inherit,
    Unset,

    // Color values
    Color(Color),

    // String values
    String(String),
    Identifier(String),

    // Numeric values
    Number(f32),
    Integer(i32),

    // Function values
    Function(String, Vec<PropertyValue>),

    // List values
    List(Vec<PropertyValue>, ListSeparator),

    // Complex values
    Display(DisplayValue),
    Position(PositionValue),
    FloatValue(FloatValue),
    Clear(ClearValue),
    BorderStyle(BorderStyleValue),
    TextAlign(TextAlignValue),
    TextDecoration(TextDecorationValue),
    FontWeight(FontWeightValue),
    FontStyle(FontStyleValue),
    FontVariant(FontVariantValue),
    TextTransform(TextTransformValue),
    WhiteSpace(WhiteSpaceValue),
    WordBreak(WordBreakValue),
    OverflowWrap(OverflowWrapValue),
    VerticalAlign(VerticalAlignValue),
    Visibility(VisibilityValue),
    Opacity(OpacityValue),
    ZIndex(ZIndexValue),

    // Flexbox values
    FlexDirection(FlexDirectionValue),
    FlexWrap(FlexWrapValue),
    JustifyContent(JustifyContentValue),
    AlignItems(AlignItemsValue),
    AlignContent(AlignContentValue),
    AlignSelf(AlignSelfValue),
    FlexBasis(FlexBasisValue),

    // Grid values
    GridTemplateColumns(GridTemplateColumnsValue),
    GridTemplateRows(GridTemplateRowsValue),
    GridArea(GridAreaValue),
    JustifySelf(JustifySelfValue),

    // Background values
    BackgroundImage(BackgroundImageValue),
    BackgroundRepeat(BackgroundRepeatValue),
    BackgroundPosition(BackgroundPositionValue),
    BackgroundSize(BackgroundSizeValue),
    BackgroundAttachment(BackgroundAttachmentValue),

    // Transform values
    Transform(TransformValue),

    // Animation values
    Transition(TransitionValue),
    Animation(AnimationValue),

    // Custom property value
    Custom(String),

    // Unknown value
    Unknown(String),
}

/// Length units
#[derive(Debug, Clone, Copy, PartialEq, Serialize, Deserialize)]
pub enum LengthUnit {
    Px,      // pixels
    Em,      // relative to font size
    Rem,     // relative to root font size
    Ex,      // relative to x-height
    Ch,      // relative to character width
    Vw,      // viewport width
    Vh,      // viewport height
    Vmin,    // viewport minimum
    Vmax,    // viewport maximum
    Pt,      // points
    Pc,      // picas
    In,      // inches
    Cm,      // centimeters
    Mm,      // millimeters
    Q,       // quarter-millimeters
    Percent, // percentage (handled separately)
}

/// Color representation
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum Color {
    Rgb(u8, u8, u8),
    Rgba(u8, u8, u8, f32),
    Hsl(f32, f32, f32),
    Hsla(f32, f32, f32, f32),
    Hex(String),
    Named(String), // CSS named colors
    Transparent,
    CurrentColor,
}

/// List separator types
#[derive(Debug, Clone, Copy, PartialEq, Serialize, Deserialize)]
pub enum ListSeparator {
    Space,
    Comma,
    Slash,
}

/// Display values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum DisplayValue {
    None,
    Block,
    Inline,
    InlineBlock,
    Flex,
    InlineFlex,
    Grid,
    InlineGrid,
    Table,
    InlineTable,
    TableRow,
    TableCell,
    TableHeaderGroup,
    TableFooterGroup,
    TableBodyGroup,
    TableColumn,
    TableColumnGroup,
    TableCaption,
    ListItem,
    RunIn,
    Compact,
    Marker,
}

/// Position values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum PositionValue {
    Static,
    Relative,
    Absolute,
    Fixed,
    Sticky,
}

/// Float values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FloatValue {
    None,
    Left,
    Right,
    InlineStart,
    InlineEnd,
}

/// Clear values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum ClearValue {
    None,
    Left,
    Right,
    Both,
    InlineStart,
    InlineEnd,
}

/// Border style values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BorderStyleValue {
    None,
    Hidden,
    Dotted,
    Dashed,
    Solid,
    Double,
    Groove,
    Ridge,
    Inset,
    Outset,
}

/// Text align values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TextAlignValue {
    Left,
    Right,
    Center,
    Justify,
    Start,
    End,
    MatchParent,
}

/// Text decoration values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TextDecorationValue {
    None,
    Underline,
    Overline,
    LineThrough,
    Blink,
}

/// Font weight values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FontWeightValue {
    Normal,
    Bold,
    Bolder,
    Lighter,
    Weight100,
    Weight200,
    Weight300,
    Weight400,
    Weight500,
    Weight600,
    Weight700,
    Weight800,
    Weight900,
}

/// Font style values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FontStyleValue {
    Normal,
    Italic,
    Oblique,
}

/// Font variant values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FontVariantValue {
    Normal,
    SmallCaps,
}

/// Text transform values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TextTransformValue {
    None,
    Capitalize,
    Uppercase,
    Lowercase,
    FullWidth,
}

/// White space values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum WhiteSpaceValue {
    Normal,
    Pre,
    Nowrap,
    PreWrap,
    PreLine,
    BreakSpaces,
}

/// Word break values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum WordBreakValue {
    Normal,
    BreakAll,
    KeepAll,
    BreakWord,
}

/// Overflow wrap values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum OverflowWrapValue {
    Normal,
    BreakWord,
    Anywhere,
}

/// Vertical align values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum VerticalAlignValue {
    Baseline,
    Sub,
    Super,
    Top,
    TextTop,
    Middle,
    Bottom,
    TextBottom,
    Length(f32, LengthUnit),
    Percentage(f32),
}

/// Visibility values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum VisibilityValue {
    Visible,
    Hidden,
    Collapse,
}

/// Opacity values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum OpacityValue {
    Value(f32), // 0.0 to 1.0
}

/// Z-index values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum ZIndexValue {
    Auto,
    Integer(i32),
}

/// Flex direction values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FlexDirectionValue {
    Row,
    RowReverse,
    Column,
    ColumnReverse,
}

/// Flex wrap values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FlexWrapValue {
    Nowrap,
    Wrap,
    WrapReverse,
}

/// Justify content values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum JustifyContentValue {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly,
}

/// Align items values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AlignItemsValue {
    Stretch,
    FlexStart,
    FlexEnd,
    Center,
    Baseline,
}

/// Align content values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AlignContentValue {
    Stretch,
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
}

/// Align self values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AlignSelfValue {
    Auto,
    Stretch,
    FlexStart,
    FlexEnd,
    Center,
    Baseline,
}

/// Flex basis values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum FlexBasisValue {
    Auto,
    Content,
    Length(f32, LengthUnit),
    Percentage(f32),
}

/// Grid template columns values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GridTemplateColumnsValue {
    None,
    Subgrid,
    List(Vec<GridTrackSize>),
}

/// Grid template rows values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GridTemplateRowsValue {
    None,
    Subgrid,
    List(Vec<GridTrackSize>),
}

/// Grid track size
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GridTrackSize {
    TrackSize(TrackSize),
    Repeat(RepeatValue, Vec<GridTrackSize>),
}

/// Track size
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TrackSize {
    Length(f32, LengthUnit),
    Percentage(f32),
    Flex(f32),
    MinContent,
    MaxContent,
    Auto,
}

/// Repeat value
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum RepeatValue {
    Integer(i32),
    AutoFill,
    AutoFit,
}

/// Grid area values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GridAreaValue {
    Named(String),
    GridArea(GridArea),
}

/// Grid area
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct GridArea {
    pub row_start: Option<GridLine>,
    pub column_start: Option<GridLine>,
    pub row_end: Option<GridLine>,
    pub column_end: Option<GridLine>,
}

/// Grid line
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GridLine {
    Number(i32),
    Named(String),
    Span(Option<i32>),
}

/// Justify self values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum JustifySelfValue {
    Auto,
    Normal,
    Stretch,
    Start,
    End,
    FlexStart,
    FlexEnd,
    Center,
    Left,
    Right,
    Baseline,
}

/// Background image values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundImageValue {
    None,
    Url(String),
    LinearGradient(LinearGradient),
    RadialGradient(RadialGradient),
    ConicGradient(ConicGradient),
}

/// Linear gradient
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct LinearGradient {
    pub direction: Option<GradientDirection>,
    pub color_stops: Vec<ColorStop>,
}

/// Radial gradient
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct RadialGradient {
    pub shape: Option<GradientShape>,
    pub position: Option<GradientPosition>,
    pub color_stops: Vec<ColorStop>,
}

/// Conic gradient
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ConicGradient {
    pub angle: Option<f32>,
    pub position: Option<GradientPosition>,
    pub color_stops: Vec<ColorStop>,
}

/// Gradient direction
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GradientDirection {
    Angle(f32),
    ToTop,
    ToRight,
    ToBottom,
    ToLeft,
    ToTopRight,
    ToTopLeft,
    ToBottomRight,
    ToBottomLeft,
}

/// Gradient shape
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GradientShape {
    Circle,
    Ellipse,
}

/// Gradient position
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum GradientPosition {
    Center,
    Top,
    Bottom,
    Left,
    Right,
    Position(f32, f32, LengthUnit, LengthUnit),
}

/// Color stop
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct ColorStop {
    pub color: Color,
    pub position: Option<f32>,
}

/// Background repeat values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundRepeatValue {
    Repeat,
    NoRepeat,
    RepeatX,
    RepeatY,
    Space,
    Round,
}

/// Background position values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundPositionValue {
    Position(Vec<BackgroundPositionItem>),
}

/// Background position item
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundPositionItem {
    Keyword(BackgroundPositionKeyword),
    Length(f32, LengthUnit),
    Percentage(f32),
}

/// Background position keyword
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundPositionKeyword {
    Top,
    Bottom,
    Left,
    Right,
    Center,
}

/// Background size values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundSizeValue {
    Auto,
    Cover,
    Contain,
    Size(BackgroundSizeItem, Option<BackgroundSizeItem>),
}

/// Background size item
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundSizeItem {
    Length(f32, LengthUnit),
    Percentage(f32),
}

/// Background attachment values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum BackgroundAttachmentValue {
    Scroll,
    Fixed,
    Local,
}

/// Transform values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TransformValue {
    None,
    List(Vec<TransformFunction>),
}

/// Transform function
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TransformFunction {
    Translate(f32, f32, Option<f32>),
    TranslateX(f32),
    TranslateY(f32),
    TranslateZ(f32),
    Scale(f32, f32, Option<f32>),
    ScaleX(f32),
    ScaleY(f32),
    ScaleZ(f32),
    Rotate(f32),
    RotateX(f32),
    RotateY(f32),
    RotateZ(f32),
    Skew(f32, f32),
    SkewX(f32),
    SkewY(f32),
    Matrix(f32, f32, f32, f32, f32, f32),
    Matrix3d(Vec<f32>), // 16 values
    Perspective(f32),
}

/// Transition values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TransitionValue {
    None,
    List(Vec<TransitionItem>),
}

/// Transition item
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct TransitionItem {
    pub property: Option<String>,
    pub duration: Option<f32>,
    pub timing_function: Option<TimingFunction>,
    pub delay: Option<f32>,
}

/// Timing function
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum TimingFunction {
    Ease,
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    CubicBezier(f32, f32, f32, f32),
    Steps(i32, Option<StepPosition>),
}

/// Step position
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum StepPosition {
    Start,
    End,
}

/// Animation values
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AnimationValue {
    None,
    List(Vec<AnimationItem>),
}

/// Animation item
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct AnimationItem {
    pub name: Option<String>,
    pub duration: Option<f32>,
    pub timing_function: Option<TimingFunction>,
    pub delay: Option<f32>,
    pub iteration_count: Option<AnimationIterationCount>,
    pub direction: Option<AnimationDirection>,
    pub fill_mode: Option<AnimationFillMode>,
    pub play_state: Option<AnimationPlayState>,
}

/// Animation iteration count
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AnimationIterationCount {
    Infinite,
    Count(f32),
}

/// Animation direction
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AnimationDirection {
    Normal,
    Reverse,
    Alternate,
    AlternateReverse,
}

/// Animation fill mode
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AnimationFillMode {
    None,
    Forwards,
    Backwards,
    Both,
}

/// Animation play state
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum AnimationPlayState {
    Running,
    Paused,
}

/// CSS property declaration
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct PropertyDeclaration {
    pub property: Property,
    pub value: PropertyValue,
    pub important: bool,
}

impl PropertyDeclaration {
    pub fn new(property: Property, value: PropertyValue, important: bool) -> Self {
        Self {
            property,
            value,
            important,
        }
    }
}

/// CSS property map for storing computed styles
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct PropertyMap {
    pub properties: HashMap<Property, PropertyValue>,
}

impl PropertyMap {
    pub fn new() -> Self {
        Self {
            properties: HashMap::new(),
        }
    }

    pub fn set_property(&mut self, property: Property, value: PropertyValue) {
        self.properties.insert(property, value);
    }

    pub fn get_property(&self, property: &Property) -> Option<&PropertyValue> {
        self.properties.get(property)
    }

    pub fn remove_property(&mut self, property: &Property) -> Option<PropertyValue> {
        self.properties.remove(property)
    }

    pub fn has_property(&self, property: &Property) -> bool {
        self.properties.contains_key(property)
    }

    pub fn merge(&mut self, other: &PropertyMap) {
        for (property, value) in &other.properties {
            self.properties.insert(property.clone(), value.clone());
        }
    }

    pub fn is_empty(&self) -> bool {
        self.properties.is_empty()
    }

    pub fn len(&self) -> usize {
        self.properties.len()
    }
}

impl Default for PropertyMap {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_property_parsing() {
        assert_eq!(Property::from_string("color"), Property::Color);
        assert_eq!(
            Property::from_string("background-color"),
            Property::BackgroundColor
        );
        assert_eq!(Property::from_string("font-size"), Property::FontSize);
        assert_eq!(
            Property::from_string("--custom-property"),
            Property::Custom("--custom-property".to_string())
        );
        assert_eq!(
            Property::from_string("unknown-property"),
            Property::Unknown("unknown-property".to_string())
        );
    }

    #[test]
    fn test_property_to_string() {
        assert_eq!(Property::Color.to_string(), "color");
        assert_eq!(Property::BackgroundColor.to_string(), "background-color");
        assert_eq!(Property::FontSize.to_string(), "font-size");
        assert_eq!(
            Property::Custom("--custom-property".to_string()).to_string(),
            "--custom-property"
        );
    }

    #[test]
    fn test_property_map_operations() {
        let mut map = PropertyMap::new();

        map.set_property(
            Property::Color,
            PropertyValue::Color(Color::Named("red".to_string())),
        );
        map.set_property(
            Property::FontSize,
            PropertyValue::Length(16.0, LengthUnit::Px),
        );

        assert!(map.has_property(&Property::Color));
        assert!(map.has_property(&Property::FontSize));
        assert!(!map.has_property(&Property::BackgroundColor));

        assert_eq!(map.len(), 2);

        if let Some(PropertyValue::Color(Color::Named(color))) = map.get_property(&Property::Color)
        {
            assert_eq!(color, "red");
        } else {
            panic!("Expected color property");
        }
    }

    #[test]
    fn test_property_map_merge() {
        let mut map1 = PropertyMap::new();
        map1.set_property(
            Property::Color,
            PropertyValue::Color(Color::Named("red".to_string())),
        );

        let mut map2 = PropertyMap::new();
        map2.set_property(
            Property::FontSize,
            PropertyValue::Length(16.0, LengthUnit::Px),
        );

        map1.merge(&map2);

        assert_eq!(map1.len(), 2);
        assert!(map1.has_property(&Property::Color));
        assert!(map1.has_property(&Property::FontSize));
    }
}
