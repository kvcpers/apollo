use crate::error::UrlResult;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Url {
    scheme: String,
    username: Option<String>,
    password: Option<String>,
    host: Option<String>,
    port: Option<u16>,
    path: String,
    query: Option<String>,
    fragment: Option<String>,
}

impl Url {
    pub fn new() -> Self {
        Self {
            scheme: String::new(),
            username: None,
            password: None,
            host: None,
            port: None,
            path: String::new(),
            query: None,
            fragment: None,
        }
    }

    pub fn parse(input: &str) -> UrlResult<Self> {
        crate::parser::UrlParser::parse(input)
    }

    pub fn parse_relative(input: &str, base: &Url) -> UrlResult<Self> {
        crate::parser::UrlParser::parse_relative(input, base)
    }

    pub fn scheme(&self) -> &str {
        &self.scheme
    }

    pub fn username(&self) -> Option<&str> {
        self.username.as_deref()
    }

    pub fn password(&self) -> Option<&str> {
        self.password.as_deref()
    }

    pub fn host(&self) -> Option<&str> {
        self.host.as_deref()
    }

    pub fn port(&self) -> Option<u16> {
        self.port
    }

    pub fn path(&self) -> &str {
        &self.path
    }

    pub fn query(&self) -> Option<&str> {
        self.query.as_deref()
    }

    pub fn fragment(&self) -> Option<&str> {
        self.fragment.as_deref()
    }

    pub fn set_scheme(&mut self, scheme: String) {
        self.scheme = scheme;
    }

    pub fn set_username(&mut self, username: Option<String>) {
        self.username = username;
    }

    pub fn set_password(&mut self, password: Option<String>) {
        self.password = password;
    }

    pub fn set_host(&mut self, host: Option<String>) {
        self.host = host;
    }

    pub fn set_port(&mut self, port: Option<u16>) {
        self.port = port;
    }

    pub fn set_path(&mut self, path: String) {
        self.path = path;
    }

    pub fn set_query(&mut self, query: Option<String>) {
        self.query = query;
    }

    pub fn set_fragment(&mut self, fragment: Option<String>) {
        self.fragment = fragment;
    }

    pub fn serialize(&self) -> String {
        let mut result = String::new();

        if !self.scheme.is_empty() {
            result.push_str(&self.scheme);
            result.push(':');
        }

        if let Some(host) = &self.host {
            result.push_str("//");

            if let Some(username) = &self.username {
                result.push_str(username);
                if let Some(password) = &self.password {
                    result.push(':');
                    result.push_str(password);
                }
                result.push('@');
            }

            result.push_str(host);

            if let Some(port) = self.port {
                result.push(':');
                result.push_str(&port.to_string());
            }
        }

        result.push_str(&self.path);

        if let Some(query) = &self.query {
            result.push('?');
            result.push_str(query);
        }

        if let Some(fragment) = &self.fragment {
            result.push('#');
            result.push_str(fragment);
        }

        result
    }

    pub fn origin(&self) -> Option<String> {
        if let Some(host) = &self.host {
            let mut origin = String::new();
            origin.push_str(&self.scheme);
            origin.push_str("://");
            origin.push_str(host);
            if let Some(port) = self.port {
                origin.push(':');
                origin.push_str(&port.to_string());
            }
            Some(origin)
        } else {
            None
        }
    }

    pub fn hostname(&self) -> Option<&str> {
        self.host.as_deref()
    }

    pub fn href(&self) -> String {
        self.to_string()
    }

    pub fn search(&self) -> Option<&str> {
        self.query.as_deref()
    }

    pub fn hash(&self) -> Option<&str> {
        self.fragment.as_deref()
    }

    pub fn is_absolute(&self) -> bool {
        !self.scheme.is_empty()
    }

    pub fn is_relative(&self) -> bool {
        self.scheme.is_empty()
    }

    pub fn is_secure(&self) -> bool {
        matches!(self.scheme.as_str(), "https" | "wss")
    }

    pub fn is_special(&self) -> bool {
        matches!(
            self.scheme.as_str(),
            "http" | "https" | "ws" | "wss" | "ftp" | "file"
        )
    }

    pub fn is_valid(&self) -> bool {
        !self.scheme.is_empty() || !self.path.is_empty()
    }

    pub fn query_params(&self) -> HashMap<String, String> {
        let mut params = HashMap::new();
        if let Some(query) = &self.query {
            for pair in query.split('&') {
                if let Some((key, value)) = pair.split_once('=') {
                    params.insert(key.to_string(), value.to_string());
                }
            }
        }
        params
    }

    pub fn set_query_params(&mut self, params: HashMap<String, String>) {
        if params.is_empty() {
            self.query = None;
        } else {
            let query = params
                .iter()
                .map(|(k, v)| format!("{}={}", k, v))
                .collect::<Vec<_>>()
                .join("&");
            self.query = Some(query);
        }
    }
}

impl Default for Url {
    fn default() -> Self {
        Self::new()
    }
}

impl std::fmt::Display for Url {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.serialize())
    }
}
