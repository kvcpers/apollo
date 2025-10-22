#pragma once

namespace renderer {

// Blend modes
enum class BlendMode {
    Normal,
    Multiply,
    Screen,
    Overlay,
    SoftLight,
    HardLight,
    ColorDodge,
    ColorBurn,
    Darken,
    Lighten,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity
};

// Fill rules
enum class FillRule {
    EvenOdd,
    NonZero
};

// Line cap styles
enum class LineCap {
    Butt,
    Round,
    Square
};

// Line join styles
enum class LineJoin {
    Miter,
    Round,
    Bevel
};

// Text alignment
enum class TextAlign {
    Left,
    Center,
    Right,
    Justify
};

// Text baseline
enum class TextBaseline {
    Top,
    Middle,
    Bottom,
    Alphabetic,
    Ideographic
};

// Image formats
enum class ImageFormat {
    Unknown,
    PNG,
    JPEG,
    GIF,
    BMP,
    TIFF,
    WebP,
    SVG
};

// Pixel formats
enum class PixelFormat {
    Unknown,
    RGBA8888,
    RGB888,
    RGB565,
    RGBA4444,
    RGBA5551,
    Alpha8,
    Luminance8,
    LuminanceAlpha8
};

// Filter types
enum class FilterType {
    None,
    Blur,
    DropShadow,
    BoxShadow,
    Glow,
    Emboss,
    Bevel,
    Invert,
    Brightness,
    Contrast,
    Saturation,
    Hue,
    Sepia,
    Grayscale
};

// Gradient types
enum class GradientType {
    Linear,
    Radial,
    Conic
};

// Pattern types
enum class PatternType {
    Solid,
    Gradient,
    Image,
    Checkerboard,
    Stripes,
    Dots,
    Noise
};

// Transform types
enum class TransformType {
    Identity,
    Translation,
    Scale,
    Rotation,
    Skew,
    Perspective,
    Matrix
};

// Clip types
enum class ClipType {
    None,
    Rect,
    Path,
    Mask
};

// Layer types
enum class LayerType {
    Root,
    Container,
    Image,
    Text,
    Shape,
    Effect,
    Mask,
    Blend
};

// Compositor types
enum class CompositorType {
    Software,
    Hardware,
    Hybrid
};

// Backend types
enum class BackendType {
    Software,
    OpenGL,
    Vulkan,
    Metal,
    Direct3D
};

// Device types
enum class DeviceType {
    CPU,
    GPU,
    Hybrid
};

// Surface types
enum class SurfaceType {
    Window,
    Offscreen,
    Image,
    Texture,
    Framebuffer
};

// Context types
enum class ContextType {
    Immediate,
    Deferred,
    CommandBuffer
};

// Paint styles
enum class PaintStyle {
    Fill,
    Stroke,
    FillAndStroke
};

// Path commands
enum class PathCommand {
    MoveTo,
    LineTo,
    QuadTo,
    CubicTo,
    Close,
    ArcTo,
    Arc
};

// Path segments
enum class PathSegment {
    MoveTo,
    LineTo,
    QuadTo,
    CubicTo,
    Close,
    ArcTo,
    Arc
};

// Text rendering modes
enum class TextRenderingMode {
    Fill,
    Stroke,
    FillAndStroke,
    Invisible,
    FillClip,
    StrokeClip,
    FillAndStrokeClip,
    Clip
};

// Font styles
enum class FontStyle {
    Normal,
    Italic,
    Oblique
};

// Font weights
enum class FontWeight {
    Thin = 100,
    ExtraLight = 200,
    Light = 300,
    Normal = 400,
    Medium = 500,
    SemiBold = 600,
    Bold = 700,
    ExtraBold = 800,
    Black = 900
};

// Font stretch
enum class FontStretch {
    UltraCondensed = 50,
    ExtraCondensed = 62,
    Condensed = 75,
    SemiCondensed = 87,
    Normal = 100,
    SemiExpanded = 112,
    Expanded = 125,
    ExtraExpanded = 150,
    UltraExpanded = 200
};

// Antialiasing modes
enum class AntialiasMode {
    None,
    Fast,
    High,
    Best
};

// Rendering hints
enum class RenderingHint {
    Fast,
    Quality,
    Best
};

// Memory management
enum class MemoryType {
    Static,
    Dynamic,
    Stream,
    Persistent
};

// Buffer types
enum class BufferType {
    Vertex,
    Index,
    Uniform,
    Storage,
    Indirect
};

// Shader types
enum class ShaderType {
    Vertex,
    Fragment,
    Compute,
    Geometry,
    TessellationControl,
    TessellationEvaluation
};

// Primitive types
enum class PrimitiveType {
    Points,
    Lines,
    LineStrip,
    LineLoop,
    Triangles,
    TriangleStrip,
    TriangleFan,
    Quads,
    QuadStrip,
    Polygon
};

// Texture types
enum class TextureType {
    Texture1D,
    Texture2D,
    Texture3D,
    TextureCube,
    TextureArray
};

// Texture formats
enum class TextureFormat {
    R8,
    RG8,
    RGB8,
    RGBA8,
    R16,
    RG16,
    RGB16,
    RGBA16,
    R32F,
    RG32F,
    RGB32F,
    RGBA32F,
    Depth16,
    Depth24,
    Depth32F,
    Stencil8,
    Depth24Stencil8
};

// Texture filters
enum class TextureFilter {
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

// Texture wrap modes
enum class TextureWrapMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

// Framebuffer types
enum class FramebufferType {
    Color,
    Depth,
    Stencil,
    DepthStencil
};

// Render target types
enum class RenderTargetType {
    Color,
    Depth,
    Stencil,
    DepthStencil
};

// Clear flags
enum class ClearFlags {
    None = 0,
    Color = 1 << 0,
    Depth = 1 << 1,
    Stencil = 1 << 2,
    All = Color | Depth | Stencil
};

// Cull face modes
enum class CullFaceMode {
    None,
    Front,
    Back,
    FrontAndBack
};

// Front face winding
enum class FrontFaceWinding {
    Clockwise,
    CounterClockwise
};

// Depth test functions
enum class DepthTestFunction {
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always
};

// Stencil test functions
enum class StencilTestFunction {
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always
};

// Stencil operations
enum class StencilOperation {
    Keep,
    Zero,
    Replace,
    Increment,
    IncrementWrap,
    Decrement,
    DecrementWrap,
    Invert
};

// Blend functions
enum class BlendFunction {
    Zero,
    One,
    SourceColor,
    OneMinusSourceColor,
    DestinationColor,
    OneMinusDestinationColor,
    SourceAlpha,
    OneMinusSourceAlpha,
    DestinationAlpha,
    OneMinusDestinationAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
    SourceAlphaSaturate
};

// Blend equations
enum class BlendEquation {
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

// Logic operations
enum class LogicOperation {
    Clear,
    Set,
    Copy,
    CopyInverted,
    Noop,
    Invert,
    And,
    Nand,
    Or,
    Nor,
    Xor,
    Equiv,
    AndReverse,
    AndInverted,
    OrReverse,
    OrInverted
};

// Polygon modes
enum class PolygonMode {
    Point,
    Line,
    Fill
};

// Line width
enum class LineWidth {
    Thin = 1,
    Normal = 2,
    Thick = 3
};

// Point size
enum class PointSize {
    Small = 1,
    Normal = 2,
    Large = 3
};

// Scissor test
enum class ScissorTest {
    Disabled,
    Enabled
};

// Multisampling
enum class Multisampling {
    Disabled,
    Enabled
};

// Sample count
enum class SampleCount {
    None = 0,
    X2 = 2,
    X4 = 4,
    X8 = 8,
    X16 = 16
};

// Sync types
enum class SyncType {
    None,
    Fence,
    Semaphore,
    Event
};

// Command buffer types
enum class CommandBufferType {
    Primary,
    Secondary
};

// Command buffer usage
enum class CommandBufferUsage {
    OneTime,
    RenderPass,
    Simultaneous
};

// Queue types
enum class QueueType {
    Graphics,
    Compute,
    Transfer,
    Present
};

// Queue priority
enum class QueuePriority {
    Low,
    Normal,
    High,
    Critical
};

// Memory properties
enum class MemoryProperty {
    DeviceLocal = 1 << 0,
    HostVisible = 1 << 1,
    HostCoherent = 1 << 2,
    HostCached = 1 << 3,
    LazilyAllocated = 1 << 4
};

// Buffer usage
enum class BufferUsage {
    TransferSrc = 1 << 0,
    TransferDst = 1 << 1,
    UniformTexelBuffer = 1 << 2,
    StorageTexelBuffer = 1 << 3,
    UniformBuffer = 1 << 4,
    StorageBuffer = 1 << 5,
    IndexBuffer = 1 << 6,
    VertexBuffer = 1 << 7,
    IndirectBuffer = 1 << 8
};

// Image usage
enum class ImageUsage {
    TransferSrc = 1 << 0,
    TransferDst = 1 << 1,
    Sampled = 1 << 2,
    Storage = 1 << 3,
    ColorAttachment = 1 << 4,
    DepthStencilAttachment = 1 << 5,
    TransientAttachment = 1 << 6,
    InputAttachment = 1 << 7
};

// Image layout
enum class ImageLayout {
    Undefined,
    General,
    ColorAttachmentOptimal,
    DepthStencilAttachmentOptimal,
    DepthStencilReadOnlyOptimal,
    ShaderReadOnlyOptimal,
    TransferSrcOptimal,
    TransferDstOptimal,
    Preinitialized,
    PresentSrcKHR
};

// Access flags
enum class AccessFlags {
    None = 0,
    IndirectCommandRead = 1 << 0,
    IndexRead = 1 << 1,
    VertexAttributeRead = 1 << 2,
    UniformRead = 1 << 3,
    InputAttachmentRead = 1 << 4,
    ShaderRead = 1 << 5,
    ShaderWrite = 1 << 6,
    ColorAttachmentRead = 1 << 7,
    ColorAttachmentWrite = 1 << 8,
    DepthStencilAttachmentRead = 1 << 9,
    DepthStencilAttachmentWrite = 1 << 10,
    TransferRead = 1 << 11,
    TransferWrite = 1 << 12,
    HostRead = 1 << 13,
    HostWrite = 1 << 14,
    MemoryRead = 1 << 15,
    MemoryWrite = 1 << 16
};

// Pipeline stage flags
enum class PipelineStageFlags {
    None = 0,
    TopOfPipe = 1 << 0,
    DrawIndirect = 1 << 1,
    VertexInput = 1 << 2,
    VertexShader = 1 << 3,
    TessellationControlShader = 1 << 4,
    TessellationEvaluationShader = 1 << 5,
    GeometryShader = 1 << 6,
    FragmentShader = 1 << 7,
    EarlyFragmentTests = 1 << 8,
    LateFragmentTests = 1 << 9,
    ColorAttachmentOutput = 1 << 10,
    ComputeShader = 1 << 11,
    Transfer = 1 << 12,
    BottomOfPipe = 1 << 13,
    Host = 1 << 14,
    AllGraphics = 1 << 15,
    AllCommands = 1 << 16
};

} // namespace renderer
