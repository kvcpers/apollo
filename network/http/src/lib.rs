pub mod cache;
pub mod client;
pub mod cookies;
pub mod error;
pub mod headers;
pub mod request;
pub mod response;
pub mod utils;

pub use cache::HttpCache;
pub use client::HttpClient;
pub use cookies::CookieJar;
pub use error::{HttpError, HttpResult};
pub use headers::HttpHeaders;
pub use request::{HttpMethod, HttpRequest};
pub use response::HttpResponse;

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_http_get() {
        let client = HttpClient::new();
        let response = client.get("https://httpbin.org/get").await.unwrap();
        assert_eq!(response.status(), 200);
    }

    #[tokio::test]
    async fn test_http_post() {
        let client = HttpClient::new();
        let response = client
            .post("https://httpbin.org/post")
            .json(&serde_json::json!({"key": "value"}))
            .send()
            .await
            .unwrap();
        assert_eq!(response.status(), 200);
    }

    #[test]
    fn test_cookie_parsing() {
        let cookie_jar = CookieJar::new();
        cookie_jar
            .parse_set_cookie("session=abc123; Path=/; HttpOnly")
            .unwrap();
        assert!(cookie_jar.get("session").is_some());
    }

    #[test]
    fn test_cache_headers() {
        let headers = HttpHeaders::new();
        headers.set("Cache-Control", "max-age=3600");
        headers.set("ETag", "\"abc123\"");

        assert_eq!(headers.get("Cache-Control"), Some("max-age=3600"));
        assert_eq!(headers.get("ETag"), Some("\"abc123\""));
    }
}
