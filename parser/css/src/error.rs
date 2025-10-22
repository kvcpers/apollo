use std::fmt;

#[derive(Debug, Clone, PartialEq)]
pub enum CssError {
    ParseError {
        message: String,
        line: usize,
        column: usize,
        context: String,
    },
    TokenizerError {
        message: String,
        position: usize,
        context: String,
    },
    InvalidSelector(String),
    InvalidProperty(String),
    InvalidValue(String),
    UnexpectedToken {
        expected: String,
        found: String,
        position: usize,
    },
    IOError(String),
}

impl fmt::Display for CssError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            CssError::ParseError { message, line, column, context } => {
                write!(f, "CSS parse error at line {}, column {}: {} (context: '{}')", 
                       line, column, message, context)
            }
            CssError::TokenizerError { message, position, context } => {
                write!(f, "CSS tokenizer error at position {}: {} (context: '{}')", 
                       position, message, context)
            }
            CssError::InvalidSelector(selector) => {
                write!(f, "Invalid CSS selector: '{}'", selector)
            }
            CssError::InvalidProperty(property) => {
                write!(f, "Invalid CSS property: '{}'", property)
            }
            CssError::InvalidValue(value) => {
                write!(f, "Invalid CSS value: '{}'", value)
            }
            CssError::UnexpectedToken { expected, found, position } => {
                write!(f, "Unexpected CSS token at position {}: expected '{}', found '{}'", 
                       position, expected, found)
            }
            CssError::IOError(msg) => {
                write!(f, "CSS IO error: {}", msg)
            }
        }
    }
}

impl std::error::Error for CssError {}

pub type CssResult<T> = Result<T, CssError>;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_error_display() {
        let error = CssError::ParseError {
            message: "Expected property value".to_string(),
            line: 5,
            column: 10,
            context: "color:".to_string(),
        };
        
        let error_str = format!("{}", error);
        assert!(error_str.contains("line 5"));
        assert!(error_str.contains("column 10"));
        assert!(error_str.contains("Expected property value"));
    }
}
