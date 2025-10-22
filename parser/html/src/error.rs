use std::fmt;

#[derive(Debug, Clone, PartialEq)]
pub enum HtmlError {
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
    InvalidTagName(String),
    InvalidAttribute(String),
    UnclosedTag(String),
    UnexpectedToken {
        expected: String,
        found: String,
        position: usize,
    },
    IOError(String),
}

impl fmt::Display for HtmlError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            HtmlError::ParseError {
                message,
                line,
                column,
                context,
            } => {
                write!(
                    f,
                    "Parse error at line {}, column {}: {} (context: '{}')",
                    line, column, message, context
                )
            }
            HtmlError::TokenizerError {
                message,
                position,
                context,
            } => {
                write!(
                    f,
                    "Tokenizer error at position {}: {} (context: '{}')",
                    position, message, context
                )
            }
            HtmlError::InvalidTagName(name) => {
                write!(f, "Invalid tag name: '{}'", name)
            }
            HtmlError::InvalidAttribute(attr) => {
                write!(f, "Invalid attribute: '{}'", attr)
            }
            HtmlError::UnclosedTag(tag) => {
                write!(f, "Unclosed tag: '{}'", tag)
            }
            HtmlError::UnexpectedToken {
                expected,
                found,
                position,
            } => {
                write!(
                    f,
                    "Unexpected token at position {}: expected '{}', found '{}'",
                    position, expected, found
                )
            }
            HtmlError::IOError(msg) => {
                write!(f, "IO error: {}", msg)
            }
        }
    }
}

impl std::error::Error for HtmlError {}

pub type HtmlResult<T> = Result<T, HtmlError>;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_error_display() {
        let error = HtmlError::ParseError {
            message: "Expected closing tag".to_string(),
            line: 5,
            column: 10,
            context: "<div>".to_string(),
        };

        let error_str = format!("{}", error);
        assert!(error_str.contains("line 5"));
        assert!(error_str.contains("column 10"));
        assert!(error_str.contains("Expected closing tag"));
    }
}
