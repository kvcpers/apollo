use thiserror::Error;

#[derive(Error, Debug)]
pub enum UrlError {
    #[error("Invalid URL: {0}")]
    InvalidUrl(String),

    #[error("Invalid scheme: {0}")]
    InvalidScheme(String),

    #[error("Invalid host: {0}")]
    InvalidHost(String),

    #[error("Invalid port: {0}")]
    InvalidPort(String),

    #[error("Invalid path: {0}")]
    InvalidPath(String),

    #[error("Invalid query: {0}")]
    InvalidQuery(String),

    #[error("Invalid fragment: {0}")]
    InvalidFragment(String),

    #[error("Invalid encoding: {0}")]
    InvalidEncoding(String),

    #[error("Invalid character: {0}")]
    InvalidCharacter(char),

    #[error("Invalid format: {0}")]
    InvalidFormat(String),

    #[error("Missing required component: {0}")]
    MissingRequired(String),

    #[error("Unsupported operation: {0}")]
    UnsupportedOperation(String),

    #[error("Parse error: {0}")]
    ParseError(String),

    #[error("IO error: {0}")]
    IoError(#[from] std::io::Error),

    #[error("UTF-8 error: {0}")]
    Utf8Error(#[from] std::str::Utf8Error),

    #[error("Unknown error: {0}")]
    Unknown(String),
}

pub type UrlResult<T> = Result<T, UrlError>;
