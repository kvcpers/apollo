use std::collections::VecDeque;
use crate::error::{HtmlError, HtmlResult};

/// HTML token types
#[derive(Debug, Clone, PartialEq)]
pub enum TokenType {
    StartTag {
        name: String,
        attributes: Vec<(String, String)>,
        self_closing: bool,
    },
    EndTag(String),
    Text(String),
    Comment(String),
    Doctype(String),
    Character(String),
    EOF,
}

/// HTML token with position information
#[derive(Debug, Clone)]
pub struct Token {
    pub token_type: TokenType,
    pub line: usize,
    pub column: usize,
    pub position: usize,
}

/// HTML tokenizer state machine
#[derive(Debug, Clone, PartialEq)]
enum TokenizerState {
    Data,
    TagOpen,
    EndTagOpen,
    TagName,
    BeforeAttributeName,
    AttributeName,
    AfterAttributeName,
    BeforeAttributeValue,
    AttributeValueDoubleQuoted,
    AttributeValueSingleQuoted,
    AttributeValueUnquoted,
    AfterAttributeValueQuoted,
    SelfClosingStartTag,
    BogusComment,
    MarkupDeclarationOpen,
    CommentStart,
    CommentStartDash,
    Comment,
    CommentEndDash,
    CommentEnd,
    CommentEndBang,
    Doctype,
    BeforeDoctypeName,
    DoctypeName,
    AfterDoctypeName,
    AfterDoctypePublicKeyword,
    BeforeDoctypePublicIdentifier,
    DoctypePublicIdentifierDoubleQuoted,
    DoctypePublicIdentifierSingleQuoted,
    AfterDoctypePublicIdentifier,
    BetweenDoctypePublicAndSystemIdentifiers,
    AfterDoctypeSystemKeyword,
    BeforeDoctypeSystemIdentifier,
    DoctypeSystemIdentifierDoubleQuoted,
    DoctypeSystemIdentifierSingleQuoted,
    AfterDoctypeSystemIdentifier,
    BogusDoctype,
    CharacterReference,
}

/// HTML tokenizer implementation
pub struct HtmlTokenizer {
    input: Vec<char>,
    position: usize,
    line: usize,
    column: usize,
    state: TokenizerState,
    current_token: Option<Token>,
    tokens: VecDeque<Token>,
    buffer: String,
    current_attribute_name: String,
    current_attribute_value: String,
    temp_buffer: String,
}

impl HtmlTokenizer {
    pub fn new(input: &str) -> Self {
        Self {
            input: input.chars().collect(),
            position: 0,
            line: 1,
            column: 1,
            state: TokenizerState::Data,
            current_token: None,
            tokens: VecDeque::new(),
            buffer: String::new(),
            current_attribute_name: String::new(),
            current_attribute_value: String::new(),
            temp_buffer: String::new(),
        }
    }

    pub fn tokenize(&mut self) -> HtmlResult<Vec<Token>> {
        while !self.is_eof() {
            self.consume_input()?;
        }
        
        // Add EOF token
        self.tokens.push_back(Token {
            token_type: TokenType::EOF,
            line: self.line,
            column: self.column,
            position: self.position,
        });

        Ok(self.tokens.drain(..).collect())
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

    fn consume_input(&mut self) -> HtmlResult<()> {
        match self.state {
            TokenizerState::Data => self.state_data(),
            TokenizerState::TagOpen => self.state_tag_open(),
            TokenizerState::EndTagOpen => self.state_end_tag_open(),
            TokenizerState::TagName => self.state_tag_name(),
            TokenizerState::BeforeAttributeName => self.state_before_attribute_name(),
            TokenizerState::AttributeName => self.state_attribute_name(),
            TokenizerState::AfterAttributeName => self.state_after_attribute_name(),
            TokenizerState::BeforeAttributeValue => self.state_before_attribute_value(),
            TokenizerState::AttributeValueDoubleQuoted => self.state_attribute_value_double_quoted(),
            TokenizerState::AttributeValueSingleQuoted => self.state_attribute_value_single_quoted(),
            TokenizerState::AttributeValueUnquoted => self.state_attribute_value_unquoted(),
            TokenizerState::AfterAttributeValueQuoted => self.state_after_attribute_value_quoted(),
            TokenizerState::SelfClosingStartTag => self.state_self_closing_start_tag(),
            TokenizerState::CommentStart => self.state_comment_start(),
            TokenizerState::CommentStartDash => self.state_comment_start_dash(),
            TokenizerState::Comment => self.state_comment(),
            TokenizerState::CommentEndDash => self.state_comment_end_dash(),
            TokenizerState::CommentEnd => self.state_comment_end(),
            TokenizerState::CommentEndBang => self.state_comment_end_bang(),
            TokenizerState::Doctype => self.state_doctype(),
            TokenizerState::BeforeDoctypeName => self.state_before_doctype_name(),
            TokenizerState::DoctypeName => self.state_doctype_name(),
            _ => {
                // Simplified implementation - handle other states as needed
                self.consume_char();
                self.state = TokenizerState::Data;
                Ok(())
            }
        }
    }

    fn state_data(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('<') => {
                self.consume_char();
                self.state = TokenizerState::TagOpen;
            }
            Some(_) => {
                self.buffer.clear();
                while let Some(ch) = self.current_char() {
                    if ch == '<' {
                        break;
                    }
                    self.buffer.push(ch);
                    self.consume_char();
                }
                if !self.buffer.is_empty() {
                    self.emit_token(TokenType::Text(self.buffer.clone()));
                }
            }
            None => {
                if !self.buffer.is_empty() {
                    self.emit_token(TokenType::Text(self.buffer.clone()));
                }
            }
        }
        Ok(())
    }

    fn state_tag_open(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('!') => {
                self.consume_char();
                self.state = TokenizerState::MarkupDeclarationOpen;
            }
            Some('/') => {
                self.consume_char();
                self.state = TokenizerState::EndTagOpen;
            }
            Some(ch) if ch.is_ascii_alphabetic() => {
                self.buffer.clear();
                self.buffer.push(ch.to_ascii_lowercase());
                self.consume_char();
                self.state = TokenizerState::TagName;
            }
            Some('?') => {
                // Processing instruction - treat as comment
                self.consume_char();
                self.buffer.clear();
                while let Some(ch) = self.current_char() {
                    if ch == '?' && self.peek_char(1) == Some('>') {
                        self.consume_char();
                        self.consume_char();
                        break;
                    }
                    self.buffer.push(ch);
                    self.consume_char();
                }
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                // Parse error - treat as character
                self.consume_char();
                self.emit_token(TokenType::Character("<".to_string()));
                self.state = TokenizerState::Data;
            }
            None => {
                // EOF - emit character
                self.emit_token(TokenType::Character("<".to_string()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_end_tag_open(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_ascii_alphabetic() => {
                self.buffer.clear();
                self.buffer.push(ch.to_ascii_lowercase());
                self.consume_char();
                self.state = TokenizerState::TagName;
            }
            Some('>') => {
                // Parse error - invalid end tag
                self.consume_char();
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                // Parse error - treat as bogus comment
                self.consume_char();
                self.state = TokenizerState::BogusComment;
            }
            None => {
                // EOF - treat as bogus comment
                self.state = TokenizerState::BogusComment;
            }
        }
        Ok(())
    }

    fn state_tag_name(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_ascii_alphanumeric() => {
                self.buffer.push(ch.to_ascii_lowercase());
                self.consume_char();
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: Vec::new(),
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
            Some('/') => {
                self.consume_char();
                self.state = TokenizerState::SelfClosingStartTag;
            }
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
                self.state = TokenizerState::BeforeAttributeName;
            }
            Some(_) => {
                // Parse error - treat as attribute name
                self.current_attribute_name = self.buffer.clone();
                self.buffer.clear();
                self.state = TokenizerState::BeforeAttributeName;
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: Vec::new(),
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_before_attribute_name(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: Vec::new(),
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
            Some('/') => {
                self.consume_char();
                self.state = TokenizerState::SelfClosingStartTag;
            }
            Some(_) => {
                self.current_attribute_name.clear();
                self.state = TokenizerState::AttributeName;
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: Vec::new(),
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_attribute_name(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_ascii_alphanumeric() || ch == '-' || ch == '_' || ch == ':' => {
                self.current_attribute_name.push(ch.to_ascii_lowercase());
                self.consume_char();
            }
            Some('=') => {
                self.consume_char();
                self.state = TokenizerState::BeforeAttributeValue;
            }
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
                self.state = TokenizerState::AfterAttributeName;
            }
            Some('>') => {
                self.consume_char();
                // Emit attribute without value
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), String::new())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                // Parse error - treat as attribute name
                let ch = self.consume_char().unwrap();
                self.current_attribute_name.push(ch);
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), String::new())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_after_attribute_name(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
            }
            Some('=') => {
                self.consume_char();
                self.state = TokenizerState::BeforeAttributeValue;
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), String::new())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                // Parse error - treat as attribute name
                self.current_attribute_name.clear();
                self.state = TokenizerState::AttributeName;
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), String::new())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_before_attribute_value(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
            }
            Some('"') => {
                self.consume_char();
                self.current_attribute_value.clear();
                self.state = TokenizerState::AttributeValueDoubleQuoted;
            }
            Some('\'') => {
                self.consume_char();
                self.current_attribute_value.clear();
                self.state = TokenizerState::AttributeValueSingleQuoted;
            }
            Some(_) => {
                self.current_attribute_value.clear();
                self.state = TokenizerState::AttributeValueUnquoted;
            }
            None => {
                // EOF - treat as empty value
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), String::new())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_attribute_value_double_quoted(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('"') => {
                self.consume_char();
                self.state = TokenizerState::AfterAttributeValueQuoted;
            }
            Some('&') => {
                // Handle character reference
                let ch = self.consume_char().unwrap();
                self.current_attribute_value.push(ch);
            }
            Some(_) => {
                let ch = self.consume_char().unwrap();
                self.current_attribute_value.push(ch);
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_attribute_value_single_quoted(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('\'') => {
                self.consume_char();
                self.state = TokenizerState::AfterAttributeValueQuoted;
            }
            Some('&') => {
                // Handle character reference
                let ch = self.consume_char().unwrap();
                self.current_attribute_value.push(ch);
            }
            Some(_) => {
                let ch = self.consume_char().unwrap();
                self.current_attribute_value.push(ch);
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_attribute_value_unquoted(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
                self.state = TokenizerState::BeforeAttributeName;
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
            Some('&') => {
                // Handle character reference
                let ch = self.consume_char().unwrap();
                self.current_attribute_value.push(ch);
            }
            Some(_) => {
                let ch = self.consume_char().unwrap();
                self.current_attribute_value.push(ch);
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_after_attribute_value_quoted(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
                self.state = TokenizerState::BeforeAttributeName;
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
            Some('/') => {
                self.consume_char();
                self.state = TokenizerState::SelfClosingStartTag;
            }
            Some(_) => {
                // Parse error - treat as attribute name
                self.current_attribute_name.clear();
                self.state = TokenizerState::AttributeName;
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: false,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_self_closing_start_tag(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: true,
                });
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                // Parse error - treat as attribute name
                self.current_attribute_name.clear();
                self.state = TokenizerState::AttributeName;
            }
            None => {
                // EOF
                self.emit_token(TokenType::StartTag {
                    name: self.buffer.clone(),
                    attributes: vec![(self.current_attribute_name.clone(), self.current_attribute_value.clone())],
                    self_closing: true,
                });
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_comment_start(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('-') => {
                self.consume_char();
                self.state = TokenizerState::CommentStartDash;
            }
            Some('>') => {
                // Parse error - empty comment
                self.consume_char();
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                self.buffer.clear();
                self.state = TokenizerState::Comment;
            }
            None => {
                // EOF
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_comment_start_dash(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('-') => {
                self.consume_char();
                self.buffer.clear();
                self.state = TokenizerState::Comment;
            }
            Some('>') => {
                // Parse error - empty comment
                self.consume_char();
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                self.buffer.push('-');
                self.state = TokenizerState::Comment;
            }
            None => {
                // EOF
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_comment(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('-') => {
                self.consume_char();
                self.state = TokenizerState::CommentEndDash;
            }
            Some('&') => {
                // Handle character reference
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
            }
            Some(_) => {
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
            }
            None => {
                // EOF - emit comment
                self.emit_token(TokenType::Comment(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_comment_end_dash(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('-') => {
                self.consume_char();
                self.state = TokenizerState::CommentEnd;
            }
            Some('&') => {
                self.buffer.push('-');
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
                self.state = TokenizerState::Comment;
            }
            Some(_) => {
                self.buffer.push('-');
                self.state = TokenizerState::Comment;
            }
            None => {
                // EOF
                self.emit_token(TokenType::Comment(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_comment_end(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::Comment(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
            Some('!') => {
                self.consume_char();
                self.state = TokenizerState::CommentEndBang;
            }
            Some('-') => {
                self.buffer.push('-');
                self.consume_char();
            }
            Some('&') => {
                self.buffer.push('-');
                self.buffer.push('-');
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
                self.state = TokenizerState::Comment;
            }
            Some(_) => {
                self.buffer.push('-');
                self.buffer.push('-');
                self.state = TokenizerState::Comment;
            }
            None => {
                // EOF
                self.emit_token(TokenType::Comment(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_comment_end_bang(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some('-') => {
                self.buffer.push('-');
                self.buffer.push('-');
                self.buffer.push('!');
                self.consume_char();
                self.state = TokenizerState::CommentEndDash;
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::Comment(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
            Some('&') => {
                self.buffer.push('-');
                self.buffer.push('-');
                self.buffer.push('!');
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
                self.state = TokenizerState::Comment;
            }
            Some(_) => {
                self.buffer.push('-');
                self.buffer.push('-');
                self.buffer.push('!');
                self.state = TokenizerState::Comment;
            }
            None => {
                // EOF
                self.emit_token(TokenType::Comment(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_markup_declaration_open(&mut self) -> HtmlResult<()> {
        // Simplified - handle DOCTYPE and comments
        if self.peek_char(0) == Some('-') && self.peek_char(1) == Some('-') {
            self.consume_char();
            self.consume_char();
            self.buffer.clear();
            self.state = TokenizerState::CommentStart;
        } else {
            // Assume DOCTYPE
            self.state = TokenizerState::Doctype;
        }
        Ok(())
    }

    fn state_doctype(&mut self) -> HtmlResult<()> {
        // Simplified DOCTYPE parsing
        self.buffer.clear();
        while let Some(ch) = self.current_char() {
            if ch == '>' {
                self.consume_char();
                break;
            }
            self.buffer.push(ch);
            self.consume_char();
        }
        self.emit_token(TokenType::Doctype(self.buffer.clone()));
        self.state = TokenizerState::Data;
        Ok(())
    }

    fn state_before_doctype_name(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
            }
            Some(ch) if ch.is_ascii_alphabetic() => {
                self.buffer.clear();
                self.buffer.push(ch.to_ascii_uppercase());
                self.consume_char();
                self.state = TokenizerState::DoctypeName;
            }
            Some('>') => {
                // Parse error - no DOCTYPE name
                self.consume_char();
                self.emit_token(TokenType::Doctype(String::new()));
                self.state = TokenizerState::Data;
            }
            Some(_) => {
                // Parse error - treat as DOCTYPE name
                self.buffer.clear();
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
                self.state = TokenizerState::DoctypeName;
            }
            None => {
                // EOF
                self.emit_token(TokenType::Doctype(String::new()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn state_doctype_name(&mut self) -> HtmlResult<()> {
        match self.current_char() {
            Some(ch) if ch.is_whitespace() => {
                self.consume_char();
                self.state = TokenizerState::AfterDoctypeName;
            }
            Some('>') => {
                self.consume_char();
                self.emit_token(TokenType::Doctype(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
            Some(ch) if ch.is_ascii_alphabetic() => {
                self.buffer.push(ch.to_ascii_uppercase());
                self.consume_char();
            }
            Some(_) => {
                // Parse error - treat as DOCTYPE name
                let ch = self.consume_char().unwrap();
                self.buffer.push(ch);
            }
            None => {
                // EOF
                self.emit_token(TokenType::Doctype(self.buffer.clone()));
                self.state = TokenizerState::Data;
            }
        }
        Ok(())
    }

    fn emit_token(&mut self, token_type: TokenType) {
        let token = Token {
            token_type,
            line: self.line,
            column: self.column,
            position: self.position,
        };
        self.tokens.push_back(token);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_simple_text() {
        let mut tokenizer = HtmlTokenizer::new("Hello World");
        let tokens = tokenizer.tokenize().unwrap();
        
        assert_eq!(tokens.len(), 2);
        assert_eq!(tokens[0].token_type, TokenType::Text("Hello World".to_string()));
        assert_eq!(tokens[1].token_type, TokenType::EOF);
    }

    #[test]
    fn test_simple_tag() {
        let mut tokenizer = HtmlTokenizer::new("<div>content</div>");
        let tokens = tokenizer.tokenize().unwrap();
        
        assert_eq!(tokens.len(), 4);
        assert_eq!(tokens[0].token_type, TokenType::StartTag {
            name: "div".to_string(),
            attributes: vec![],
            self_closing: false,
        });
        assert_eq!(tokens[1].token_type, TokenType::Text("content".to_string()));
        assert_eq!(tokens[2].token_type, TokenType::EndTag("div".to_string()));
        assert_eq!(tokens[3].token_type, TokenType::EOF);
    }

    #[test]
    fn test_tag_with_attributes() {
        let mut tokenizer = HtmlTokenizer::new(r#"<div id="test" class="container">content</div>"#);
        let tokens = tokenizer.tokenize().unwrap();
        
        assert_eq!(tokens.len(), 4);
        match &tokens[0].token_type {
            TokenType::StartTag { name, attributes, self_closing } => {
                assert_eq!(name, "div");
                assert_eq!(attributes.len(), 2);
                assert_eq!(attributes[0], ("id".to_string(), "test".to_string()));
                assert_eq!(attributes[1], ("class".to_string(), "container".to_string()));
                assert!(!self_closing);
            }
            _ => panic!("Expected StartTag"),
        }
    }

    #[test]
    fn test_comment() {
        let mut tokenizer = HtmlTokenizer::new("<!-- This is a comment -->");
        let tokens = tokenizer.tokenize().unwrap();
        
        assert_eq!(tokens.len(), 2);
        assert_eq!(tokens[0].token_type, TokenType::Comment(" This is a comment ".to_string()));
        assert_eq!(tokens[1].token_type, TokenType::EOF);
    }

    #[test]
    fn test_doctype() {
        let mut tokenizer = HtmlTokenizer::new("<!DOCTYPE html>");
        let tokens = tokenizer.tokenize().unwrap();
        
        assert_eq!(tokens.len(), 2);
        assert_eq!(tokens[0].token_type, TokenType::Doctype("DOCTYPE html".to_string()));
        assert_eq!(tokens[1].token_type, TokenType::EOF);
    }
}
