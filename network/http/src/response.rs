use crate::error::{HttpError, HttpResult};
use crate::headers::HttpHeaders;
use serde::Deserialize;
use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
pub struct HttpResponse {
    status: u16,
    headers: HttpHeaders,
    body: Vec<u8>,
}

impl HttpResponse {
    pub fn new(status: u16, headers: HttpHeaders, body: Vec<u8>) -> Self {
        Self {
            status,
            headers,
            body,
        }
    }

    pub fn status(&self) -> u16 {
        self.status
    }

    pub fn is_success(&self) -> bool {
        self.status >= 200 && self.status < 300
    }

    pub fn is_redirect(&self) -> bool {
        self.status >= 300 && self.status < 400
    }

    pub fn is_client_error(&self) -> bool {
        self.status >= 400 && self.status < 500
    }

    pub fn is_server_error(&self) -> bool {
        self.status >= 500 && self.status < 600
    }

    pub fn headers(&self) -> &HttpHeaders {
        &self.headers
    }

    pub fn header(&self, name: &str) -> Option<&str> {
        self.headers.get(name)
    }

    pub fn body(&self) -> &[u8] {
        &self.body
    }

    pub fn text(&self) -> HttpResult<String> {
        Ok(String::from_utf8(self.body.clone())?)
    }

    pub fn json<T: for<'de> Deserialize<'de>>(&self) -> HttpResult<T> {
        Ok(serde_json::from_slice(&self.body)?)
    }

    pub fn content_type(&self) -> Option<&str> {
        self.header("Content-Type")
    }

    pub fn content_length(&self) -> Option<usize> {
        self.header("Content-Length")
            .and_then(|s| s.parse().ok())
    }

    pub fn location(&self) -> Option<&str> {
        self.header("Location")
    }

    pub fn set_cookie(&self) -> Option<&str> {
        self.header("Set-Cookie")
    }

    pub fn cache_control(&self) -> Option<&str> {
        self.header("Cache-Control")
    }

    pub fn etag(&self) -> Option<&str> {
        self.header("ETag")
    }

    pub fn last_modified(&self) -> Option<&str> {
        self.header("Last-Modified")
    }

    pub fn expires(&self) -> Option<&str> {
        self.header("Expires")
    }

    pub fn age(&self) -> Option<&str> {
        self.header("Age")
    }

    pub fn vary(&self) -> Option<&str> {
        self.header("Vary")
    }

    pub fn server(&self) -> Option<&str> {
        self.header("Server")
    }

    pub fn date(&self) -> Option<&str> {
        self.header("Date")
    }

    pub fn connection(&self) -> Option<&str> {
        self.header("Connection")
    }

    pub fn transfer_encoding(&self) -> Option<&str> {
        self.header("Transfer-Encoding")
    }

    pub fn content_encoding(&self) -> Option<&str> {
        self.header("Content-Encoding")
    }

    pub fn content_language(&self) -> Option<&str> {
        self.header("Content-Language")
    }

    pub fn content_disposition(&self) -> Option<&str> {
        self.header("Content-Disposition")
    }

    pub fn content_range(&self) -> Option<&str> {
        self.header("Content-Range")
    }

    pub fn accept_ranges(&self) -> Option<&str> {
        self.header("Accept-Ranges")
    }

    pub fn allow(&self) -> Option<&str> {
        self.header("Allow")
    }

    pub fn www_authenticate(&self) -> Option<&str> {
        self.header("WWW-Authenticate")
    }

    pub fn proxy_authenticate(&self) -> Option<&str> {
        self.header("Proxy-Authenticate")
    }

    pub fn authorization(&self) -> Option<&str> {
        self.header("Authorization")
    }

    pub fn proxy_authorization(&self) -> Option<&str> {
        self.header("Proxy-Authorization")
    }

    pub fn referer(&self) -> Option<&str> {
        self.header("Referer")
    }

    pub fn user_agent(&self) -> Option<&str> {
        self.header("User-Agent")
    }

    pub fn origin(&self) -> Option<&str> {
        self.header("Origin")
    }

    pub fn host(&self) -> Option<&str> {
        self.header("Host")
    }

    pub fn from(&self) -> Option<&str> {
        self.header("From")
    }

    pub fn warning(&self) -> Option<&str> {
        self.header("Warning")
    }

    pub fn upgrade(&self) -> Option<&str> {
        self.header("Upgrade")
    }

    pub fn expect(&self) -> Option<&str> {
        self.header("Expect")
    }

    pub fn if_match(&self) -> Option<&str> {
        self.header("If-Match")
    }

    pub fn if_none_match(&self) -> Option<&str> {
        self.header("If-None-Match")
    }

    pub fn if_modified_since(&self) -> Option<&str> {
        self.header("If-Modified-Since")
    }

    pub fn if_unmodified_since(&self) -> Option<&str> {
        self.header("If-Unmodified-Since")
    }

    pub fn if_range(&self) -> Option<&str> {
        self.header("If-Range")
    }

    pub fn range(&self) -> Option<&str> {
        self.header("Range")
    }

    pub fn max_forwards(&self) -> Option<&str> {
        self.header("Max-Forwards")
    }

    pub fn proxy_connection(&self) -> Option<&str> {
        self.header("Proxy-Connection")
    }

    pub fn te(&self) -> Option<&str> {
        self.header("TE")
    }

    pub fn trailer(&self) -> Option<&str> {
        self.header("Trailer")
    }

    pub fn pragma(&self) -> Option<&str> {
        self.header("Pragma")
    }

    pub fn via(&self) -> Option<&str> {
        self.header("Via")
    }

    pub fn x_forwarded_for(&self) -> Option<&str> {
        self.header("X-Forwarded-For")
    }

    pub fn x_forwarded_proto(&self) -> Option<&str> {
        self.header("X-Forwarded-Proto")
    }

    pub fn x_forwarded_host(&self) -> Option<&str> {
        self.header("X-Forwarded-Host")
    }

    pub fn x_real_ip(&self) -> Option<&str> {
        self.header("X-Real-IP")
    }

    pub fn x_requested_with(&self) -> Option<&str> {
        self.header("X-Requested-With")
    }

    pub fn x_csrf_token(&self) -> Option<&str> {
        self.header("X-CSRF-Token")
    }

    pub fn x_frame_options(&self) -> Option<&str> {
        self.header("X-Frame-Options")
    }

    pub fn x_content_type_options(&self) -> Option<&str> {
        self.header("X-Content-Type-Options")
    }

    pub fn x_xss_protection(&self) -> Option<&str> {
        self.header("X-XSS-Protection")
    }

    pub fn strict_transport_security(&self) -> Option<&str> {
        self.header("Strict-Transport-Security")
    }

    pub fn content_security_policy(&self) -> Option<&str> {
        self.header("Content-Security-Policy")
    }

    pub fn referrer_policy(&self) -> Option<&str> {
        self.header("Referrer-Policy")
    }

    pub fn feature_policy(&self) -> Option<&str> {
        self.header("Feature-Policy")
    }

    pub fn permissions_policy(&self) -> Option<&str> {
        self.header("Permissions-Policy")
    }

    pub fn cross_origin_embedder_policy(&self) -> Option<&str> {
        self.header("Cross-Origin-Embedder-Policy")
    }

    pub fn cross_origin_opener_policy(&self) -> Option<&str> {
        self.header("Cross-Origin-Opener-Policy")
    }

    pub fn cross_origin_resource_policy(&self) -> Option<&str> {
        self.header("Cross-Origin-Resource-Policy")
    }

    pub fn access_control_allow_origin(&self) -> Option<&str> {
        self.header("Access-Control-Allow-Origin")
    }

    pub fn access_control_allow_methods(&self) -> Option<&str> {
        self.header("Access-Control-Allow-Methods")
    }

    pub fn access_control_allow_headers(&self) -> Option<&str> {
        self.header("Access-Control-Allow-Headers")
    }

    pub fn access_control_allow_credentials(&self) -> Option<&str> {
        self.header("Access-Control-Allow-Credentials")
    }

    pub fn access_control_expose_headers(&self) -> Option<&str> {
        self.header("Access-Control-Expose-Headers")
    }

    pub fn access_control_max_age(&self) -> Option<&str> {
        self.header("Access-Control-Max-Age")
    }

    pub fn access_control_request_method(&self) -> Option<&str> {
        self.header("Access-Control-Request-Method")
    }

    pub fn access_control_request_headers(&self) -> Option<&str> {
        self.header("Access-Control-Request-Headers")
    }

    pub fn link(&self) -> Option<&str> {
        self.header("Link")
    }

    pub fn alt_svc(&self) -> Option<&str> {
        self.header("Alt-Svc")
    }

    pub fn clear_site_data(&self) -> Option<&str> {
        self.header("Clear-Site-Data")
    }

    pub fn report_to(&self) -> Option<&str> {
        self.header("Report-To")
    }

    pub fn nel(&self) -> Option<&str> {
        self.header("NEL")
    }

    pub fn expect_ct(&self) -> Option<&str> {
        self.header("Expect-CT")
    }

    pub fn public_key_pins(&self) -> Option<&str> {
        self.header("Public-Key-Pins")
    }

    pub fn public_key_pins_report_only(&self) -> Option<&str> {
        self.header("Public-Key-Pins-Report-Only")
    }

    pub fn timing_allow_origin(&self) -> Option<&str> {
        self.header("Timing-Allow-Origin")
    }

    pub fn x_dns_prefetch_control(&self) -> Option<&str> {
        self.header("X-DNS-Prefetch-Control")
    }

    pub fn x_download_options(&self) -> Option<&str> {
        self.header("X-Download-Options")
    }

    pub fn x_permitted_cross_domain_policies(&self) -> Option<&str> {
        self.header("X-Permitted-Cross-Domain-Policies")
    }

    pub fn x_powered_by(&self) -> Option<&str> {
        self.header("X-Powered-By")
    }

    pub fn x_ua_compatible(&self) -> Option<&str> {
        self.header("X-UA-Compatible")
    }

    pub fn x_webkit_csp(&self) -> Option<&str> {
        self.header("X-WebKit-CSP")
    }

    pub fn x_webkit_csp_report_only(&self) -> Option<&str> {
        self.header("X-WebKit-CSP-Report-Only")
    }
}
