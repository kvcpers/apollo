use crate::error::{HttpError, HttpResult};
use crate::response::HttpResponse;
use url_parser::Url;
use std::collections::HashMap;
use std::time::{SystemTime, UNIX_EPOCH, Duration};
use serde::{Serialize, Deserialize};

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CacheEntry {
    response: HttpResponse,
    timestamp: SystemTime,
    etag: Option<String>,
    last_modified: Option<String>,
    max_age: Option<Duration>,
    expires: Option<SystemTime>,
    cache_control: Option<String>,
    vary: Option<String>,
}

impl CacheEntry {
    pub fn new(response: HttpResponse) -> Self {
        Self {
            response,
            timestamp: SystemTime::now(),
            etag: None,
            last_modified: None,
            max_age: None,
            expires: None,
            cache_control: None,
            vary: None,
        }
    }

    pub fn response(&self) -> &HttpResponse {
        &self.response
    }

    pub fn timestamp(&self) -> SystemTime {
        self.timestamp
    }

    pub fn etag(&self) -> Option<&str> {
        self.etag.as_deref()
    }

    pub fn last_modified(&self) -> Option<&str> {
        self.last_modified.as_deref()
    }

    pub fn max_age(&self) -> Option<Duration> {
        self.max_age
    }

    pub fn expires(&self) -> Option<SystemTime> {
        self.expires
    }

    pub fn cache_control(&self) -> Option<&str> {
        self.cache_control.as_deref()
    }

    pub fn vary(&self) -> Option<&str> {
        self.vary.as_deref()
    }

    pub fn is_expired(&self) -> bool {
        if let Some(expires) = self.expires {
            SystemTime::now() > expires
        } else if let Some(max_age) = self.max_age {
            SystemTime::now() > self.timestamp + max_age
        } else {
            false
        }
    }

    pub fn is_stale(&self) -> bool {
        self.is_expired()
    }

    pub fn age(&self) -> Duration {
        SystemTime::now().duration_since(self.timestamp).unwrap_or_default()
    }

    pub fn remaining_ttl(&self) -> Option<Duration> {
        if let Some(expires) = self.expires {
            let now = SystemTime::now();
            if now < expires {
                Some(expires.duration_since(now).unwrap_or_default())
            } else {
                None
            }
        } else if let Some(max_age) = self.max_age {
            let age = self.age();
            if age < max_age {
                Some(max_age - age)
            } else {
                None
            }
        } else {
            None
        }
    }

    pub fn can_serve_stale(&self) -> bool {
        // This is a simplified implementation
        // In a real implementation, we would check for stale-while-revalidate directives
        false
    }

    pub fn needs_revalidation(&self) -> bool {
        self.etag.is_some() || self.last_modified.is_some()
    }

    pub fn is_cacheable(&self) -> bool {
        let status = self.response.status();
        let cache_control = self.response.cache_control();
        
        // Check if response is cacheable
        if status >= 200 && status < 300 {
            // Check for no-cache or no-store directives
            if let Some(cc) = cache_control {
                if cc.contains("no-cache") || cc.contains("no-store") {
                    return false;
                }
            }
            true
        } else if status >= 300 && status < 400 {
            // Redirects are generally cacheable
            true
        } else {
            // Other status codes are generally not cacheable
            false
        }
    }

    pub fn set_etag(&mut self, etag: Option<String>) {
        self.etag = etag;
    }

    pub fn set_last_modified(&mut self, last_modified: Option<String>) {
        self.last_modified = last_modified;
    }

    pub fn set_max_age(&mut self, max_age: Option<Duration>) {
        self.max_age = max_age;
    }

    pub fn set_expires(&mut self, expires: Option<SystemTime>) {
        self.expires = expires;
    }

    pub fn set_cache_control(&mut self, cache_control: Option<String>) {
        self.cache_control = cache_control;
    }

    pub fn set_vary(&mut self, vary: Option<String>) {
        self.vary = vary;
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct HttpCache {
    entries: HashMap<String, CacheEntry>,
    hits: u64,
    misses: u64,
    max_size: usize,
    max_age: Option<Duration>,
}

impl HttpCache {
    pub fn new() -> Self {
        Self {
            entries: HashMap::new(),
            hits: 0,
            misses: 0,
            max_size: 1000,
            max_age: None,
        }
    }

    pub fn with_max_size(mut self, max_size: usize) -> Self {
        self.max_size = max_size;
        self
    }

    pub fn with_max_age(mut self, max_age: Option<Duration>) -> Self {
        self.max_age = max_age;
        self
    }

    pub fn get(&mut self, url: &str) -> Option<&CacheEntry> {
        let is_expired = self.entries.get(url).map(|entry| entry.is_expired()).unwrap_or(false);
        if is_expired {
            self.entries.remove(url);
            self.misses += 1;
            None
        } else if let Some(entry) = self.entries.get(url) {
            self.hits += 1;
            Some(entry)
        } else {
            self.misses += 1;
            None
        }
    }

    pub fn set(&mut self, url: &str, response: HttpResponse) -> HttpResult<()> {
        let mut entry = CacheEntry::new(response);
        
        // Extract cache-related headers
        if let Some(etag) = entry.response().etag() {
            entry.set_etag(Some(etag.to_string()));
        }
        
        if let Some(last_modified) = entry.response().last_modified() {
            entry.set_last_modified(Some(last_modified.to_string()));
        }
        
        let cache_control = entry.response().cache_control().map(|s| s.to_string());
        if let Some(ref cache_control) = cache_control {
            entry.set_cache_control(Some(cache_control.clone()));
            
            // Parse max-age directive
            if let Some(max_age_str) = cache_control.split(',').find(|s| s.trim().starts_with("max-age=")) {
                if let Some(max_age) = max_age_str.split('=').nth(1) {
                    if let Ok(seconds) = max_age.trim().parse::<u64>() {
                        entry.set_max_age(Some(Duration::from_secs(seconds)));
                    }
                }
            }
        }
        
        if let Some(vary) = entry.response().vary() {
            entry.set_vary(Some(vary.to_string()));
        }
        
        if let Some(expires) = entry.response().expires() {
            // This is a simplified implementation
            // In a real implementation, we would parse the date properly
            entry.set_expires(Some(SystemTime::now() + Duration::from_secs(3600)));
        }
        
        // Check if entry is cacheable
        if !entry.is_cacheable() {
            return Ok(());
        }
        
        // Check cache size limit
        if self.entries.len() >= self.max_size {
            self.evict_oldest();
        }
        
        self.entries.insert(url.to_string(), entry);
        Ok(())
    }

    pub fn remove(&mut self, url: &str) -> Option<CacheEntry> {
        self.entries.remove(url)
    }

    pub fn clear(&mut self) {
        self.entries.clear();
        self.hits = 0;
        self.misses = 0;
    }

    pub fn size(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }

    pub fn hits(&self) -> u64 {
        self.hits
    }

    pub fn misses(&self) -> u64 {
        self.misses
    }

    pub fn hit_rate(&self) -> f64 {
        let total = self.hits + self.misses;
        if total == 0 {
            0.0
        } else {
            self.hits as f64 / total as f64
        }
    }

    pub fn max_size(&self) -> usize {
        self.max_size
    }

    pub fn max_age(&self) -> Option<Duration> {
        self.max_age
    }

    pub fn set_max_size(&mut self, max_size: usize) {
        self.max_size = max_size;
        while self.entries.len() > self.max_size {
            self.evict_oldest();
        }
    }

    pub fn set_max_age(&mut self, max_age: Option<Duration>) {
        self.max_age = max_age;
    }

    pub fn evict_expired(&mut self) {
        self.entries.retain(|_, entry| !entry.is_expired());
    }

    pub fn evict_oldest(&mut self) {
        if let Some((oldest_url, _)) = self.entries.iter()
            .min_by_key(|(_, entry)| entry.timestamp()) {
            let oldest_url = oldest_url.clone();
            self.entries.remove(&oldest_url);
        }
    }

    pub fn evict_lru(&mut self) {
        // This is a simplified implementation
        // In a real implementation, we would track access times
        self.evict_oldest();
    }

    pub fn evict_random(&mut self) {
        if let Some((url, _)) = self.entries.iter().next() {
            let url = url.clone();
            self.entries.remove(&url);
        }
    }

    pub fn contains(&self, url: &str) -> bool {
        self.entries.contains_key(url)
    }

    pub fn get_entry(&self, url: &str) -> Option<&CacheEntry> {
        self.entries.get(url)
    }

    pub fn get_mut_entry(&mut self, url: &str) -> Option<&mut CacheEntry> {
        self.entries.get_mut(url)
    }

    pub fn iter(&self) -> impl Iterator<Item = (&String, &CacheEntry)> {
        self.entries.iter()
    }

    pub fn keys(&self) -> impl Iterator<Item = &String> {
        self.entries.keys()
    }

    pub fn values(&self) -> impl Iterator<Item = &CacheEntry> {
        self.entries.values()
    }

    pub fn to_vec(&self) -> Vec<(String, CacheEntry)> {
        self.entries.iter().map(|(k, v)| (k.clone(), v.clone())).collect()
    }

    pub fn from_vec(entries: Vec<(String, CacheEntry)>) -> Self {
        let mut cache = Self::new();
        for (url, entry) in entries {
            cache.entries.insert(url, entry);
        }
        cache
    }

    pub fn to_map(&self) -> HashMap<String, CacheEntry> {
        self.entries.clone()
    }

    pub fn from_map(entries: HashMap<String, CacheEntry>) -> Self {
        Self {
            entries,
            hits: 0,
            misses: 0,
            max_size: 1000,
            max_age: None,
        }
    }

    pub fn merge(&mut self, other: &HttpCache) {
        for (url, entry) in other.iter() {
            self.entries.insert(url.clone(), entry.clone());
        }
        self.hits += other.hits;
        self.misses += other.misses;
    }

    pub fn merge_into(mut self, other: &HttpCache) -> Self {
        self.merge(other);
        self
    }

    pub fn stats(&self) -> CacheStats {
        CacheStats {
            size: self.size(),
            hits: self.hits,
            misses: self.misses,
            hit_rate: self.hit_rate(),
            max_size: self.max_size,
            max_age: self.max_age,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct CacheStats {
    pub size: usize,
    pub hits: u64,
    pub misses: u64,
    pub hit_rate: f64,
    pub max_size: usize,
    pub max_age: Option<Duration>,
}

impl Default for HttpCache {
    fn default() -> Self {
        Self::new()
    }
}

impl From<HashMap<String, CacheEntry>> for HttpCache {
    fn from(entries: HashMap<String, CacheEntry>) -> Self {
        Self::from_map(entries)
    }
}

impl Into<HashMap<String, CacheEntry>> for HttpCache {
    fn into(self) -> HashMap<String, CacheEntry> {
        self.to_map()
    }
}

impl std::fmt::Display for HttpCache {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "Cache Stats:")?;
        writeln!(f, "  Size: {}", self.size())?;
        writeln!(f, "  Hits: {}", self.hits)?;
        writeln!(f, "  Misses: {}", self.misses)?;
        writeln!(f, "  Hit Rate: {:.2}%", self.hit_rate() * 100.0)?;
        writeln!(f, "  Max Size: {}", self.max_size)?;
        if let Some(max_age) = self.max_age {
            writeln!(f, "  Max Age: {:?}", max_age)?;
        }
        Ok(())
    }
}
