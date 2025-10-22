use crate::error::{UrlError, UrlResult};
use crate::url::Url;
// use crate::utils::*;

pub struct UrlParser;

impl UrlParser {
    pub fn parse(input: &str) -> UrlResult<Url> {
        let mut url = Url::new();
        let mut input = input.trim();

        // Parse scheme
        if let Some((scheme, rest)) = input.split_once(':') {
            if !scheme.is_empty() && is_valid_scheme(scheme) {
                url.set_scheme(scheme.to_lowercase());
                input = rest;
            } else {
                return Err(UrlError::InvalidScheme(scheme.to_string()));
            }
        }

        // Parse authority
        if input.starts_with("//") {
            input = &input[2..];
            let (authority, rest) = if let Some(pos) = input.find('/') {
                (&input[..pos], &input[pos..])
            } else if let Some(pos) = input.find('?') {
                (&input[..pos], &input[pos..])
            } else if let Some(pos) = input.find('#') {
                (&input[..pos], &input[pos..])
            } else {
                (input, "")
            };

            Self::parse_authority(authority, &mut url)?;
            input = rest;
        }

        // Parse path
        if !input.is_empty() && !input.starts_with('?') && !input.starts_with('#') {
            let (path, rest) = if let Some(pos) = input.find('?') {
                (&input[..pos], &input[pos..])
            } else if let Some(pos) = input.find('#') {
                (&input[..pos], &input[pos..])
            } else {
                (input, "")
            };

            url.set_path(path.to_string());
            input = rest;
        }

        // Parse query
        if input.starts_with('?') {
            input = &input[1..];
            let (query, rest) = if let Some(pos) = input.find('#') {
                (&input[..pos], &input[pos..])
            } else {
                (input, "")
            };

            url.set_query(Some(query.to_string()));
            input = rest;
        }

        // Parse fragment
        if input.starts_with('#') {
            input = &input[1..];
            url.set_fragment(Some(input.to_string()));
        }

        Ok(url)
    }

    pub fn parse_relative(input: &str, base: &Url) -> UrlResult<Url> {
        let mut url = base.clone();

        if let Some(authority) = input.strip_prefix("//") {
            // Protocol-relative URL
            Self::parse_authority(authority, &mut url)?;
        } else if input.starts_with('/') {
            // Absolute path
            url.set_path(input.to_string());
        } else {
            // Relative path
            let base_path = url.path();
            let new_path = if base_path.ends_with('/') {
                format!("{}{}", base_path, input)
            } else {
                format!("{}/{}", base_path, input)
            };
            url.set_path(new_path);
        }

        Ok(url)
    }

    fn parse_authority(authority: &str, url: &mut Url) -> UrlResult<()> {
        let mut authority = authority;

        // Parse userinfo
        if let Some(at_pos) = authority.rfind('@') {
            let userinfo = &authority[..at_pos];
            authority = &authority[at_pos + 1..];

            if let Some(colon_pos) = userinfo.find(':') {
                let username = &userinfo[..colon_pos];
                let password = &userinfo[colon_pos + 1..];
                url.set_username(Some(username.to_string()));
                url.set_password(Some(password.to_string()));
            } else {
                url.set_username(Some(userinfo.to_string()));
            }
        }

        // Parse host and port
        if let Some(colon_pos) = authority.rfind(':') {
            let host = &authority[..colon_pos];
            let port_str = &authority[colon_pos + 1..];

            if let Ok(port) = port_str.parse::<u16>() {
                url.set_host(Some(host.to_string()));
                url.set_port(Some(port));
            } else {
                url.set_host(Some(authority.to_string()));
            }
        } else {
            url.set_host(Some(authority.to_string()));
        }

        Ok(())
    }
}

fn is_valid_scheme(scheme: &str) -> bool {
    if scheme.is_empty() {
        return false;
    }

    let mut chars = scheme.chars();
    if !chars.next().unwrap().is_ascii_alphabetic() {
        return false;
    }

    chars.all(|c| c.is_ascii_alphanumeric() || c == '+' || c == '-' || c == '.')
}
