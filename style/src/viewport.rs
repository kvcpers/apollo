use serde::{Deserialize, Serialize};

/// Viewport information for responsive design
pub struct Viewport {
    width: f32,
    height: f32,
    device_pixel_ratio: f32,
    zoom: f32,
    min_zoom: f32,
    max_zoom: f32,
    user_zoom: bool,
    orientation: Orientation,
    initial_scale: f32,
    minimum_scale: f32,
    maximum_scale: f32,
    viewport_fit: ViewportFit,
    safe_area_insets: SafeAreaInsets,
}

/// Device orientation
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum Orientation {
    Portrait,
    Landscape,
}

/// Viewport fit behavior
#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize, Deserialize)]
pub enum ViewportFit {
    Auto,
    Contain,
    Cover,
}

/// Safe area insets for devices with notches
#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct SafeAreaInsets {
    pub top: f32,
    pub right: f32,
    pub bottom: f32,
    pub left: f32,
}

impl Viewport {
    pub fn new() -> Self {
        Self {
            width: 1024.0,
            height: 768.0,
            device_pixel_ratio: 1.0,
            zoom: 1.0,
            min_zoom: 0.25,
            max_zoom: 5.0,
            user_zoom: true,
            orientation: Orientation::Landscape,
            initial_scale: 1.0,
            minimum_scale: 0.25,
            maximum_scale: 5.0,
            viewport_fit: ViewportFit::Auto,
            safe_area_insets: SafeAreaInsets {
                top: 0.0,
                right: 0.0,
                bottom: 0.0,
                left: 0.0,
            },
        }
    }

    /// Set the viewport dimensions
    pub fn set_dimensions(&mut self, width: f32, height: f32) {
        self.width = width;
        self.height = height;
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

    /// Set the zoom level
    pub fn set_zoom(&mut self, zoom: f32) {
        self.zoom = zoom.clamp(self.min_zoom, self.max_zoom);
    }

    /// Set the minimum zoom level
    pub fn set_min_zoom(&mut self, min_zoom: f32) {
        self.min_zoom = min_zoom;
        if self.zoom < min_zoom {
            self.zoom = min_zoom;
        }
    }

    /// Set the maximum zoom level
    pub fn set_max_zoom(&mut self, max_zoom: f32) {
        self.max_zoom = max_zoom;
        if self.zoom > max_zoom {
            self.zoom = max_zoom;
        }
    }

    /// Set whether user zoom is enabled
    pub fn set_user_zoom(&mut self, enabled: bool) {
        self.user_zoom = enabled;
    }

    /// Set the orientation
    pub fn set_orientation(&mut self, orientation: Orientation) {
        self.orientation = orientation;
    }

    /// Set the initial scale
    pub fn set_initial_scale(&mut self, scale: f32) {
        self.initial_scale = scale;
    }

    /// Set the minimum scale
    pub fn set_minimum_scale(&mut self, scale: f32) {
        self.minimum_scale = scale;
    }

    /// Set the maximum scale
    pub fn set_maximum_scale(&mut self, scale: f32) {
        self.maximum_scale = scale;
    }

    /// Set the viewport fit behavior
    pub fn set_viewport_fit(&mut self, fit: ViewportFit) {
        self.viewport_fit = fit;
    }

    /// Set the safe area insets
    pub fn set_safe_area_insets(&mut self, insets: SafeAreaInsets) {
        self.safe_area_insets = insets;
    }

    /// Get the viewport width
    pub fn get_width(&self) -> f32 {
        self.width
    }

    /// Get the viewport height
    pub fn get_height(&self) -> f32 {
        self.height
    }

    /// Get the device pixel ratio
    pub fn get_device_pixel_ratio(&self) -> f32 {
        self.device_pixel_ratio
    }

    /// Get the zoom level
    pub fn get_zoom(&self) -> f32 {
        self.zoom
    }

    /// Get the minimum zoom level
    pub fn get_min_zoom(&self) -> f32 {
        self.min_zoom
    }

    /// Get the maximum zoom level
    pub fn get_max_zoom(&self) -> f32 {
        self.max_zoom
    }

    /// Check if user zoom is enabled
    pub fn is_user_zoom_enabled(&self) -> bool {
        self.user_zoom
    }

    /// Get the orientation
    pub fn get_orientation(&self) -> Orientation {
        self.orientation
    }

    /// Get the initial scale
    pub fn get_initial_scale(&self) -> f32 {
        self.initial_scale
    }

    /// Get the minimum scale
    pub fn get_minimum_scale(&self) -> f32 {
        self.minimum_scale
    }

    /// Get the maximum scale
    pub fn get_maximum_scale(&self) -> f32 {
        self.maximum_scale
    }

    /// Get the viewport fit behavior
    pub fn get_viewport_fit(&self) -> ViewportFit {
        self.viewport_fit
    }

    /// Get the safe area insets
    pub fn get_safe_area_insets(&self) -> &SafeAreaInsets {
        &self.safe_area_insets
    }

    /// Get the effective viewport dimensions (accounting for zoom)
    pub fn get_effective_dimensions(&self) -> (f32, f32) {
        (self.width / self.zoom, self.height / self.zoom)
    }

    /// Get the effective viewport width (accounting for zoom)
    pub fn get_effective_width(&self) -> f32 {
        self.width / self.zoom
    }

    /// Get the effective viewport height (accounting for zoom)
    pub fn get_effective_height(&self) -> f32 {
        self.height / self.zoom
    }

    /// Get the viewport dimensions in CSS pixels
    pub fn get_css_dimensions(&self) -> (f32, f32) {
        (
            self.width / self.device_pixel_ratio,
            self.height / self.device_pixel_ratio,
        )
    }

    /// Get the viewport width in CSS pixels
    pub fn get_css_width(&self) -> f32 {
        self.width / self.device_pixel_ratio
    }

    /// Get the viewport height in CSS pixels
    pub fn get_css_height(&self) -> f32 {
        self.height / self.device_pixel_ratio
    }

    /// Get the aspect ratio
    pub fn get_aspect_ratio(&self) -> f32 {
        self.width / self.height
    }

    /// Check if the viewport is in portrait mode
    pub fn is_portrait(&self) -> bool {
        self.orientation == Orientation::Portrait
    }

    /// Check if the viewport is in landscape mode
    pub fn is_landscape(&self) -> bool {
        self.orientation == Orientation::Landscape
    }

    /// Check if the viewport is square
    pub fn is_square(&self) -> bool {
        (self.width - self.height).abs() < f32::EPSILON
    }

    /// Get the viewport area
    pub fn get_area(&self) -> f32 {
        self.width * self.height
    }

    /// Get the viewport perimeter
    pub fn get_perimeter(&self) -> f32 {
        2.0 * (self.width + self.height)
    }

    /// Get the viewport diagonal
    pub fn get_diagonal(&self) -> f32 {
        (self.width * self.width + self.height * self.height).sqrt()
    }

    /// Check if the viewport is wider than it is tall
    pub fn is_wider_than_tall(&self) -> bool {
        self.width > self.height
    }

    /// Check if the viewport is taller than it is wide
    pub fn is_taller_than_wide(&self) -> bool {
        self.height > self.width
    }

    /// Get the viewport dimensions as a string
    pub fn get_dimensions_string(&self) -> String {
        format!("{}x{}", self.width as u32, self.height as u32)
    }

    /// Get the viewport information as a string
    pub fn get_info_string(&self) -> String {
        format!(
            "Viewport: {}x{} ({}), DPR: {}, Zoom: {}, Orientation: {:?}",
            self.width as u32,
            self.height as u32,
            self.orientation,
            self.device_pixel_ratio,
            self.zoom,
            self.orientation
        )
    }

    /// Update the viewport based on a new size
    pub fn update(&mut self, width: f32, height: f32) {
        self.set_dimensions(width, height);
    }

    /// Reset the viewport to default values
    pub fn reset(&mut self) {
        self.width = 1024.0;
        self.height = 768.0;
        self.device_pixel_ratio = 1.0;
        self.zoom = 1.0;
        self.min_zoom = 0.25;
        self.max_zoom = 5.0;
        self.user_zoom = true;
        self.orientation = Orientation::Landscape;
        self.initial_scale = 1.0;
        self.minimum_scale = 0.25;
        self.maximum_scale = 5.0;
        self.viewport_fit = ViewportFit::Auto;
        self.safe_area_insets = SafeAreaInsets {
            top: 0.0,
            right: 0.0,
            bottom: 0.0,
            left: 0.0,
        };
    }

    /// Clone the viewport
    pub fn clone(&self) -> Self {
        Self {
            width: self.width,
            height: self.height,
            device_pixel_ratio: self.device_pixel_ratio,
            zoom: self.zoom,
            min_zoom: self.min_zoom,
            max_zoom: self.max_zoom,
            user_zoom: self.user_zoom,
            orientation: self.orientation,
            initial_scale: self.initial_scale,
            minimum_scale: self.minimum_scale,
            maximum_scale: self.maximum_scale,
            viewport_fit: self.viewport_fit,
            safe_area_insets: self.safe_area_insets.clone(),
        }
    }
}

impl Default for Viewport {
    fn default() -> Self {
        Self::new()
    }
}

impl Clone for Viewport {
    fn clone(&self) -> Self {
        Self {
            width: self.width,
            height: self.height,
            device_pixel_ratio: self.device_pixel_ratio,
            zoom: self.zoom,
            min_zoom: self.min_zoom,
            max_zoom: self.max_zoom,
            user_zoom: self.user_zoom,
            orientation: self.orientation,
            initial_scale: self.initial_scale,
            minimum_scale: self.minimum_scale,
            maximum_scale: self.maximum_scale,
            viewport_fit: self.viewport_fit,
            safe_area_insets: self.safe_area_insets.clone(),
        }
    }
}

impl Clone for SafeAreaInsets {
    fn clone(&self) -> Self {
        Self {
            top: self.top,
            right: self.right,
            bottom: self.bottom,
            left: self.left,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_viewport_creation() {
        let viewport = Viewport::new();
        assert_eq!(viewport.get_width(), 1024.0);
        assert_eq!(viewport.get_height(), 768.0);
        assert_eq!(viewport.get_device_pixel_ratio(), 1.0);
        assert_eq!(viewport.get_zoom(), 1.0);
        assert_eq!(viewport.get_orientation(), Orientation::Landscape);
    }

    #[test]
    fn test_viewport_dimensions() {
        let mut viewport = Viewport::new();
        viewport.set_dimensions(800.0, 600.0);

        assert_eq!(viewport.get_width(), 800.0);
        assert_eq!(viewport.get_height(), 600.0);
        assert_eq!(viewport.get_orientation(), Orientation::Landscape);
        assert_eq!(viewport.get_aspect_ratio(), 800.0 / 600.0);
    }

    #[test]
    fn test_viewport_zoom() {
        let mut viewport = Viewport::new();
        viewport.set_zoom(2.0);

        assert_eq!(viewport.get_zoom(), 2.0);
        assert_eq!(viewport.get_effective_width(), 512.0);
        assert_eq!(viewport.get_effective_height(), 384.0);
    }

    #[test]
    fn test_viewport_device_pixel_ratio() {
        let mut viewport = Viewport::new();
        viewport.set_device_pixel_ratio(2.0);

        assert_eq!(viewport.get_device_pixel_ratio(), 2.0);
        assert_eq!(viewport.get_css_width(), 512.0);
        assert_eq!(viewport.get_css_height(), 384.0);
    }

    #[test]
    fn test_viewport_orientation() {
        let mut viewport = Viewport::new();
        viewport.set_dimensions(600.0, 800.0);

        assert_eq!(viewport.get_orientation(), Orientation::Portrait);
        assert!(viewport.is_portrait());
        assert!(!viewport.is_landscape());
    }

    #[test]
    fn test_viewport_safe_area_insets() {
        let mut viewport = Viewport::new();
        let insets = SafeAreaInsets {
            top: 20.0,
            right: 0.0,
            bottom: 0.0,
            left: 0.0,
        };
        viewport.set_safe_area_insets(insets);

        let retrieved_insets = viewport.get_safe_area_insets();
        assert_eq!(retrieved_insets.top, 20.0);
        assert_eq!(retrieved_insets.right, 0.0);
        assert_eq!(retrieved_insets.bottom, 0.0);
        assert_eq!(retrieved_insets.left, 0.0);
    }

    #[test]
    fn test_viewport_zoom_limits() {
        let mut viewport = Viewport::new();
        viewport.set_min_zoom(0.5);
        viewport.set_max_zoom(3.0);

        viewport.set_zoom(0.1); // Should be clamped to 0.5
        assert_eq!(viewport.get_zoom(), 0.5);

        viewport.set_zoom(5.0); // Should be clamped to 3.0
        assert_eq!(viewport.get_zoom(), 3.0);
    }

    #[test]
    fn test_viewport_info_string() {
        let viewport = Viewport::new();
        let info = viewport.get_info_string();
        assert!(info.contains("1024x768"));
        assert!(info.contains("Landscape"));
        assert!(info.contains("DPR: 1"));
        assert!(info.contains("Zoom: 1"));
    }

    #[test]
    fn test_viewport_update() {
        let mut viewport = Viewport::new();
        viewport.update(1920.0, 1080.0);

        assert_eq!(viewport.get_width(), 1920.0);
        assert_eq!(viewport.get_height(), 1080.0);
        assert_eq!(viewport.get_orientation(), Orientation::Landscape);
    }

    #[test]
    fn test_viewport_reset() {
        let mut viewport = Viewport::new();
        viewport.set_dimensions(800.0, 600.0);
        viewport.set_zoom(2.0);
        viewport.set_device_pixel_ratio(2.0);

        viewport.reset();

        assert_eq!(viewport.get_width(), 1024.0);
        assert_eq!(viewport.get_height(), 768.0);
        assert_eq!(viewport.get_zoom(), 1.0);
        assert_eq!(viewport.get_device_pixel_ratio(), 1.0);
    }
}
