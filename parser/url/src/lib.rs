pub mod error;
pub mod parser;
pub mod url;
pub mod utils;

pub use error::{UrlError, UrlResult};
pub use parser::UrlParser;
pub use url::Url;
pub use utils::*;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_url_parsing() {
        let url = Url::parse("https://example.com:8080/path?query=value#fragment").unwrap();
        assert_eq!(url.scheme(), "https");
        assert_eq!(url.host(), "example.com");
        assert_eq!(url.port(), Some(8080));
        assert_eq!(url.path(), "/path");
        assert_eq!(url.query(), Some("query=value"));
        assert_eq!(url.fragment(), Some("fragment"));
    }

    #[test]
    fn test_relative_url() {
        let base = Url::parse("https://example.com/base/").unwrap();
        let relative = Url::parse_relative("path/to/resource", &base).unwrap();
        assert_eq!(relative.path(), "/base/path/to/resource");
    }

    #[test]
    fn test_url_encoding() {
        let encoded = encode_component("hello world");
        assert_eq!(encoded, "hello%20world");

        let decoded = decode_component("hello%20world").unwrap();
        assert_eq!(decoded, "hello world");
    }
}
