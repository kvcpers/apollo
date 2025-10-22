use std::collections::HashMap;
use std::sync::Arc;
use tokio::sync::RwLock;

/// Event types that can be dispatched
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum EventType {
    // DOM Events
    Click,
    DoubleClick,
    MouseDown,
    MouseUp,
    MouseMove,
    MouseOver,
    MouseOut,
    KeyDown,
    KeyUp,
    KeyPress,
    Focus,
    Blur,
    Submit,
    Change,
    Input,
    Load,
    Unload,
    Resize,
    Scroll,

    // Navigation Events
    NavigationStart,
    NavigationComplete,
    NavigationError,

    // Tab Events
    TabCreated,
    TabClosed,
    TabActivated,
    TabUpdated,

    // Window Events
    WindowCreated,
    WindowClosed,
    WindowActivated,
    WindowResized,
    WindowMoved,

    // Custom Events
    Custom(String),
}

impl EventType {
    /// Convert string to event type
    pub fn from_string(name: &str) -> Self {
        match name.to_lowercase().as_str() {
            "click" => EventType::Click,
            "dblclick" | "doubleclick" => EventType::DoubleClick,
            "mousedown" => EventType::MouseDown,
            "mouseup" => EventType::MouseUp,
            "mousemove" => EventType::MouseMove,
            "mouseover" => EventType::MouseOver,
            "mouseout" => EventType::MouseOut,
            "keydown" => EventType::KeyDown,
            "keyup" => EventType::KeyUp,
            "keypress" => EventType::KeyPress,
            "focus" => EventType::Focus,
            "blur" => EventType::Blur,
            "submit" => EventType::Submit,
            "change" => EventType::Change,
            "input" => EventType::Input,
            "load" => EventType::Load,
            "unload" => EventType::Unload,
            "resize" => EventType::Resize,
            "scroll" => EventType::Scroll,
            _ => EventType::Custom(name.to_string()),
        }
    }

    /// Convert event type to string
    #[allow(clippy::inherent_to_string)]
    pub fn to_string(&self) -> String {
        match self {
            EventType::Click => "click".to_string(),
            EventType::DoubleClick => "dblclick".to_string(),
            EventType::MouseDown => "mousedown".to_string(),
            EventType::MouseUp => "mouseup".to_string(),
            EventType::MouseMove => "mousemove".to_string(),
            EventType::MouseOver => "mouseover".to_string(),
            EventType::MouseOut => "mouseout".to_string(),
            EventType::KeyDown => "keydown".to_string(),
            EventType::KeyUp => "keyup".to_string(),
            EventType::KeyPress => "keypress".to_string(),
            EventType::Focus => "focus".to_string(),
            EventType::Blur => "blur".to_string(),
            EventType::Submit => "submit".to_string(),
            EventType::Change => "change".to_string(),
            EventType::Input => "input".to_string(),
            EventType::Load => "load".to_string(),
            EventType::Unload => "unload".to_string(),
            EventType::Resize => "resize".to_string(),
            EventType::Scroll => "scroll".to_string(),
            EventType::NavigationStart => "navigationstart".to_string(),
            EventType::NavigationComplete => "navigationcomplete".to_string(),
            EventType::NavigationError => "navigationerror".to_string(),
            EventType::TabCreated => "tabcreated".to_string(),
            EventType::TabClosed => "tabclosed".to_string(),
            EventType::TabActivated => "tabactivated".to_string(),
            EventType::TabUpdated => "tabupdated".to_string(),
            EventType::WindowCreated => "windowcreated".to_string(),
            EventType::WindowClosed => "windowclosed".to_string(),
            EventType::WindowActivated => "windowactivated".to_string(),
            EventType::WindowResized => "windowresized".to_string(),
            EventType::WindowMoved => "windowmoved".to_string(),
            EventType::Custom(name) => name.clone(),
        }
    }
}

/// Event data
#[derive(Debug, Clone)]
pub struct EventData {
    pub event_type: EventType,
    pub target_id: Option<String>,
    pub timestamp: u64,
    pub bubbles: bool,
    pub cancelable: bool,
    pub default_prevented: bool,
    pub propagation_stopped: bool,
    pub data: HashMap<String, serde_json::Value>,
}

impl EventData {
    pub fn new(event_type: EventType) -> Self {
        Self {
            event_type,
            target_id: None,
            timestamp: std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap_or_default()
                .as_millis() as u64,
            bubbles: true,
            cancelable: true,
            default_prevented: false,
            propagation_stopped: false,
            data: HashMap::new(),
        }
    }

    pub fn with_target(mut self, target_id: String) -> Self {
        self.target_id = Some(target_id);
        self
    }

    pub fn with_data(mut self, key: String, value: serde_json::Value) -> Self {
        self.data.insert(key, value);
        self
    }

    pub fn prevent_default(&mut self) {
        if self.cancelable {
            self.default_prevented = true;
        }
    }

    pub fn stop_propagation(&mut self) {
        self.propagation_stopped = true;
    }
}

/// Event listener function type
pub type EventListener = Box<dyn Fn(&EventData) + Send + Sync>;

/// Event target that can receive and dispatch events
pub struct EventTarget {
    listeners: HashMap<EventType, Vec<EventListener>>,
    parent: Option<Arc<RwLock<EventTarget>>>,
}

impl EventTarget {
    pub fn new() -> Self {
        Self {
            listeners: HashMap::new(),
            parent: None,
        }
    }

    pub fn with_parent(parent: Arc<RwLock<EventTarget>>) -> Self {
        Self {
            listeners: HashMap::new(),
            parent: Some(parent),
        }
    }

    /// Add an event listener
    pub fn add_event_listener(&mut self, event_type: EventType, listener: EventListener) {
        self.listeners
            .entry(event_type)
            .or_default()
            .push(listener);
    }

    /// Remove an event listener
    pub fn remove_event_listener(&mut self, event_type: EventType, listener_index: usize) {
        if let Some(listeners) = self.listeners.get_mut(&event_type) {
            if listener_index < listeners.len() {
                let _ = listeners.remove(listener_index);
            }
        }
    }

    /// Dispatch an event
    pub async fn dispatch_event(&self, event: EventData) {
        // Call listeners for this target
        if let Some(listeners) = self.listeners.get(&event.event_type) {
            for listener in listeners {
                listener(&event);

                if event.propagation_stopped {
                    return;
                }
            }
        }

        // Bubble up to parent if event bubbles
        if event.bubbles && !event.propagation_stopped {
            if let Some(parent) = &self.parent {
                let parent = parent.read().await;
                Box::pin(parent.dispatch_event(event)).await;
            }
        }
    }

    /// Check if has listeners for event type
    pub fn has_listeners(&self, event_type: &EventType) -> bool {
        self.listeners
            .get(event_type)
            .is_some_and(|listeners| !listeners.is_empty())
    }

    /// Get listener count for event type
    pub fn get_listener_count(&self, event_type: &EventType) -> usize {
        self.listeners
            .get(event_type)
            .map_or(0, |listeners| listeners.len())
    }

    /// Set parent event target
    pub fn set_parent(&mut self, parent: Arc<RwLock<EventTarget>>) {
        self.parent = Some(parent);
    }

    /// Clear parent event target
    pub fn clear_parent(&mut self) {
        self.parent = None;
    }
}

impl Default for EventTarget {
    fn default() -> Self {
        Self::new()
    }
}

/// Event dispatcher for global events
pub struct EventDispatcher {
    targets: HashMap<String, Arc<RwLock<EventTarget>>>,
}

impl EventDispatcher {
    pub fn new() -> Self {
        Self {
            targets: HashMap::new(),
        }
    }

    /// Register an event target
    pub fn register_target(&mut self, id: String, target: Arc<RwLock<EventTarget>>) {
        self.targets.insert(id, target);
    }

    /// Unregister an event target
    pub fn unregister_target(&mut self, id: &str) {
        self.targets.remove(id);
    }

    /// Dispatch event to specific target
    pub async fn dispatch_to_target(&self, target_id: &str, event: EventData) {
        if let Some(target) = self.targets.get(target_id) {
            let target = target.read().await;
            target.dispatch_event(event).await;
        }
    }

    /// Broadcast event to all targets
    pub async fn broadcast(&self, event: EventData) {
        for target in self.targets.values() {
            let target = target.read().await;
            target.dispatch_event(event.clone()).await;
        }
    }

    /// Get target by ID
    pub fn get_target(&self, id: &str) -> Option<&Arc<RwLock<EventTarget>>> {
        self.targets.get(id)
    }

    /// Get all registered target IDs
    pub fn get_target_ids(&self) -> Vec<&String> {
        self.targets.keys().collect()
    }

    /// Get target count
    pub fn get_target_count(&self) -> usize {
        self.targets.len()
    }
}

impl Default for EventDispatcher {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::atomic::{AtomicU32, Ordering};

    #[test]
    fn test_event_type_conversion() {
        assert_eq!(EventType::from_string("click"), EventType::Click);
        assert_eq!(EventType::from_string("dblclick"), EventType::DoubleClick);
        assert_eq!(
            EventType::from_string("custom_event"),
            EventType::Custom("custom_event".to_string())
        );

        assert_eq!(EventType::Click.to_string(), "click");
        assert_eq!(EventType::DoubleClick.to_string(), "dblclick");
        assert_eq!(EventType::Custom("test".to_string()).to_string(), "test");
    }

    #[test]
    fn test_event_data() {
        let event = EventData::new(EventType::Click)
            .with_target("button1".to_string())
            .with_data("x".to_string(), serde_json::Value::Number(100.into()))
            .with_data("y".to_string(), serde_json::Value::Number(200.into()));

        assert_eq!(event.event_type, EventType::Click);
        assert_eq!(event.target_id, Some("button1".to_string()));
        assert!(event.bubbles);
        assert!(event.cancelable);
        assert!(!event.default_prevented);
        assert!(!event.propagation_stopped);
        assert_eq!(
            event.data.get("x"),
            Some(&serde_json::Value::Number(100.into()))
        );
        assert_eq!(
            event.data.get("y"),
            Some(&serde_json::Value::Number(200.into()))
        );
    }

    #[test]
    fn test_event_target_listeners() {
        let mut target = EventTarget::new();

        let counter = Arc::new(AtomicU32::new(0));
        let counter_clone = counter.clone();

        let listener = Box::new(move |_event: &EventData| {
            counter_clone.fetch_add(1, Ordering::SeqCst);
        });

        target.add_event_listener(EventType::Click, listener);

        assert!(target.has_listeners(&EventType::Click));
        assert_eq!(target.get_listener_count(&EventType::Click), 1);
        assert!(!target.has_listeners(&EventType::KeyDown));
        assert_eq!(target.get_listener_count(&EventType::KeyDown), 0);
    }

    #[test]
    fn test_event_dispatcher() {
        let mut dispatcher = EventDispatcher::new();

        let target1 = Arc::new(RwLock::new(EventTarget::new()));
        let target2 = Arc::new(RwLock::new(EventTarget::new()));

        dispatcher.register_target("target1".to_string(), target1);
        dispatcher.register_target("target2".to_string(), target2);

        assert_eq!(dispatcher.get_target_count(), 2);
        assert!(dispatcher.get_target("target1").is_some());
        assert!(dispatcher.get_target("target2").is_some());
        assert!(dispatcher.get_target("target3").is_none());

        dispatcher.unregister_target("target1");
        assert_eq!(dispatcher.get_target_count(), 1);
        assert!(dispatcher.get_target("target1").is_none());
    }
}
