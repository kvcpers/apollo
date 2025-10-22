use crate::error::{HttpError, HttpResult};
use crate::response::HttpResponse;
use crate::headers::HttpHeaders;
use crate::client::HttpClient;
use serde::Serialize;
use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
pub enum HttpMethod {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT,
}

impl HttpMethod {
    pub fn as_str(&self) -> &'static str {
        match self {
            HttpMethod::GET => "GET",
            HttpMethod::POST => "POST",
            HttpMethod::PUT => "PUT",
            HttpMethod::DELETE => "DELETE",
            HttpMethod::PATCH => "PATCH",
            HttpMethod::HEAD => "HEAD",
            HttpMethod::OPTIONS => "OPTIONS",
            HttpMethod::TRACE => "TRACE",
            HttpMethod::CONNECT => "CONNECT",
        }
    }
}

pub struct HttpRequest {
    method: HttpMethod,
    url: String,
    headers: HttpHeaders,
    body: Option<Vec<u8>>,
    client: HttpClient,
    timeout: Option<std::time::Duration>,
}

impl HttpRequest {
    pub fn new(method: HttpMethod, url: &str, client: HttpClient) -> Self {
        Self {
            method,
            url: url.to_string(),
            headers: HttpHeaders::new(),
            body: None,
            client,
            timeout: None,
        }
    }

    pub fn header(mut self, name: &str, value: &str) -> Self {
        self.headers.set(name, value);
        self
    }

    pub fn headers(mut self, headers: HashMap<String, String>) -> Self {
        for (name, value) in headers {
            self.headers.set(&name, &value);
        }
        self
    }

    pub fn json<T: Serialize>(mut self, data: &T) -> HttpResult<Self> {
        let json = serde_json::to_vec(data)?;
        self.headers.set("Content-Type", "application/json");
        self.body = Some(json);
        Ok(self)
    }

    pub fn form(mut self, data: &HashMap<String, String>) -> Self {
        let form_data = data.iter()
            .map(|(k, v)| format!("{}={}", k, v))
            .collect::<Vec<_>>()
            .join("&");
        self.headers.set("Content-Type", "application/x-www-form-urlencoded");
        self.body = Some(form_data.into_bytes());
        self
    }

    pub fn body(mut self, body: Vec<u8>) -> Self {
        self.body = Some(body);
        self
    }

    pub fn timeout(mut self, timeout: std::time::Duration) -> Self {
        self.timeout = Some(timeout);
        self
    }

    pub async fn send(self) -> HttpResult<HttpResponse> {
        // This is a simplified implementation
        // In a real implementation, this would use hyper or reqwest
        Ok(HttpResponse::new(200, HttpHeaders::new(), vec![]))
    }

    pub fn method(&self) -> &HttpMethod {
        &self.method
    }

    pub fn url(&self) -> &str {
        &self.url
    }

    pub fn get_headers(&self) -> &HttpHeaders {
        &self.headers
    }

    pub fn get_body(&self) -> Option<&Vec<u8>> {
        self.body.as_ref()
    }

    pub fn get_timeout(&self) -> Option<std::time::Duration> {
        self.timeout
    }
}
