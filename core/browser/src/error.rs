use std::fmt;

#[derive(Debug, Clone, PartialEq)]
pub enum BrowserError {
    ParseError(String),
    NetworkError(String),
    IOError(String),
    InitializationError(String),
    RuntimeError(String),
    SecurityError(String),
    NotImplemented(String),
}

impl fmt::Display for BrowserError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            BrowserError::ParseError(msg) => write!(f, "Parse error: {}", msg),
            BrowserError::NetworkError(msg) => write!(f, "Network error: {}", msg),
            BrowserError::IOError(msg) => write!(f, "IO error: {}", msg),
            BrowserError::InitializationError(msg) => write!(f, "Initialization error: {}", msg),
            BrowserError::RuntimeError(msg) => write!(f, "Runtime error: {}", msg),
            BrowserError::SecurityError(msg) => write!(f, "Security error: {}", msg),
            BrowserError::NotImplemented(msg) => write!(f, "Not implemented: {}", msg),
        }
    }
}

impl std::error::Error for BrowserError {}

pub type BrowserResult<T> = Result<T, BrowserError>;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_error_display() {
        let error = BrowserError::ParseError("Invalid HTML".to_string());
        let error_str = format!("{}", error);
        assert!(error_str.contains("Parse error"));
        assert!(error_str.contains("Invalid HTML"));
    }
}
