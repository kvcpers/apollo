use crate::error::{CssError, CssResult};
use serde::{Deserialize, Serialize};

/// CSS token types
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum CssTokenType {
    Ident(String),
    String(String),
    Hash(String),
    AtKeyword(String),
    Url(String),
    Function(String),
    Number(f32),
    Percentage(f32),
    Dimension(f32, String),
    Whitespace,
    CDO, // <!--
    CDC, // -->
    Colon,
    Semicolon,
    LeftCurlyBracket,
    RightCurlyBracket,
    LeftParenthesis,
    RightParenthesis,
    LeftSquareBracket,
    RightSquareBracket,
    Comma,
    Delim(char),
    EOF,
}

/// CSS token with position information
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CssToken {
    pub token_type: CssTokenType,
    pub line: usize,
    pub column: usize,
    pub position: usize,
}

/// CSS tokenizer implementation
pub struct CssTokenizer {
    input: Vec<char>,
    position: usize,
    line: usize,
    column: usize,
    tokens: Vec<CssToken>,
}

impl CssTokenizer {
    pub fn new(input: &str) -> Self {
        Self {
            input: input.chars().collect(),
            position: 0,
            line: 1,
            column: 1,
            tokens: Vec::new(),
        }
    }

    pub fn tokenize(&mut self) -> CssResult<Vec<CssToken>> {
        while !self.is_eof() {
            self.consume_input()?;
        }

        // Add EOF token
        self.tokens.push(CssToken {
            token_type: CssTokenType::EOF,
            line: self.line,
            column: self.column,
            position: self.position,
        });

        Ok(self.tokens.clone())
    }

    fn is_eof(&self) -> bool {
        self.position >= self.input.len()
    }

    fn current_char(&self) -> Option<char> {
        self.input.get(self.position).copied()
    }

    fn peek_char(&self, offset: usize) -> Option<char> {
        self.input.get(self.position + offset).copied()
    }

    fn consume_char(&mut self) -> Option<char> {
        if let Some(ch) = self.current_char() {
            self.position += 1;
            if ch == '\n' {
                self.line += 1;
                self.column = 1;
            } else {
                self.column += 1;
            }
            Some(ch)
        } else {
            None
        }
    }

    fn consume_input(&mut self) -> CssResult<()> {
        let ch = match self.current_char() {
            Some(c) => c,
            None => return Ok(()),
        };

        match ch {
            ' ' | '\t' | '\n' | '\r' | '\x0C' => {
                self.consume_whitespace();
            }
            '"' | '\'' => {
                self.consume_string()?;
            }
            '#' => {
                self.consume_hash()?;
            }
            '(' => {
                self.emit_token(CssTokenType::LeftParenthesis);
                self.consume_char();
            }
            ')' => {
                self.emit_token(CssTokenType::RightParenthesis);
                self.consume_char();
            }
            '+' | '-' => {
                if self.would_start_number() {
                    self.consume_numeric()?;
                } else {
                    self.emit_token(CssTokenType::Delim(ch));
                    self.consume_char();
                }
            }
            '.' => {
                if self.would_start_number() {
                    self.consume_numeric()?;
                } else {
                    self.emit_token(CssTokenType::Delim(ch));
                    self.consume_char();
                }
            }
            '0'..='9' => {
                self.consume_numeric()?;
            }
            ':' => {
                self.emit_token(CssTokenType::Colon);
                self.consume_char();
            }
            ';' => {
                self.emit_token(CssTokenType::Semicolon);
                self.consume_char();
            }
            '<' => {
                if self.peek_char(1) == Some('!')
                    && self.peek_char(2) == Some('-')
                    && self.peek_char(3) == Some('-')
                {
                    self.consume_char(); // '<'
                    self.consume_char(); // '!'
                    self.consume_char(); // '-'
                    self.consume_char(); // '-'
                    self.emit_token(CssTokenType::CDO);
                } else {
                    self.emit_token(CssTokenType::Delim(ch));
                    self.consume_char();
                }
            }
            '@' => {
                self.consume_at_keyword()?;
            }
            '[' => {
                self.emit_token(CssTokenType::LeftSquareBracket);
                self.consume_char();
            }
            ']' => {
                self.emit_token(CssTokenType::RightSquareBracket);
                self.consume_char();
            }
            '{' => {
                self.emit_token(CssTokenType::LeftCurlyBracket);
                self.consume_char();
            }
            '}' => {
                self.emit_token(CssTokenType::RightCurlyBracket);
                self.consume_char();
            }
            ',' => {
                self.emit_token(CssTokenType::Comma);
                self.consume_char();
            }
            '/' => {
                if self.peek_char(1) == Some('*') {
                    self.consume_comment()?;
                } else {
                    self.emit_token(CssTokenType::Delim(ch));
                    self.consume_char();
                }
            }
            '\\' => {
                self.consume_escape_sequence()?;
            }
            _ => {
                if self.would_start_identifier() {
                    self.consume_identifier()?;
                } else {
                    self.emit_token(CssTokenType::Delim(ch));
                    self.consume_char();
                }
            }
        }

        Ok(())
    }

    fn consume_whitespace(&mut self) {
        while let Some(ch) = self.current_char() {
            if ch.is_whitespace() {
                self.consume_char();
            } else {
                break;
            }
        }
        self.emit_token(CssTokenType::Whitespace);
    }

    fn consume_string(&mut self) -> CssResult<()> {
        let quote = self.consume_char().unwrap();
        let mut value = String::new();

        while let Some(ch) = self.current_char() {
            match ch {
                '\\' => {
                    self.consume_char();
                    if let Some(escaped) = self.consume_escape_sequence_char()? {
                        value.push(escaped);
                    }
                }
                '\n' | '\r' | '\x0C' => {
                    // Unescaped newline in string - parse error
                    return Err(CssError::TokenizerError {
                        message: "Unescaped newline in string".to_string(),
                        position: self.position,
                        context: format!("string: \"{}\"", value),
                    });
                }
                _ if ch == quote => {
                    self.consume_char();
                    break;
                }
                _ => {
                    value.push(ch);
                    self.consume_char();
                }
            }
        }

        self.emit_token(CssTokenType::String(value));
        Ok(())
    }

    fn consume_hash(&mut self) -> CssResult<()> {
        self.consume_char(); // consume '#'
        let mut value = String::new();

        if let Some(ch) = self.current_char() {
            if self.would_start_identifier() {
                while let Some(ch) = self.current_char() {
                    if self.is_identifier_char(ch) {
                        value.push(ch);
                        self.consume_char();
                    } else {
                        break;
                    }
                }
            }
        }

        self.emit_token(CssTokenType::Hash(value));
        Ok(())
    }

    fn consume_numeric(&mut self) -> CssResult<()> {
        let mut repr = String::new();
        let start_pos = self.position;

        // Consume the number part
        if let Some(ch) = self.current_char() {
            if ch == '+' || ch == '-' {
                repr.push(ch);
                self.consume_char();
            }
        }

        while let Some(ch) = self.current_char() {
            if ch.is_ascii_digit() {
                repr.push(ch);
                self.consume_char();
            } else {
                break;
            }
        }

        if let Some(ch) = self.current_char() {
            if ch == '.' {
                repr.push(ch);
                self.consume_char();
                while let Some(ch) = self.current_char() {
                    if ch.is_ascii_digit() {
                        repr.push(ch);
                        self.consume_char();
                    } else {
                        break;
                    }
                }
            }
        }

        let number = match repr.parse::<f32>() {
            Ok(n) => n,
            Err(_) => {
                return Err(CssError::TokenizerError {
                    message: "Invalid number".to_string(),
                    position: start_pos,
                    context: repr,
                });
            }
        };

        // Check if this is a dimension or percentage
        if let Some(ch) = self.current_char() {
            if ch == '%' {
                self.consume_char();
                self.emit_token(CssTokenType::Percentage(number));
                return Ok(());
            } else if self.would_start_identifier() {
                let mut unit = String::new();
                while let Some(ch) = self.current_char() {
                    if self.is_identifier_char(ch) {
                        unit.push(ch);
                        self.consume_char();
                    } else {
                        break;
                    }
                }
                self.emit_token(CssTokenType::Dimension(number, unit));
                return Ok(());
            }
        }

        self.emit_token(CssTokenType::Number(number));
        Ok(())
    }

    fn consume_at_keyword(&mut self) -> CssResult<()> {
        self.consume_char(); // consume '@'
        let mut value = String::new();

        if let Some(ch) = self.current_char() {
            if self.would_start_identifier() {
                while let Some(ch) = self.current_char() {
                    if self.is_identifier_char(ch) {
                        value.push(ch);
                        self.consume_char();
                    } else {
                        break;
                    }
                }
            }
        }

        self.emit_token(CssTokenType::AtKeyword(value));
        Ok(())
    }

    fn consume_identifier(&mut self) -> CssResult<()> {
        let mut value = String::new();

        while let Some(ch) = self.current_char() {
            if self.is_identifier_char(ch) {
                value.push(ch);
                self.consume_char();
            } else {
                break;
            }
        }

        // Check if this is a function
        if let Some(ch) = self.current_char() {
            if ch == '(' {
                self.emit_token(CssTokenType::Function(value));
            } else {
                self.emit_token(CssTokenType::Ident(value));
            }
        } else {
            self.emit_token(CssTokenType::Ident(value));
        }

        Ok(())
    }

    fn consume_escape_sequence(&mut self) -> CssResult<()> {
        self.consume_char(); // consume '\'

        if let Some(escaped) = self.consume_escape_sequence_char()? {
            // Start an identifier with the escaped character
            let mut value = String::new();
            value.push(escaped);

            while let Some(ch) = self.current_char() {
                if self.is_identifier_char(ch) {
                    value.push(ch);
                    self.consume_char();
                } else {
                    break;
                }
            }

            // Check if this is a function
            if let Some(ch) = self.current_char() {
                if ch == '(' {
                    self.emit_token(CssTokenType::Function(value));
                } else {
                    self.emit_token(CssTokenType::Ident(value));
                }
            } else {
                self.emit_token(CssTokenType::Ident(value));
            }
        }

        Ok(())
    }

    fn consume_escape_sequence_char(&mut self) -> CssResult<Option<char>> {
        if let Some(ch) = self.current_char() {
            if ch.is_ascii_hexdigit() {
                // Hexadecimal escape sequence
                let mut hex = String::new();
                let mut count = 0;

                while let Some(ch) = self.current_char() {
                    if ch.is_ascii_hexdigit() && count < 6 {
                        hex.push(ch);
                        self.consume_char();
                        count += 1;
                    } else {
                        break;
                    }
                }

                // Consume optional whitespace
                if let Some(ch) = self.current_char() {
                    if ch.is_whitespace() {
                        self.consume_char();
                    }
                }

                let code_point = match u32::from_str_radix(&hex, 16) {
                    Ok(n) => n,
                    Err(_) => 0xFFFD, // Replacement character
                };

                Ok(std::char::from_u32(code_point))
            } else {
                // Simple escape sequence
                self.consume_char();
                Ok(Some(ch))
            }
        } else {
            Ok(Some('\u{FFFD}')) // Replacement character for EOF
        }
    }

    fn consume_comment(&mut self) -> CssResult<()> {
        self.consume_char(); // consume '/'
        self.consume_char(); // consume '*'

        while let Some(ch) = self.current_char() {
            if ch == '*' && self.peek_char(1) == Some('/') {
                self.consume_char(); // consume '*'
                self.consume_char(); // consume '/'
                break;
            }
            self.consume_char();
        }

        Ok(())
    }

    fn would_start_number(&self) -> bool {
        match self.current_char() {
            Some('+') | Some('-') => {
                if let Some(ch) = self.peek_char(1) {
                    ch.is_ascii_digit()
                        || (ch == '.' && self.peek_char(2).map_or(false, |c| c.is_ascii_digit()))
                } else {
                    false
                }
            }
            Some('.') => self.peek_char(1).map_or(false, |c| c.is_ascii_digit()),
            Some(ch) if ch.is_ascii_digit() => true,
            _ => false,
        }
    }

    fn would_start_identifier(&self) -> bool {
        match self.current_char() {
            Some('\\') => true,
            Some(ch) if self.is_identifier_start(ch) => true,
            _ => false,
        }
    }

    fn is_identifier_start(&self, ch: char) -> bool {
        ch.is_ascii_alphabetic() || ch == '_' || ch as u32 >= 0x80
    }

    fn is_identifier_char(&self, ch: char) -> bool {
        self.is_identifier_start(ch) || ch.is_ascii_digit() || ch == '-'
    }

    fn emit_token(&mut self, token_type: CssTokenType) {
        let token = CssToken {
            token_type,
            line: self.line,
            column: self.column,
            position: self.position,
        };
        self.tokens.push(token);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_simple_tokens() {
        let mut tokenizer = CssTokenizer::new("div { color: red; }");
        let tokens = tokenizer.tokenize().unwrap();

        assert_eq!(tokens.len(), 8); // Including EOF
        assert_eq!(tokens[0].token_type, CssTokenType::Ident("div".to_string()));
        assert_eq!(tokens[1].token_type, CssTokenType::Whitespace);
        assert_eq!(tokens[2].token_type, CssTokenType::LeftCurlyBracket);
        assert_eq!(tokens[3].token_type, CssTokenType::Whitespace);
        assert_eq!(
            tokens[4].token_type,
            CssTokenType::Ident("color".to_string())
        );
        assert_eq!(tokens[5].token_type, CssTokenType::Colon);
        assert_eq!(tokens[6].token_type, CssTokenType::Whitespace);
        assert_eq!(tokens[7].token_type, CssTokenType::Ident("red".to_string()));
    }

    #[test]
    fn test_string_tokens() {
        let mut tokenizer = CssTokenizer::new(r#"content: "Hello World";"#);
        let tokens = tokenizer.tokenize().unwrap();

        assert_eq!(tokens.len(), 5); // Including EOF
        assert_eq!(
            tokens[2].token_type,
            CssTokenType::String("Hello World".to_string())
        );
    }

    #[test]
    fn test_number_tokens() {
        let mut tokenizer = CssTokenizer::new("width: 100px; height: 50%;");
        let tokens = tokenizer.tokenize().unwrap();

        let dimension_token = tokens
            .iter()
            .find(|t| matches!(t.token_type, CssTokenType::Dimension(100.0, _)))
            .unwrap();
        assert_eq!(
            dimension_token.token_type,
            CssTokenType::Dimension(100.0, "px".to_string())
        );

        let percentage_token = tokens
            .iter()
            .find(|t| matches!(t.token_type, CssTokenType::Percentage(50.0)))
            .unwrap();
        assert_eq!(percentage_token.token_type, CssTokenType::Percentage(50.0));
    }

    #[test]
    fn test_hash_tokens() {
        let mut tokenizer = CssTokenizer::new("#container { background: #ff0000; }");
        let tokens = tokenizer.tokenize().unwrap();

        let hash_token = tokens
            .iter()
            .find(|t| matches!(t.token_type, CssTokenType::Hash(_)))
            .unwrap();
        match &hash_token.token_type {
            CssTokenType::Hash(value) => assert_eq!(value, "container"),
            _ => panic!("Expected hash token"),
        }
    }
}
