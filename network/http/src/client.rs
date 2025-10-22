use crate::error::{HttpError, HttpResult};
use crate::request::{HttpRequest, HttpMethod};
use crate::response::HttpResponse;
use crate::cookies::CookieJar;
use crate::cache::HttpCache;
use crate::headers::HttpHeaders;
use url_parser::Url;
use std::time::Duration;
use std::sync::Arc;
use tokio::sync::RwLock;

pub struct HttpClient {
    cookie_jar: Arc<RwLock<CookieJar>>,
    cache: Arc<RwLock<HttpCache>>,
    timeout: Duration,
    max_redirects: usize,
    user_agent: String,
    default_headers: HttpHeaders,
}

impl HttpClient {
    pub fn new() -> Self {
        Self {
            cookie_jar: Arc::new(RwLock::new(CookieJar::new())),
            cache: Arc::new(RwLock::new(HttpCache::new())),
            timeout: Duration::from_secs(30),
            max_redirects: 10,
            user_agent: "ChromeBrowser/1.0".to_string(),
            default_headers: HttpHeaders::new(),
        }
    }

    pub fn with_timeout(mut self, timeout: Duration) -> Self {
        self.timeout = timeout;
        self
    }

    pub fn with_max_redirects(mut self, max_redirects: usize) -> Self {
        self.max_redirects = max_redirects;
        self
    }

    pub fn with_user_agent(mut self, user_agent: String) -> Self {
        self.user_agent = user_agent;
        self
    }

    pub fn with_default_header(mut self, name: String, value: String) -> Self {
        self.default_headers.set(&name, &value);
        self
    }

    pub async fn get(&self, url: &str) -> HttpResult<HttpResponse> {
        self.request(HttpMethod::GET, url).await
    }

    pub async fn post(&self, url: &str) -> HttpRequest {
        HttpRequest::new(HttpMethod::POST, url, self.clone())
    }

    pub async fn put(&self, url: &str) -> HttpRequest {
        HttpRequest::new(HttpMethod::PUT, url, self.clone())
    }

    pub async fn delete(&self, url: &str) -> HttpRequest {
        HttpRequest::new(HttpMethod::DELETE, url, self.clone())
    }

    pub async fn patch(&self, url: &str) -> HttpRequest {
        HttpRequest::new(HttpMethod::PATCH, url, self.clone())
    }

    pub async fn head(&self, url: &str) -> HttpResult<HttpResponse> {
        self.request(HttpMethod::HEAD, url).await
    }

    pub async fn options(&self, url: &str) -> HttpResult<HttpResponse> {
        self.request(HttpMethod::OPTIONS, url).await
    }

    pub async fn request(&self, method: HttpMethod, url: &str) -> HttpResult<HttpResponse> {
        let mut request = HttpRequest::new(method, url, self.clone());
        request.send().await
    }

    pub async fn cookie_jar(&self) -> Arc<RwLock<CookieJar>> {
        self.cookie_jar.clone()
    }

    pub async fn cache(&self) -> Arc<RwLock<HttpCache>> {
        self.cache.clone()
    }

    pub fn timeout(&self) -> Duration {
        self.timeout
    }

    pub fn max_redirects(&self) -> usize {
        self.max_redirects
    }

    pub fn user_agent(&self) -> &str {
        &self.user_agent
    }

    pub fn default_headers(&self) -> &HttpHeaders {
        &self.default_headers
    }

    pub async fn clear_cookies(&self) {
        let mut jar = self.cookie_jar.write().await;
        jar.clear();
    }

    pub async fn clear_cache(&self) {
        let mut cache = self.cache.write().await;
        cache.clear();
    }

    pub async fn get_cookies(&self, url: &str) -> HttpResult<Vec<String>> {
        let jar = self.cookie_jar.read().await;
        let url = Url::parse(url)?;
        Ok(jar.get_cookies_for_url(&url))
    }

    pub async fn set_cookie(&self, url: &str, cookie: &str) -> HttpResult<()> {
        let mut jar = self.cookie_jar.write().await;
        let url = Url::parse(url)?;
        jar.parse_set_cookie_with_url(cookie, &url)
    }

    pub async fn cache_size(&self) -> usize {
        let cache = self.cache.read().await;
        cache.size()
    }

    pub async fn cache_hit_rate(&self) -> f64 {
        let cache = self.cache.read().await;
        cache.hit_rate()
    }
}

impl Clone for HttpClient {
    fn clone(&self) -> Self {
        Self {
            cookie_jar: self.cookie_jar.clone(),
            cache: self.cache.clone(),
            timeout: self.timeout,
            max_redirects: self.max_redirects,
            user_agent: self.user_agent.clone(),
            default_headers: self.default_headers.clone(),
        }
    }
}

impl Default for HttpClient {
    fn default() -> Self {
        Self::new()
    }
}
