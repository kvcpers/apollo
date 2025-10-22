use crate::error::{HttpError, HttpResult};
use std::collections::HashMap;
use std::time::{Duration, SystemTime, UNIX_EPOCH};

pub fn parse_content_type(
    content_type: &str,
) -> HttpResult<(String, Option<String>, Option<String>)> {
    let parts: Vec<&str> = content_type.split(';').collect();
    if parts.is_empty() {
        return Err(HttpError::InvalidHeader(
            "Empty Content-Type header".to_string(),
        ));
    }

    let media_type = parts[0].trim().to_string();
    let mut charset = None;
    let mut boundary = None;

    for part in &parts[1..] {
        let part = part.trim();
        if let Some((key, value)) = part.split_once('=') {
            match key.trim().to_lowercase().as_str() {
                "charset" => charset = Some(value.trim().to_string()),
                "boundary" => boundary = Some(value.trim().to_string()),
                _ => {}
            }
        }
    }

    Ok((media_type, charset, boundary))
}

pub fn parse_cache_control(cache_control: &str) -> HttpResult<HashMap<String, Option<String>>> {
    let mut directives = HashMap::new();

    for directive in cache_control.split(',') {
        let directive = directive.trim();
        if let Some((key, value)) = directive.split_once('=') {
            directives.insert(key.trim().to_string(), Some(value.trim().to_string()));
        } else {
            directives.insert(directive.to_string(), None);
        }
    }

    Ok(directives)
}

pub fn parse_accept_header(accept: &str) -> HttpResult<Vec<(String, f64)>> {
    let mut types = Vec::new();

    for type_spec in accept.split(',') {
        let type_spec = type_spec.trim();
        if let Some((media_type, q_value)) = type_spec.split_once(';') {
            let media_type = media_type.trim().to_string();
            let q_value = q_value.trim();
            if let Some(q_value) = q_value.strip_prefix("q=") {
                if let Ok(q) = q_value.parse::<f64>() {
                    types.push((media_type, q));
                } else {
                    types.push((media_type, 1.0));
                }
            } else {
                types.push((media_type, 1.0));
            }
        } else {
            types.push((type_spec.to_string(), 1.0));
        }
    }

    // Sort by quality value (descending)
    types.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap_or(std::cmp::Ordering::Equal));

    Ok(types)
}

pub fn parse_accept_language(accept_language: &str) -> HttpResult<Vec<(String, f64)>> {
    let mut languages = Vec::new();

    for lang_spec in accept_language.split(',') {
        let lang_spec = lang_spec.trim();
        if let Some((language, q_value)) = lang_spec.split_once(';') {
            let language = language.trim().to_string();
            let q_value = q_value.trim();
            if let Some(q_value) = q_value.strip_prefix("q=") {
                if let Ok(q) = q_value.parse::<f64>() {
                    languages.push((language, q));
                } else {
                    languages.push((language, 1.0));
                }
            } else {
                languages.push((language, 1.0));
            }
        } else {
            languages.push((lang_spec.to_string(), 1.0));
        }
    }

    // Sort by quality value (descending)
    languages.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap_or(std::cmp::Ordering::Equal));

    Ok(languages)
}

pub fn parse_accept_encoding(accept_encoding: &str) -> HttpResult<Vec<(String, f64)>> {
    let mut encodings = Vec::new();

    for enc_spec in accept_encoding.split(',') {
        let enc_spec = enc_spec.trim();
        if let Some((encoding, q_value)) = enc_spec.split_once(';') {
            let encoding = encoding.trim().to_string();
            let q_value = q_value.trim();
            if let Some(q_value) = q_value.strip_prefix("q=") {
                if let Ok(q) = q_value.parse::<f64>() {
                    encodings.push((encoding, q));
                } else {
                    encodings.push((encoding, 1.0));
                }
            } else {
                encodings.push((encoding, 1.0));
            }
        } else {
            encodings.push((enc_spec.to_string(), 1.0));
        }
    }

    // Sort by quality value (descending)
    encodings.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap_or(std::cmp::Ordering::Equal));

    Ok(encodings)
}

pub fn parse_range_header(range: &str) -> HttpResult<Vec<(Option<u64>, Option<u64>)>> {
    let mut ranges = Vec::new();

    if let Some(range_spec) = range.strip_prefix("bytes=") {
        for range_item in range_spec.split(',') {
            let range_item = range_item.trim();
            if range_item.starts_with('-') {
                // Suffix range: -500
                let suffix = range_item[1..].parse::<u64>()?;
                ranges.push((None, Some(suffix)));
            } else if range_item.ends_with('-') {
                // Prefix range: 500-
                let start = range_item[..range_item.len() - 1].parse::<u64>()?;
                ranges.push((Some(start), None));
            } else if let Some((start, end)) = range_item.split_once('-') {
                // Range: 500-999
                let start = start.parse::<u64>()?;
                let end = end.parse::<u64>()?;
                ranges.push((Some(start), Some(end)));
            }
        }
    }

    Ok(ranges)
}

pub fn parse_content_range(
    content_range: &str,
) -> HttpResult<(Option<u64>, Option<u64>, Option<u64>)> {
    if let Some(range_spec) = content_range.strip_prefix("bytes ") {
        if let Some((range, total)) = range_spec.split_once('/') {
            let total = if total == "*" {
                None
            } else {
                Some(total.parse::<u64>()?)
            };

            if let Some((start, end)) = range.split_once('-') {
                let start = start.parse::<u64>()?;
                let end = end.parse::<u64>()?;
                Ok((Some(start), Some(end), total))
            } else {
                Err(HttpError::InvalidHeader(
                    "Invalid Content-Range format".to_string(),
                ))
            }
        } else {
            Err(HttpError::InvalidHeader(
                "Invalid Content-Range format".to_string(),
            ))
        }
    } else {
        Err(HttpError::InvalidHeader(
            "Invalid Content-Range format".to_string(),
        ))
    }
}

pub fn parse_www_authenticate(www_authenticate: &str) -> HttpResult<HashMap<String, String>> {
    let mut auth_params = HashMap::new();

    if let Some((scheme, params)) = www_authenticate.split_once(' ') {
        auth_params.insert("scheme".to_string(), scheme.to_string());

        for param in params.split(',') {
            let param = param.trim();
            if let Some((key, value)) = param.split_once('=') {
                let key = key.trim().to_string();
                let value = value.trim().trim_matches('"').to_string();
                auth_params.insert(key, value);
            }
        }
    }

    Ok(auth_params)
}

pub fn parse_authorization(authorization: &str) -> HttpResult<(String, String)> {
    if let Some((scheme, credentials)) = authorization.split_once(' ') {
        Ok((scheme.to_string(), credentials.to_string()))
    } else {
        Err(HttpError::InvalidHeader(
            "Invalid Authorization format".to_string(),
        ))
    }
}

pub fn parse_set_cookie(set_cookie: &str) -> HttpResult<HashMap<String, String>> {
    let mut cookie_params = HashMap::new();

    if let Some((name_value, attributes)) = set_cookie.split_once(';') {
        if let Some((name, value)) = name_value.split_once('=') {
            cookie_params.insert("name".to_string(), name.trim().to_string());
            cookie_params.insert("value".to_string(), value.trim().to_string());
        }

        for attr in attributes.split(';') {
            let attr = attr.trim();
            if let Some((key, value)) = attr.split_once('=') {
                cookie_params.insert(key.trim().to_string(), value.trim().to_string());
            } else {
                cookie_params.insert(attr.to_string(), "".to_string());
            }
        }
    }

    Ok(cookie_params)
}

pub fn parse_cookie(cookie: &str) -> HttpResult<HashMap<String, String>> {
    let mut cookies = HashMap::new();

    for cookie_item in cookie.split(';') {
        let cookie_item = cookie_item.trim();
        if let Some((name, value)) = cookie_item.split_once('=') {
            cookies.insert(name.trim().to_string(), value.trim().to_string());
        }
    }

    Ok(cookies)
}

pub fn format_http_date(time: SystemTime) -> String {
    // This is a simplified implementation
    // In a real implementation, we would format the date according to RFC 7231
    "Thu, 01 Jan 2038 00:00:00 GMT".to_string()
}

pub fn parse_http_date(date_str: &str) -> HttpResult<SystemTime> {
    // This is a simplified implementation
    // In a real implementation, we would parse the date according to RFC 7231
    Ok(UNIX_EPOCH + Duration::from_secs(0))
}

pub fn is_redirect_status(status: u16) -> bool {
    matches!(status, 301 | 302 | 303 | 307 | 308)
}

pub fn is_success_status(status: u16) -> bool {
    status >= 200 && status < 300
}

pub fn is_client_error_status(status: u16) -> bool {
    status >= 400 && status < 500
}

pub fn is_server_error_status(status: u16) -> bool {
    status >= 500 && status < 600
}

pub fn is_cacheable_status(status: u16) -> bool {
    matches!(
        status,
        200 | 203 | 204 | 206 | 300 | 301 | 404 | 405 | 410 | 414
    )
}

pub fn is_conditional_header(header_name: &str) -> bool {
    matches!(
        header_name.to_lowercase().as_str(),
        "if-match" | "if-none-match" | "if-modified-since" | "if-unmodified-since" | "if-range"
    )
}

pub fn is_hop_by_hop_header(header_name: &str) -> bool {
    matches!(
        header_name.to_lowercase().as_str(),
        "connection"
            | "keep-alive"
            | "proxy-authenticate"
            | "proxy-authorization"
            | "te"
            | "trailers"
            | "transfer-encoding"
            | "upgrade"
    )
}

pub fn is_end_to_end_header(header_name: &str) -> bool {
    !is_hop_by_hop_header(header_name)
}

pub fn is_safe_method(method: &str) -> bool {
    matches!(
        method.to_uppercase().as_str(),
        "GET" | "HEAD" | "OPTIONS" | "TRACE"
    )
}

pub fn is_idempotent_method(method: &str) -> bool {
    matches!(
        method.to_uppercase().as_str(),
        "GET" | "HEAD" | "PUT" | "DELETE" | "OPTIONS" | "TRACE"
    )
}

pub fn is_cacheable_method(method: &str) -> bool {
    matches!(method.to_uppercase().as_str(), "GET" | "HEAD")
}

pub fn is_unsafe_method(method: &str) -> bool {
    matches!(
        method.to_uppercase().as_str(),
        "POST" | "PUT" | "DELETE" | "PATCH"
    )
}

pub fn get_default_port(scheme: &str) -> Option<u16> {
    match scheme.to_lowercase().as_str() {
        "http" => Some(80),
        "https" => Some(443),
        "ftp" => Some(21),
        "ftps" => Some(990),
        "ssh" => Some(22),
        "telnet" => Some(23),
        "smtp" => Some(25),
        "dns" => Some(53),
        "pop3" => Some(110),
        "nntp" => Some(119),
        "imap" => Some(143),
        "snmp" => Some(161),
        "ldap" => Some(389),
        "https" => Some(443),
        "smtps" => Some(465),
        "imaps" => Some(993),
        "pop3s" => Some(995),
        "socks" => Some(1080),
        "mysql" => Some(3306),
        "postgresql" => Some(5432),
        "redis" => Some(6379),
        "mongodb" => Some(27017),
        _ => None,
    }
}

pub fn is_secure_scheme(scheme: &str) -> bool {
    matches!(
        scheme.to_lowercase().as_str(),
        "https" | "wss" | "ftps" | "smtps" | "imaps" | "pop3s"
    )
}

pub fn is_special_scheme(scheme: &str) -> bool {
    matches!(
        scheme.to_lowercase().as_str(),
        "http" | "https" | "ws" | "wss" | "ftp" | "file"
    )
}

pub fn normalize_header_name(name: &str) -> String {
    let mut result = String::new();
    let mut capitalize_next = true;

    for c in name.chars() {
        if c == '-' {
            result.push('-');
            capitalize_next = true;
        } else if capitalize_next {
            result.push(c.to_uppercase().next().unwrap_or(c));
            capitalize_next = false;
        } else {
            result.push(c.to_lowercase().next().unwrap_or(c));
        }
    }

    result
}

pub fn normalize_header_value(value: &str) -> String {
    // Remove leading and trailing whitespace
    value.trim().to_string()
}

pub fn is_valid_header_name(name: &str) -> bool {
    if name.is_empty() {
        return false;
    }

    for c in name.chars() {
        if !c.is_ascii_alphanumeric() && c != '-' && c != '_' {
            return false;
        }
    }

    true
}

pub fn is_valid_header_value(value: &str) -> bool {
    // Check for control characters
    for c in value.chars() {
        if c.is_control() && c != '\t' {
            return false;
        }
    }

    true
}

pub fn is_valid_method(method: &str) -> bool {
    matches!(
        method.to_uppercase().as_str(),
        "GET" | "POST" | "PUT" | "DELETE" | "PATCH" | "HEAD" | "OPTIONS" | "TRACE" | "CONNECT"
    )
}

pub fn is_valid_status_code(status: u16) -> bool {
    status >= 100 && status < 600
}

pub fn get_status_reason(status: u16) -> &'static str {
    match status {
        100 => "Continue",
        101 => "Switching Protocols",
        200 => "OK",
        201 => "Created",
        202 => "Accepted",
        203 => "Non-Authoritative Information",
        204 => "No Content",
        205 => "Reset Content",
        206 => "Partial Content",
        300 => "Multiple Choices",
        301 => "Moved Permanently",
        302 => "Found",
        303 => "See Other",
        304 => "Not Modified",
        305 => "Use Proxy",
        307 => "Temporary Redirect",
        308 => "Permanent Redirect",
        400 => "Bad Request",
        401 => "Unauthorized",
        402 => "Payment Required",
        403 => "Forbidden",
        404 => "Not Found",
        405 => "Method Not Allowed",
        406 => "Not Acceptable",
        407 => "Proxy Authentication Required",
        408 => "Request Timeout",
        409 => "Conflict",
        410 => "Gone",
        411 => "Length Required",
        412 => "Precondition Failed",
        413 => "Payload Too Large",
        414 => "URI Too Long",
        415 => "Unsupported Media Type",
        416 => "Range Not Satisfiable",
        417 => "Expectation Failed",
        418 => "I'm a teapot",
        421 => "Misdirected Request",
        422 => "Unprocessable Entity",
        423 => "Locked",
        424 => "Failed Dependency",
        425 => "Too Early",
        426 => "Upgrade Required",
        428 => "Precondition Required",
        429 => "Too Many Requests",
        431 => "Request Header Fields Too Large",
        451 => "Unavailable For Legal Reasons",
        500 => "Internal Server Error",
        501 => "Not Implemented",
        502 => "Bad Gateway",
        503 => "Service Unavailable",
        504 => "Gateway Timeout",
        505 => "HTTP Version Not Supported",
        506 => "Variant Also Negotiates",
        507 => "Insufficient Storage",
        508 => "Loop Detected",
        510 => "Not Extended",
        511 => "Network Authentication Required",
        _ => "Unknown",
    }
}
