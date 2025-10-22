use serde::{Deserialize, Serialize};
use std::collections::HashMap;

/// Media query matcher for responsive design
pub struct MediaQueryMatcher {
    current_media: MediaType,
    viewport_width: f32,
    viewport_height: f32,
    device_pixel_ratio: f32,
    orientation: Orientation,
    color_depth: u32,
    monochrome: bool,
    scan: ScanType,
    grid: bool,
    update_frequency: UpdateFrequency,
    hover_capability: HoverCapability,
    pointer_capability: PointerCapability,
}

/// Media type enumeration
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum MediaType {
    All,
    Screen,
    Print,
    Speech,
    Braille,
    Embossed,
    Handheld,
    Projection,
    Tty,
    Tv,
}

/// Device orientation
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum Orientation {
    Portrait,
    Landscape,
}

/// Scan type for TV displays
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum ScanType {
    Progressive,
    Interlace,
}

/// Update frequency for displays
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum UpdateFrequency {
    None,
    Slow,
    Normal,
}

/// Hover capability
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum HoverCapability {
    None,
    Hover,
}

/// Pointer capability
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum PointerCapability {
    None,
    Coarse,
    Fine,
}

/// Media query feature
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub enum MediaFeature {
    Width(f32),
    MinWidth(f32),
    MaxWidth(f32),
    Height(f32),
    MinHeight(f32),
    MaxHeight(f32),
    AspectRatio(f32, f32),
    MinAspectRatio(f32, f32),
    MaxAspectRatio(f32, f32),
    Orientation(Orientation),
    Resolution(f32),
    MinResolution(f32),
    MaxResolution(f32),
    Scan(ScanType),
    Grid(bool),
    Update(UpdateFrequency),
    OverflowBlock(Orientation),
    OverflowInline(Orientation),
    Color(u32),
    MinColor(u32),
    MaxColor(u32),
    ColorIndex(u32),
    MinColorIndex(u32),
    MaxColorIndex(u32),
    Monochrome(bool),
    MinMonochrome(bool),
    MaxMonochrome(bool),
    ColorGamut(String),
    Pointer(HoverCapability),
    Hover(HoverCapability),
    AnyPointer(PointerCapability),
    AnyHover(HoverCapability),
    PrefersReducedMotion(bool),
    PrefersReducedTransparency(bool),
    PrefersColorScheme(String),
    PrefersContrast(String),
    ForcedColors(String),
    Scripting(String),
}

/// Media query
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct MediaQuery {
    pub media_type: Option<MediaType>,
    pub features: Vec<MediaFeature>,
    pub negated: bool,
}

impl MediaQueryMatcher {
    pub fn new() -> Self {
        Self {
            current_media: MediaType::Screen,
            viewport_width: 1024.0,
            viewport_height: 768.0,
            device_pixel_ratio: 1.0,
            orientation: Orientation::Landscape,
            color_depth: 24,
            monochrome: false,
            scan: ScanType::Progressive,
            grid: false,
            update_frequency: UpdateFrequency::Normal,
            hover_capability: HoverCapability::Hover,
            pointer_capability: PointerCapability::Fine,
        }
    }

    /// Set the current media type
    pub fn set_media_type(&mut self, media_type: MediaType) {
        self.current_media = media_type;
    }

    /// Set the viewport dimensions
    pub fn set_viewport(&mut self, width: f32, height: f32) {
        self.viewport_width = width;
        self.viewport_height = height;
        self.orientation = if width > height {
            Orientation::Landscape
        } else {
            Orientation::Portrait
        };
    }

    /// Set the device pixel ratio
    pub fn set_device_pixel_ratio(&mut self, ratio: f32) {
        self.device_pixel_ratio = ratio;
    }

    /// Set the color depth
    pub fn set_color_depth(&mut self, depth: u32) {
        self.color_depth = depth;
    }

    /// Set the monochrome flag
    pub fn set_monochrome(&mut self, monochrome: bool) {
        self.monochrome = monochrome;
    }

    /// Set the scan type
    pub fn set_scan(&mut self, scan: ScanType) {
        self.scan = scan;
    }

    /// Set the grid flag
    pub fn set_grid(&mut self, grid: bool) {
        self.grid = grid;
    }

    /// Set the update frequency
    pub fn set_update_frequency(&mut self, frequency: UpdateFrequency) {
        self.update_frequency = frequency;
    }

    /// Set the hover capability
    pub fn set_hover_capability(&mut self, capability: HoverCapability) {
        self.hover_capability = capability;
    }

    /// Set the pointer capability
    pub fn set_pointer_capability(&mut self, capability: PointerCapability) {
        self.pointer_capability = capability;
    }

    /// Check if a media query matches the current media
    pub fn matches(&self, media_query: &MediaQuery) -> bool {
        let mut matches = true;

        // Check media type
        if let Some(media_type) = media_query.media_type {
            if media_type != self.current_media && media_type != MediaType::All {
                matches = false;
            }
        }

        // Check features
        for feature in &media_query.features {
            if !self.matches_feature(feature) {
                matches = false;
                break;
            }
        }

        // Apply negation
        if media_query.negated {
            matches = !matches;
        }

        matches
    }

    /// Check if a media feature matches
    fn matches_feature(&self, feature: &MediaFeature) -> bool {
        match feature {
            MediaFeature::Width(value) => (self.viewport_width - *value).abs() < f32::EPSILON,
            MediaFeature::MinWidth(value) => self.viewport_width >= *value,
            MediaFeature::MaxWidth(value) => self.viewport_width <= *value,
            MediaFeature::Height(value) => (self.viewport_height - *value).abs() < f32::EPSILON,
            MediaFeature::MinHeight(value) => self.viewport_height >= *value,
            MediaFeature::MaxHeight(value) => self.viewport_height <= *value,
            MediaFeature::AspectRatio(w, h) => {
                let aspect_ratio = self.viewport_width / self.viewport_height;
                let target_ratio = w / h;
                (aspect_ratio - target_ratio).abs() < f32::EPSILON
            }
            MediaFeature::MinAspectRatio(w, h) => {
                let aspect_ratio = self.viewport_width / self.viewport_height;
                let target_ratio = w / h;
                aspect_ratio >= target_ratio
            }
            MediaFeature::MaxAspectRatio(w, h) => {
                let aspect_ratio = self.viewport_width / self.viewport_height;
                let target_ratio = w / h;
                aspect_ratio <= target_ratio
            }
            MediaFeature::Orientation(orientation) => self.orientation == *orientation,
            MediaFeature::Resolution(value) => {
                (self.device_pixel_ratio - *value).abs() < f32::EPSILON
            }
            MediaFeature::MinResolution(value) => self.device_pixel_ratio >= *value,
            MediaFeature::MaxResolution(value) => self.device_pixel_ratio <= *value,
            MediaFeature::Scan(scan) => self.scan == *scan,
            MediaFeature::Grid(grid) => self.grid == *grid,
            MediaFeature::Update(frequency) => self.update_frequency == *frequency,
            MediaFeature::OverflowBlock(orientation) => self.orientation == *orientation,
            MediaFeature::OverflowInline(orientation) => self.orientation == *orientation,
            MediaFeature::Color(bits) => self.color_depth >= *bits,
            MediaFeature::MinColor(bits) => self.color_depth >= *bits,
            MediaFeature::MaxColor(bits) => self.color_depth <= *bits,
            MediaFeature::ColorIndex(index) => {
                // Simplified implementation
                self.color_depth >= *index
            }
            MediaFeature::MinColorIndex(index) => {
                // Simplified implementation
                self.color_depth >= *index
            }
            MediaFeature::MaxColorIndex(index) => {
                // Simplified implementation
                self.color_depth <= *index
            }
            MediaFeature::Monochrome(monochrome) => self.monochrome == *monochrome,
            MediaFeature::MinMonochrome(monochrome) => self.monochrome == *monochrome,
            MediaFeature::MaxMonochrome(monochrome) => self.monochrome == *monochrome,
            MediaFeature::ColorGamut(gamut) => {
                // Simplified implementation
                gamut == "srgb"
            }
            MediaFeature::Pointer(capability) => self.pointer_capability == *capability,
            MediaFeature::Hover(capability) => self.hover_capability == *capability,
            MediaFeature::AnyPointer(capability) => self.pointer_capability == *capability,
            MediaFeature::AnyHover(capability) => self.hover_capability == *capability,
            MediaFeature::PrefersReducedMotion(reduced) => {
                // Simplified implementation - assume user prefers reduced motion
                *reduced
            }
            MediaFeature::PrefersReducedTransparency(reduced) => {
                // Simplified implementation - assume user prefers reduced transparency
                *reduced
            }
            MediaFeature::PrefersColorScheme(scheme) => {
                // Simplified implementation - assume light scheme
                scheme == "light"
            }
            MediaFeature::PrefersContrast(contrast) => {
                // Simplified implementation - assume normal contrast
                contrast == "normal"
            }
            MediaFeature::ForcedColors(forced) => {
                // Simplified implementation - assume no forced colors
                forced == "none"
            }
            MediaFeature::Scripting(scripting) => {
                // Simplified implementation - assume scripting is enabled
                scripting == "enabled"
            }
        }
    }

    /// Parse a media query string
    pub fn parse_media_query(&self, query: &str) -> Result<MediaQuery, String> {
        // Simplified media query parsing
        // In a real implementation, this would be much more complex

        let query = query.trim();
        let mut media_type = None;
        let mut features = Vec::new();
        let mut negated = false;

        if query.starts_with("not ") {
            negated = true;
            let query = &query[4..];

            if query == "all" {
                media_type = Some(MediaType::All);
            } else if query == "screen" {
                media_type = Some(MediaType::Screen);
            } else if query == "print" {
                media_type = Some(MediaType::Print);
            } else if query == "speech" {
                media_type = Some(MediaType::Speech);
            } else {
                return Err("Invalid media type".to_string());
            }
        } else {
            // Parse media type and features
            let parts: Vec<&str> = query.split(" and ").collect();

            if parts.is_empty() {
                return Err("Empty media query".to_string());
            }

            // First part is the media type
            let first_part = parts[0].trim();
            if first_part == "all" {
                media_type = Some(MediaType::All);
            } else if first_part == "screen" {
                media_type = Some(MediaType::Screen);
            } else if first_part == "print" {
                media_type = Some(MediaType::Print);
            } else if first_part == "speech" {
                media_type = Some(MediaType::Speech);
            } else {
                // Assume it's a feature
                if let Some(feature) = self.parse_feature(first_part) {
                    features.push(feature);
                }
            }

            // Parse remaining features
            for part in parts.iter().skip(1) {
                if let Some(feature) = self.parse_feature(part.trim()) {
                    features.push(feature);
                }
            }
        }

        Ok(MediaQuery {
            media_type,
            features,
            negated,
        })
    }

    /// Parse a media feature
    fn parse_feature(&self, feature: &str) -> Option<MediaFeature> {
        if feature.starts_with("width:") {
            if let Ok(value) = feature[6..].parse::<f32>() {
                return Some(MediaFeature::Width(value));
            }
        } else if feature.starts_with("min-width:") {
            if let Ok(value) = feature[10..].parse::<f32>() {
                return Some(MediaFeature::MinWidth(value));
            }
        } else if feature.starts_with("max-width:") {
            if let Ok(value) = feature[10..].parse::<f32>() {
                return Some(MediaFeature::MaxWidth(value));
            }
        } else if feature.starts_with("height:") {
            if let Ok(value) = feature[7..].parse::<f32>() {
                return Some(MediaFeature::Height(value));
            }
        } else if feature.starts_with("min-height:") {
            if let Ok(value) = feature[11..].parse::<f32>() {
                return Some(MediaFeature::MinHeight(value));
            }
        } else if feature.starts_with("max-height:") {
            if let Ok(value) = feature[11..].parse::<f32>() {
                return Some(MediaFeature::MaxHeight(value));
            }
        } else if feature.starts_with("orientation:") {
            let orientation = feature[12..].trim();
            if orientation == "portrait" {
                return Some(MediaFeature::Orientation(Orientation::Portrait));
            } else if orientation == "landscape" {
                return Some(MediaFeature::Orientation(Orientation::Landscape));
            }
        } else if feature.starts_with("resolution:") {
            if let Ok(value) = feature[11..].parse::<f32>() {
                return Some(MediaFeature::Resolution(value));
            }
        } else if feature.starts_with("min-resolution:") {
            if let Ok(value) = feature[15..].parse::<f32>() {
                return Some(MediaFeature::MinResolution(value));
            }
        } else if feature.starts_with("max-resolution:") {
            if let Ok(value) = feature[15..].parse::<f32>() {
                return Some(MediaFeature::MaxResolution(value));
            }
        } else if feature.starts_with("scan:") {
            let scan = feature[5..].trim();
            if scan == "progressive" {
                return Some(MediaFeature::Scan(ScanType::Progressive));
            } else if scan == "interlace" {
                return Some(MediaFeature::Scan(ScanType::Interlace));
            }
        } else if feature.starts_with("grid:") {
            let grid = feature[5..].trim();
            if grid == "1" {
                return Some(MediaFeature::Grid(true));
            } else if grid == "0" {
                return Some(MediaFeature::Grid(false));
            }
        } else if feature.starts_with("update:") {
            let update = feature[7..].trim();
            if update == "none" {
                return Some(MediaFeature::Update(UpdateFrequency::None));
            } else if update == "slow" {
                return Some(MediaFeature::Update(UpdateFrequency::Slow));
            } else if update == "normal" {
                return Some(MediaFeature::Update(UpdateFrequency::Normal));
            }
        } else if feature.starts_with("pointer:") {
            let pointer = feature[8..].trim();
            if pointer == "none" {
                return Some(MediaFeature::Pointer(HoverCapability::None));
            } else if pointer == "coarse" {
                return Some(MediaFeature::Pointer(HoverCapability::Hover));
            } else if pointer == "fine" {
                return Some(MediaFeature::Pointer(HoverCapability::Hover));
            }
        } else if feature.starts_with("hover:") {
            let hover = feature[6..].trim();
            if hover == "none" {
                return Some(MediaFeature::Hover(HoverCapability::None));
            } else if hover == "hover" {
                return Some(MediaFeature::Hover(HoverCapability::Hover));
            }
        } else if feature.starts_with("any-pointer:") {
            let pointer = feature[12..].trim();
            if pointer == "none" {
                return Some(MediaFeature::AnyPointer(PointerCapability::None));
            } else if pointer == "coarse" {
                return Some(MediaFeature::AnyPointer(PointerCapability::Coarse));
            } else if pointer == "fine" {
                return Some(MediaFeature::AnyPointer(PointerCapability::Fine));
            }
        } else if feature.starts_with("any-hover:") {
            let hover = feature[10..].trim();
            if hover == "none" {
                return Some(MediaFeature::AnyHover(HoverCapability::None));
            } else if hover == "hover" {
                return Some(MediaFeature::AnyHover(HoverCapability::Hover));
            }
        } else if feature.starts_with("prefers-reduced-motion:") {
            let reduced = feature[23..].trim();
            if reduced == "reduce" {
                return Some(MediaFeature::PrefersReducedMotion(true));
            } else if reduced == "no-preference" {
                return Some(MediaFeature::PrefersReducedMotion(false));
            }
        } else if feature.starts_with("prefers-reduced-transparency:") {
            let reduced = feature[29..].trim();
            if reduced == "reduce" {
                return Some(MediaFeature::PrefersReducedTransparency(true));
            } else if reduced == "no-preference" {
                return Some(MediaFeature::PrefersReducedTransparency(false));
            }
        } else if feature.starts_with("prefers-color-scheme:") {
            let scheme = feature[21..].trim();
            return Some(MediaFeature::PrefersColorScheme(scheme.to_string()));
        } else if feature.starts_with("prefers-contrast:") {
            let contrast = feature[17..].trim();
            return Some(MediaFeature::PrefersContrast(contrast.to_string()));
        } else if feature.starts_with("forced-colors:") {
            let forced = feature[14..].trim();
            return Some(MediaFeature::ForcedColors(forced.to_string()));
        } else if feature.starts_with("scripting:") {
            let scripting = feature[10..].trim();
            return Some(MediaFeature::Scripting(scripting.to_string()));
        }

        None
    }

    /// Get the current media type
    pub fn get_media_type(&self) -> MediaType {
        self.current_media
    }

    /// Get the viewport dimensions
    pub fn get_viewport(&self) -> (f32, f32) {
        (self.viewport_width, self.viewport_height)
    }

    /// Get the device pixel ratio
    pub fn get_device_pixel_ratio(&self) -> f32 {
        self.device_pixel_ratio
    }

    /// Get the orientation
    pub fn get_orientation(&self) -> Orientation {
        self.orientation
    }

    /// Get the color depth
    pub fn get_color_depth(&self) -> u32 {
        self.color_depth
    }

    /// Get the monochrome flag
    pub fn get_monochrome(&self) -> bool {
        self.monochrome
    }

    /// Get the scan type
    pub fn get_scan(&self) -> ScanType {
        self.scan
    }

    /// Get the grid flag
    pub fn get_grid(&self) -> bool {
        self.grid
    }

    /// Get the update frequency
    pub fn get_update_frequency(&self) -> UpdateFrequency {
        self.update_frequency
    }

    /// Get the hover capability
    pub fn get_hover_capability(&self) -> HoverCapability {
        self.hover_capability
    }

    /// Get the pointer capability
    pub fn get_pointer_capability(&self) -> PointerCapability {
        self.pointer_capability
    }
}

impl Default for MediaQueryMatcher {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_media_query_matcher_creation() {
        let matcher = MediaQueryMatcher::new();
        assert_eq!(matcher.get_media_type(), MediaType::Screen);
        assert_eq!(matcher.get_viewport(), (1024.0, 768.0));
        assert_eq!(matcher.get_device_pixel_ratio(), 1.0);
        assert_eq!(matcher.get_orientation(), Orientation::Landscape);
    }

    #[test]
    fn test_media_query_matching() {
        let mut matcher = MediaQueryMatcher::new();
        matcher.set_viewport(800.0, 600.0);

        let media_query = MediaQuery {
            media_type: Some(MediaType::Screen),
            features: vec![MediaFeature::MaxWidth(1024.0)],
            negated: false,
        };

        assert!(matcher.matches(&media_query));
    }

    #[test]
    fn test_media_query_parsing() {
        let matcher = MediaQueryMatcher::new();

        let query = matcher
            .parse_media_query("screen and (max-width: 1024px)")
            .unwrap();
        assert_eq!(query.media_type, Some(MediaType::Screen));
        assert_eq!(query.features.len(), 1);
        assert!(!query.negated);

        if let MediaFeature::MaxWidth(value) = &query.features[0] {
            assert_eq!(*value, 1024.0);
        } else {
            panic!("Expected MaxWidth feature");
        }
    }

    #[test]
    fn test_media_feature_matching() {
        let mut matcher = MediaQueryMatcher::new();
        matcher.set_viewport(800.0, 600.0);

        assert!(matcher.matches_feature(&MediaFeature::MaxWidth(1024.0)));
        assert!(!matcher.matches_feature(&MediaFeature::MaxWidth(600.0)));
        assert!(matcher.matches_feature(&MediaFeature::MinWidth(600.0)));
        assert!(!matcher.matches_feature(&MediaFeature::MinWidth(900.0)));

        assert!(matcher.matches_feature(&MediaFeature::MaxHeight(800.0)));
        assert!(!matcher.matches_feature(&MediaFeature::MaxHeight(500.0)));
        assert!(matcher.matches_feature(&MediaFeature::MinHeight(500.0)));
        assert!(!matcher.matches_feature(&MediaFeature::MinHeight(700.0)));

        assert!(matcher.matches_feature(&MediaFeature::Orientation(Orientation::Landscape)));
        assert!(!matcher.matches_feature(&MediaFeature::Orientation(Orientation::Portrait)));
    }

    #[test]
    fn test_negated_media_query() {
        let mut matcher = MediaQueryMatcher::new();
        matcher.set_viewport(800.0, 600.0);

        let media_query = MediaQuery {
            media_type: Some(MediaType::Screen),
            features: vec![MediaFeature::MaxWidth(600.0)],
            negated: true,
        };

        assert!(matcher.matches(&media_query));
    }
}
