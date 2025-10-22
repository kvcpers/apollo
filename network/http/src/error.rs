use thiserror::Error;

#[derive(Error, Debug)]
pub enum HttpError {
    #[error("Invalid URL: {0}")]
    InvalidUrl(String),

    #[error("Invalid HTTP method: {0}")]
    InvalidMethod(String),

    #[error("Invalid HTTP status: {0}")]
    InvalidStatus(u16),

    #[error("Invalid header: {0}")]
    InvalidHeader(String),

    #[error("Invalid cookie: {0}")]
    InvalidCookie(String),

    #[error("Invalid cache entry: {0}")]
    InvalidCacheEntry(String),

    #[error("Network error: {0}")]
    NetworkError(String),

    #[error("Connection error: {0}")]
    ConnectionError(String),

    #[error("Timeout error: {0}")]
    TimeoutError(String),

    #[error("SSL/TLS error: {0}")]
    SslError(String),

    #[error("Redirect error: {0}")]
    RedirectError(String),

    #[error("Cache error: {0}")]
    CacheError(String),

    #[error("Serialization error: {0}")]
    SerializationError(String),

    #[error("Deserialization error: {0}")]
    DeserializationError(String),

    #[error("Decoding error: {0}")]
    DecodingError(String),

    #[error("Compression error: {0}")]
    CompressionError(String),

    #[error("Decompression error: {0}")]
    DecompressionError(String),

    #[error("Authentication error: {0}")]
    AuthenticationError(String),

    #[error("Authorization error: {0}")]
    AuthorizationError(String),

    #[error("Permission error: {0}")]
    PermissionError(String),

    #[error("Rate limit error: {0}")]
    RateLimitError(String),

    #[error("Quota exceeded error: {0}")]
    QuotaExceededError(String),

    #[error("Server error: {0}")]
    ServerError(String),

    #[error("Client error: {0}")]
    ClientError(String),

    #[error("Configuration error: {0}")]
    ConfigurationError(String),

    #[error("Resource error: {0}")]
    ResourceError(String),

    #[error("Protocol error: {0}")]
    ProtocolError(String),

    #[error("Version error: {0}")]
    VersionError(String),

    #[error("Feature error: {0}")]
    FeatureError(String),

    #[error("Compatibility error: {0}")]
    CompatibilityError(String),

    #[error("Validation error: {0}")]
    ValidationError(String),

    #[error("Parsing error: {0}")]
    ParsingError(String),

    #[error("Format error: {0}")]
    FormatError(String),

    #[error("Content error: {0}")]
    ContentError(String),

    #[error("Media type error: {0}")]
    MediaTypeError(String),

    #[error("Charset error: {0}")]
    CharsetError(String),

    #[error("Language error: {0}")]
    LanguageError(String),

    #[error("Transfer error: {0}")]
    TransferError(String),

    #[error("Range error: {0}")]
    RangeError(String),

    #[error("Condition error: {0}")]
    ConditionError(String),

    #[error("Precondition error: {0}")]
    PreconditionError(String),

    #[error("Postcondition error: {0}")]
    PostconditionError(String),

    #[error("State error: {0}")]
    StateError(String),

    #[error("Lifecycle error: {0}")]
    LifecycleError(String),

    #[error("Event error: {0}")]
    EventError(String),

    #[error("Callback error: {0}")]
    CallbackError(String),

    #[error("Handler error: {0}")]
    HandlerError(String),

    #[error("Middleware error: {0}")]
    MiddlewareError(String),

    #[error("Plugin error: {0}")]
    PluginError(String),

    #[error("Extension error: {0}")]
    ExtensionError(String),

    #[error("Custom error: {0}")]
    CustomError(String),

    #[error("Unknown error: {0}")]
    Unknown(String),

    #[error("IO error: {0}")]
    IoError(#[from] std::io::Error),

    #[error("UTF-8 error: {0}")]
    Utf8Error(#[from] std::str::Utf8Error),

    #[error("JSON error: {0}")]
    JsonError(#[from] serde_json::Error),

    #[error("URL error: {0}")]
    UrlError(#[from] url_parser::UrlError),

    #[error("Cookie error: {0}")]
    CookieError(String),

    #[error("Base64 error: {0}")]
    Base64Error(#[from] base64::DecodeError),

    #[error("Hex error: {0}")]
    HexError(#[from] hex::FromHexError),

    #[error("Chrono error: {0}")]
    ChronoError(#[from] chrono::ParseError),

    #[error("UUID error: {0}")]
    UuidError(#[from] uuid::Error),

    #[error("Regex error: {0}")]
    RegexError(#[from] regex::Error),

    #[error("MIME error: {0}")]
    MimeError(#[from] mime::FromStrError),

    #[error("Hyper error: {0}")]
    HyperError(#[from] hyper::Error),

    #[error("Rustls error: {0}")]
    RustlsError(#[from] rustls::Error),

    #[error("Tokio error: {0}")]
    TokioError(#[from] tokio::task::JoinError),
}

pub type HttpResult<T> = Result<T, HttpError>;
