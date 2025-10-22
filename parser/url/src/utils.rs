use crate::error::{UrlError, UrlResult};

pub fn encode_component(input: &str) -> String {
    let mut result = String::new();
    for byte in input.bytes() {
        if is_unreserved(byte) {
            result.push(byte as char);
        } else {
            result.push_str(&format!("%{:02X}", byte));
        }
    }
    result
}

pub fn decode_component(input: &str) -> UrlResult<String> {
    let mut result = String::new();
    let mut chars = input.chars();
    
    while let Some(c) = chars.next() {
        if c == '%' {
            let hex1 = chars.next().ok_or_else(|| UrlError::InvalidEncoding("Incomplete percent encoding".to_string()))?;
            let hex2 = chars.next().ok_or_else(|| UrlError::InvalidEncoding("Incomplete percent encoding".to_string()))?;
            
            let byte = u8::from_str_radix(&format!("{}{}", hex1, hex2), 16)
                .map_err(|_| UrlError::InvalidEncoding("Invalid hex digit".to_string()))?;
            
            result.push(byte as char);
        } else {
            result.push(c);
        }
    }
    
    Ok(result)
}

pub fn is_unreserved(byte: u8) -> bool {
    matches!(byte, 
        b'A'..=b'Z' | b'a'..=b'z' | b'0'..=b'9' | 
        b'-' | b'.' | b'_' | b'~'
    )
}

pub fn is_reserved(byte: u8) -> bool {
    matches!(byte, 
        b'!' | b'#' | b'$' | b'%' | b'&' | b'\'' | 
        b'(' | b')' | b'*' | b'+' | b',' | b'/' | 
        b':' | b';' | b'=' | b'?' | b'@' | b'[' | 
        b']'
    )
}

pub fn is_special_scheme(scheme: &str) -> bool {
    matches!(scheme, "http" | "https" | "ws" | "wss" | "ftp" | "file")
}

pub fn is_default_port(scheme: &str, port: u16) -> bool {
    match scheme {
        "http" | "ws" => port == 80,
        "https" | "wss" => port == 443,
        "ftp" => port == 21,
        _ => false,
    }
}

pub fn normalize_path(path: &str) -> String {
    let mut segments = Vec::new();
    let parts: Vec<&str> = path.split('/').collect();
    
    for part in parts {
        match part {
            "" | "." => continue,
            ".." => {
                segments.pop();
            }
            _ => segments.push(part),
        }
    }
    
    if path.starts_with('/') {
        format!("/{}", segments.join("/"))
    } else {
        segments.join("/")
    }
}

pub fn is_valid_hostname(hostname: &str) -> bool {
    if hostname.is_empty() {
        return false;
    }
    
    if hostname.len() > 253 {
        return false;
    }
    
    for label in hostname.split('.') {
        if label.is_empty() || label.len() > 63 {
            return false;
        }
        
        if label.starts_with('-') || label.ends_with('-') {
            return false;
        }
        
        for c in label.chars() {
            if !c.is_ascii_alphanumeric() && c != '-' {
                return false;
            }
        }
    }
    
    true
}

pub fn is_valid_ipv4(ip: &str) -> bool {
    let parts: Vec<&str> = ip.split('.').collect();
    if parts.len() != 4 {
        return false;
    }
    
    for part in parts {
        if let Ok(num) = part.parse::<u8>() {
            if num > 255 {
                return false;
            }
        } else {
            return false;
        }
    }
    
    true
}

pub fn is_valid_ipv6(ip: &str) -> bool {
    // Simplified IPv6 validation
    if ip.is_empty() {
        return false;
    }
    
    let parts: Vec<&str> = ip.split(':').collect();
    if parts.len() < 3 || parts.len() > 8 {
        return false;
    }
    
    for part in parts {
        if part.is_empty() {
            continue;
        }
        
        if part.len() > 4 {
            return false;
        }
        
        for c in part.chars() {
            if !c.is_ascii_hexdigit() {
                return false;
            }
        }
    }
    
    true
}

pub fn parse_query_string(query: &str) -> std::collections::HashMap<String, String> {
    let mut params = std::collections::HashMap::new();
    
    for pair in query.split('&') {
        if let Some((key, value)) = pair.split_once('=') {
            params.insert(key.to_string(), value.to_string());
        }
    }
    
    params
}

pub fn build_query_string(params: &std::collections::HashMap<String, String>) -> String {
    params.iter()
        .map(|(k, v)| format!("{}={}", k, v))
        .collect::<Vec<_>>()
        .join("&")
}
