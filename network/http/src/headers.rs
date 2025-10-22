use std::collections::HashMap;
use serde::{Serialize, Deserialize};

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct HttpHeaders {
    headers: HashMap<String, String>,
}

impl HttpHeaders {
    pub fn new() -> Self {
        Self {
            headers: HashMap::new(),
        }
    }

    pub fn set(&mut self, name: &str, value: &str) {
        self.headers.insert(name.to_lowercase(), value.to_string());
    }

    pub fn get(&self, name: &str) -> Option<&str> {
        self.headers.get(&name.to_lowercase()).map(|s| s.as_str())
    }

    pub fn remove(&mut self, name: &str) -> Option<String> {
        self.headers.remove(&name.to_lowercase())
    }

    pub fn contains(&self, name: &str) -> bool {
        self.headers.contains_key(&name.to_lowercase())
    }

    pub fn clear(&mut self) {
        self.headers.clear();
    }

    pub fn len(&self) -> usize {
        self.headers.len()
    }

    pub fn is_empty(&self) -> bool {
        self.headers.is_empty()
    }

    pub fn iter(&self) -> impl Iterator<Item = (&String, &String)> {
        self.headers.iter()
    }

    pub fn keys(&self) -> impl Iterator<Item = &String> {
        self.headers.keys()
    }

    pub fn values(&self) -> impl Iterator<Item = &String> {
        self.headers.values()
    }

    pub fn to_vec(&self) -> Vec<(String, String)> {
        self.headers.iter().map(|(k, v)| (k.clone(), v.clone())).collect()
    }

    pub fn from_vec(headers: Vec<(String, String)>) -> Self {
        let mut http_headers = Self::new();
        for (name, value) in headers {
            http_headers.set(&name, &value);
        }
        http_headers
    }

    pub fn from_map(headers: HashMap<String, String>) -> Self {
        Self { headers }
    }

    pub fn to_map(&self) -> HashMap<String, String> {
        self.headers.clone()
    }

    pub fn merge(&mut self, other: &HttpHeaders) {
        for (name, value) in other.iter() {
            self.set(name, value);
        }
    }

    pub fn merge_into(mut self, other: &HttpHeaders) -> Self {
        self.merge(other);
        self
    }
}

impl Default for HttpHeaders {
    fn default() -> Self {
        Self::new()
    }
}

impl From<HashMap<String, String>> for HttpHeaders {
    fn from(headers: HashMap<String, String>) -> Self {
        Self::from_map(headers)
    }
}

impl Into<HashMap<String, String>> for HttpHeaders {
    fn into(self) -> HashMap<String, String> {
        self.to_map()
    }
}

impl std::fmt::Display for HttpHeaders {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for (name, value) in self.iter() {
            writeln!(f, "{}: {}", name, value)?;
        }
        Ok(())
    }
}
