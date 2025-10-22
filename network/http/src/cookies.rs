use crate::error::{HttpError, HttpResult};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::time::{SystemTime, UNIX_EPOCH};
use url_parser::Url;

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct Cookie {
    name: String,
    value: String,
    domain: Option<String>,
    path: Option<String>,
    expires: Option<SystemTime>,
    max_age: Option<u64>,
    secure: bool,
    http_only: bool,
    same_site: Option<SameSite>,
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum SameSite {
    Strict,
    Lax,
    None,
}

impl Cookie {
    pub fn new(name: String, value: String) -> Self {
        Self {
            name,
            value,
            domain: None,
            path: None,
            expires: None,
            max_age: None,
            secure: false,
            http_only: false,
            same_site: None,
        }
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn value(&self) -> &str {
        &self.value
    }

    pub fn domain(&self) -> Option<&str> {
        self.domain.as_deref()
    }

    pub fn path(&self) -> Option<&str> {
        self.path.as_deref()
    }

    pub fn expires(&self) -> Option<SystemTime> {
        self.expires
    }

    pub fn max_age(&self) -> Option<u64> {
        self.max_age
    }

    pub fn is_secure(&self) -> bool {
        self.secure
    }

    pub fn is_http_only(&self) -> bool {
        self.http_only
    }

    pub fn same_site(&self) -> Option<&SameSite> {
        self.same_site.as_ref()
    }

    pub fn set_domain(&mut self, domain: Option<String>) {
        self.domain = domain;
    }

    pub fn set_path(&mut self, path: Option<String>) {
        self.path = path;
    }

    pub fn set_expires(&mut self, expires: Option<SystemTime>) {
        self.expires = expires;
    }

    pub fn set_max_age(&mut self, max_age: Option<u64>) {
        self.max_age = max_age;
    }

    pub fn set_secure(&mut self, secure: bool) {
        self.secure = secure;
    }

    pub fn set_http_only(&mut self, http_only: bool) {
        self.http_only = http_only;
    }

    pub fn set_same_site(&mut self, same_site: Option<SameSite>) {
        self.same_site = same_site;
    }

    pub fn is_expired(&self) -> bool {
        if let Some(expires) = self.expires {
            SystemTime::now() > expires
        } else if let Some(max_age) = self.max_age {
            // This is a simplified implementation
            // In a real implementation, we would track when the cookie was set
            false
        } else {
            false
        }
    }

    pub fn matches_domain(&self, domain: &str) -> bool {
        if let Some(cookie_domain) = &self.domain {
            domain.ends_with(cookie_domain)
        } else {
            true
        }
    }

    pub fn matches_path(&self, path: &str) -> bool {
        if let Some(cookie_path) = &self.path {
            path.starts_with(cookie_path)
        } else {
            true
        }
    }

    pub fn to_string(&self) -> String {
        format!("{}={}", self.name, self.value)
    }

    pub fn to_set_cookie_string(&self) -> String {
        let mut result = format!("{}={}", self.name, self.value);

        if let Some(domain) = &self.domain {
            result.push_str(&format!("; Domain={}", domain));
        }

        if let Some(path) = &self.path {
            result.push_str(&format!("; Path={}", path));
        }

        if let Some(expires) = self.expires {
            // This is a simplified implementation
            // In a real implementation, we would format the date properly
            result.push_str("; Expires=Thu, 01 Jan 2038 00:00:00 GMT");
        }

        if let Some(max_age) = self.max_age {
            result.push_str(&format!("; Max-Age={}", max_age));
        }

        if self.secure {
            result.push_str("; Secure");
        }

        if self.http_only {
            result.push_str("; HttpOnly");
        }

        if let Some(same_site) = &self.same_site {
            match same_site {
                SameSite::Strict => result.push_str("; SameSite=Strict"),
                SameSite::Lax => result.push_str("; SameSite=Lax"),
                SameSite::None => result.push_str("; SameSite=None"),
            }
        }

        result
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CookieJar {
    cookies: HashMap<String, Cookie>,
}

impl CookieJar {
    pub fn new() -> Self {
        Self {
            cookies: HashMap::new(),
        }
    }

    pub fn set(&mut self, cookie: Cookie) {
        self.cookies.insert(cookie.name().to_string(), cookie);
    }

    pub fn get(&self, name: &str) -> Option<&Cookie> {
        self.cookies.get(name)
    }

    pub fn remove(&mut self, name: &str) -> Option<Cookie> {
        self.cookies.remove(name)
    }

    pub fn clear(&mut self) {
        self.cookies.clear();
    }

    pub fn len(&self) -> usize {
        self.cookies.len()
    }

    pub fn is_empty(&self) -> bool {
        self.cookies.is_empty()
    }

    pub fn iter(&self) -> impl Iterator<Item = (&String, &Cookie)> {
        self.cookies.iter()
    }

    pub fn keys(&self) -> impl Iterator<Item = &String> {
        self.cookies.keys()
    }

    pub fn values(&self) -> impl Iterator<Item = &Cookie> {
        self.cookies.values()
    }

    pub fn parse_set_cookie(&mut self, set_cookie: &str) -> HttpResult<()> {
        let parts: Vec<&str> = set_cookie.split(';').collect();
        if parts.is_empty() {
            return Err(HttpError::InvalidCookie(
                "Empty Set-Cookie header".to_string(),
            ));
        }

        let name_value = parts[0].trim();
        if let Some((name, value)) = name_value.split_once('=') {
            let mut cookie = Cookie::new(name.to_string(), value.to_string());

            for part in &parts[1..] {
                let part = part.trim();
                if let Some((key, value)) = part.split_once('=') {
                    match key.to_lowercase().as_str() {
                        "domain" => cookie.set_domain(Some(value.to_string())),
                        "path" => cookie.set_path(Some(value.to_string())),
                        "max-age" => {
                            if let Ok(max_age) = value.parse::<u64>() {
                                cookie.set_max_age(Some(max_age));
                            }
                        }
                        _ => {}
                    }
                } else {
                    match part.to_lowercase().as_str() {
                        "secure" => cookie.set_secure(true),
                        "httponly" => cookie.set_http_only(true),
                        "samesite=strict" => cookie.set_same_site(Some(SameSite::Strict)),
                        "samesite=lax" => cookie.set_same_site(Some(SameSite::Lax)),
                        "samesite=none" => cookie.set_same_site(Some(SameSite::None)),
                        _ => {}
                    }
                }
            }

            self.set(cookie);
            Ok(())
        } else {
            Err(HttpError::InvalidCookie(
                "Invalid cookie format".to_string(),
            ))
        }
    }

    pub fn parse_set_cookie_with_url(&mut self, set_cookie: &str, url: &Url) -> HttpResult<()> {
        let parts: Vec<&str> = set_cookie.split(';').collect();
        if parts.is_empty() {
            return Err(HttpError::InvalidCookie(
                "Empty Set-Cookie header".to_string(),
            ));
        }

        let name_value = parts[0].trim();
        if let Some((name, value)) = name_value.split_once('=') {
            let mut cookie = Cookie::new(name.to_string(), value.to_string());

            // Set default domain and path from URL
            if let Some(host) = url.host() {
                cookie.set_domain(Some(host.to_string()));
            }
            cookie.set_path(Some(url.path().to_string()));

            for part in &parts[1..] {
                let part = part.trim();
                if let Some((key, value)) = part.split_once('=') {
                    match key.to_lowercase().as_str() {
                        "domain" => cookie.set_domain(Some(value.to_string())),
                        "path" => cookie.set_path(Some(value.to_string())),
                        "max-age" => {
                            if let Ok(max_age) = value.parse::<u64>() {
                                cookie.set_max_age(Some(max_age));
                            }
                        }
                        _ => {}
                    }
                } else {
                    match part.to_lowercase().as_str() {
                        "secure" => cookie.set_secure(true),
                        "httponly" => cookie.set_http_only(true),
                        "samesite=strict" => cookie.set_same_site(Some(SameSite::Strict)),
                        "samesite=lax" => cookie.set_same_site(Some(SameSite::Lax)),
                        "samesite=none" => cookie.set_same_site(Some(SameSite::None)),
                        _ => {}
                    }
                }
            }

            self.set(cookie);
            Ok(())
        } else {
            Err(HttpError::InvalidCookie(
                "Invalid cookie format".to_string(),
            ))
        }
    }

    pub fn get_cookies_for_url(&self, url: &Url) -> Vec<String> {
        let mut cookies = Vec::new();

        for cookie in self.cookies.values() {
            if !cookie.is_expired()
                && cookie.matches_domain(url.host().unwrap_or(""))
                && cookie.matches_path(url.path())
            {
                cookies.push(cookie.to_string());
            }
        }

        cookies
    }

    pub fn to_cookie_header(&self, url: &Url) -> String {
        self.get_cookies_for_url(url).join("; ")
    }

    pub fn remove_expired(&mut self) {
        self.cookies.retain(|_, cookie| !cookie.is_expired());
    }

    pub fn to_vec(&self) -> Vec<Cookie> {
        self.cookies.values().cloned().collect()
    }

    pub fn from_vec(cookies: Vec<Cookie>) -> Self {
        let mut jar = Self::new();
        for cookie in cookies {
            jar.set(cookie);
        }
        jar
    }

    pub fn to_map(&self) -> HashMap<String, Cookie> {
        self.cookies.clone()
    }

    pub fn from_map(cookies: HashMap<String, Cookie>) -> Self {
        Self { cookies }
    }

    pub fn merge(&mut self, other: &CookieJar) {
        for (name, cookie) in other.iter() {
            self.set(cookie.clone());
        }
    }

    pub fn merge_into(mut self, other: &CookieJar) -> Self {
        self.merge(other);
        self
    }
}

impl Default for CookieJar {
    fn default() -> Self {
        Self::new()
    }
}

impl From<HashMap<String, Cookie>> for CookieJar {
    fn from(cookies: HashMap<String, Cookie>) -> Self {
        Self::from_map(cookies)
    }
}

impl Into<HashMap<String, Cookie>> for CookieJar {
    fn into(self) -> HashMap<String, Cookie> {
        self.to_map()
    }
}

impl std::fmt::Display for CookieJar {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for (name, cookie) in self.iter() {
            writeln!(f, "{}: {}", name, cookie.to_string())?;
        }
        Ok(())
    }
}
