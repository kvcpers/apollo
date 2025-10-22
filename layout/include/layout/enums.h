#pragma once

namespace layout {

// Display types
enum class Display {
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
    TableColumn,
    TableColumnGroup,
    TableRowGroup,
    TableHeaderGroup,
    TableFooterGroup,
    TableCaption,
    ListItem,
    RunIn,
    Compact,
    Marker,
    Contents
};

// Position types
enum class Position {
    Static,
    Relative,
    Absolute,
    Fixed,
    Sticky
};

// Float types
enum class Float {
    None,
    Left,
    Right
};

// Clear types
enum class Clear {
    None,
    Left,
    Right,
    Both
};

// Box sizing
enum class BoxSizing {
    ContentBox,
    BorderBox
};

// Text alignment
enum class TextAlign {
    Left,
    Right,
    Center,
    Justify,
    Start,
    End
};

// Vertical alignment
enum class VerticalAlign {
    Baseline,
    Sub,
    Super,
    Top,
    TextTop,
    Middle,
    Bottom,
    TextBottom,
    Length
};

// White space handling
enum class WhiteSpace {
    Normal,
    Pre,
    Nowrap,
    PreWrap,
    PreLine
};

// Text decoration
enum class TextDecoration {
    None,
    Underline,
    Overline,
    LineThrough,
    Blink
};

// Text transform
enum class TextTransform {
    None,
    Capitalize,
    Uppercase,
    Lowercase
};

// Direction
enum class Direction {
    LTR,
    RTL
};

// Writing mode
enum class WritingMode {
    HorizontalTb,
    VerticalRl,
    VerticalLr,
    SidewaysRl,
    SidewaysLr
};

// Flex direction
enum class FlexDirection {
    Row,
    RowReverse,
    Column,
    ColumnReverse
};

// Flex wrap
enum class FlexWrap {
    Nowrap,
    Wrap,
    WrapReverse
};

// Justify content (flexbox)
enum class JustifyContent {
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly
};

// Align items (flexbox)
enum class AlignItems {
    Stretch,
    FlexStart,
    FlexEnd,
    Center,
    Baseline
};

// Align self (flexbox)
enum class AlignSelf {
    Auto,
    Stretch,
    FlexStart,
    FlexEnd,
    Center,
    Baseline
};

// Align content (flexbox)
enum class AlignContent {
    Stretch,
    FlexStart,
    FlexEnd,
    Center,
    SpaceBetween,
    SpaceAround
};

// Grid template areas
enum class GridTemplateAreas {
    None,
    Areas
};

// Grid auto flow
enum class GridAutoFlow {
    Row,
    Column,
    Dense,
    RowDense,
    ColumnDense
};

// Border style
enum class BorderStyle {
    None,
    Hidden,
    Dotted,
    Dashed,
    Solid,
    Double,
    Groove,
    Ridge,
    Inset,
    Outset
};

// Border collapse
enum class BorderCollapse {
    Separate,
    Collapse
};

// Table layout
enum class TableLayout {
    Auto,
    Fixed
};

// Caption side
enum class CaptionSide {
    Top,
    Bottom
};

// Empty cells
enum class EmptyCells {
    Show,
    Hide
};

// List style type
enum class ListStyleType {
    None,
    Disc,
    Circle,
    Square,
    Decimal,
    DecimalLeadingZero,
    LowerRoman,
    UpperRoman,
    LowerGreek,
    LowerLatin,
    UpperLatin,
    Armenian,
    Georgian,
    LowerAlpha,
    UpperAlpha
};

// List style position
enum class ListStylePosition {
    Outside,
    Inside
};

// Overflow
enum class Overflow {
    Visible,
    Hidden,
    Scroll,
    Auto
};

// Visibility
enum class Visibility {
    Visible,
    Hidden,
    Collapse
};

// Cursor
enum class Cursor {
    Auto,
    Default,
    None,
    ContextMenu,
    Help,
    Pointer,
    Progress,
    Wait,
    Cell,
    Crosshair,
    Text,
    VerticalText,
    Alias,
    Copy,
    Move,
    NoDrop,
    NotAllowed,
    Grab,
    Grabbing,
    EResize,
    NResize,
    NEResize,
    NWResize,
    SResize,
    SEResize,
    SWResize,
    WResize,
    EWResize,
    NSResize,
    NESWResize,
    NW SEResize,
    ColResize,
    RowResize,
    AllScroll,
    ZoomIn,
    ZoomOut
};

// Outline style
enum class OutlineStyle {
    None,
    Hidden,
    Dotted,
    Dashed,
    Solid,
    Double,
    Groove,
    Ridge,
    Inset,
    Outset
};

// Background attachment
enum class BackgroundAttachment {
    Scroll,
    Fixed,
    Local
};

// Background repeat
enum class BackgroundRepeat {
    Repeat,
    RepeatX,
    RepeatY,
    NoRepeat,
    Space,
    Round
};

// Background size
enum class BackgroundSize {
    Auto,
    Cover,
    Contain,
    Length,
    Percentage
};

// Background position
enum class BackgroundPosition {
    Top,
    Bottom,
    Left,
    Right,
    Center,
    Length,
    Percentage
};

// Background clip
enum class BackgroundClip {
    BorderBox,
    PaddingBox,
    ContentBox,
    Text
};

// Background origin
enum class BackgroundOrigin {
    BorderBox,
    PaddingBox,
    ContentBox
};

// Font style
enum class FontStyle {
    Normal,
    Italic,
    Oblique
};

// Font weight
enum class FontWeight {
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
    Weight900
};

// Font variant
enum class FontVariant {
    Normal,
    SmallCaps
};

// Font stretch
enum class FontStretch {
    Normal,
    UltraCondensed,
    ExtraCondensed,
    Condensed,
    SemiCondensed,
    SemiExpanded,
    Expanded,
    ExtraExpanded,
    UltraExpanded
};

// Line height
enum class LineHeight {
    Normal,
    Length,
    Percentage,
    Number
};

// Word break
enum class WordBreak {
    Normal,
    BreakAll,
    KeepAll,
    BreakWord
};

// Word wrap
enum class WordWrap {
    Normal,
    BreakWord
};

// Hyphens
enum class Hyphens {
    None,
    Manual,
    Auto
};

// Text overflow
enum class TextOverflow {
    Clip,
    Ellipsis,
    String
};

// Text orientation
enum class TextOrientation {
    Mixed,
    Upright,
    Sideways
};

// Text combine upright
enum class TextCombineUpright {
    None,
    All,
    Digits
};

// Text decoration style
enum class TextDecorationStyle {
    Solid,
    Double,
    Dotted,
    Dashed,
    Wavy
};

// Text decoration line
enum class TextDecorationLine {
    None,
    Underline,
    Overline,
    LineThrough,
    Blink
};

// Text underline position
enum class TextUnderlinePosition {
    Auto,
    Under,
    Left,
    Right,
    UnderLeft,
    UnderRight
};

// Text decoration skip
enum class TextDecorationSkip {
    None,
    Objects,
    Spaces,
    LeadingSpaces,
    TrailingSpaces,
    Edges,
    BoxDecoration
};

// Text decoration skip ink
enum class TextDecorationSkipInk {
    Auto,
    None,
    All
};

// Text emphasis style
enum class TextEmphasisStyle {
    None,
    Filled,
    Open,
    Dot,
    Circle,
    DoubleCircle,
    Triangle,
    Sesame
};

// Text emphasis position
enum class TextEmphasisPosition {
    Over,
    Under,
    Left,
    Right,
    OverRight,
    OverLeft,
    UnderRight,
    UnderLeft
};

// Text shadow
enum class TextShadow {
    None,
    Shadow
};

// Box shadow
enum class BoxShadow {
    None,
    Shadow
};

// Filter
enum class Filter {
    None,
    Blur,
    Brightness,
    Contrast,
    DropShadow,
    Grayscale,
    HueRotate,
    Invert,
    Opacity,
    Saturate,
    Sepia
};

// Backdrop filter
enum class BackdropFilter {
    None,
    Blur,
    Brightness,
    Contrast,
    DropShadow,
    Grayscale,
    HueRotate,
    Invert,
    Opacity,
    Saturate,
    Sepia
};

// Transform style
enum class TransformStyle {
    Flat,
    Preserve3D
};

// Transform origin
enum class TransformOrigin {
    Center,
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

// Perspective
enum class Perspective {
    None,
    Length
};

// Perspective origin
enum class PerspectiveOrigin {
    Center,
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

// Backface visibility
enum class BackfaceVisibility {
    Visible,
    Hidden
};

// Transition timing function
enum class TransitionTimingFunction {
    Ease,
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    StepStart,
    StepEnd,
    Steps,
    CubicBezier
};

// Animation direction
enum class AnimationDirection {
    Normal,
    Reverse,
    Alternate,
    AlternateReverse
};

// Animation fill mode
enum class AnimationFillMode {
    None,
    Forwards,
    Backwards,
    Both
};

// Animation play state
enum class AnimationPlayState {
    Running,
    Paused
};

// Animation iteration count
enum class AnimationIterationCount {
    Infinite,
    Count
};

// Animation timing function
enum class AnimationTimingFunction {
    Ease,
    Linear,
    EaseIn,
    EaseOut,
    EaseInOut,
    StepStart,
    StepEnd,
    Steps,
    CubicBezier
};

// Will change
enum class WillChange {
    Auto,
    ScrollPosition,
    Contents,
    Transform,
    Opacity
};

// Contain
enum class Contain {
    None,
    Strict,
    Content,
    Size,
    Layout,
    Style,
    Paint
};

// Isolation
enum class Isolation {
    Auto,
    Isolate
};

// Mix blend mode
enum class MixBlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    Darken,
    Lighten,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity
};

// Object fit
enum class ObjectFit {
    Fill,
    Contain,
    Cover,
    None,
    ScaleDown
};

// Object position
enum class ObjectPosition {
    Center,
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

// Resize
enum class Resize {
    None,
    Both,
    Horizontal,
    Vertical,
    Block,
    Inline
};

// User select
enum class UserSelect {
    Auto,
    None,
    Text,
    All,
    Contain
};

// Pointer events
enum class PointerEvents {
    Auto,
    None,
    VisiblePainted,
    VisibleFill,
    VisibleStroke,
    Visible,
    Painted,
    Fill,
    Stroke,
    All
};

// Touch action
enum class TouchAction {
    Auto,
    None,
    PanX,
    PanY,
    PanLeft,
    PanRight,
    PanUp,
    PanDown,
    Manipulation
};

// Scroll behavior
enum class ScrollBehavior {
    Auto,
    Smooth
};

// Overscroll behavior
enum class OverscrollBehavior {
    Auto,
    Contain,
    None
};

// Scroll snap type
enum class ScrollSnapType {
    None,
    X,
    Y,
    Block,
    Inline,
    Both
};

// Scroll snap align
enum class ScrollSnapAlign {
    None,
    Start,
    End,
    Center
};

// Scroll snap stop
enum class ScrollSnapStop {
    Normal,
    Always
};

// Scroll margin
enum class ScrollMargin {
    Auto,
    Length
};

// Scroll padding
enum class ScrollPadding {
    Auto,
    Length
};

} // namespace layout
