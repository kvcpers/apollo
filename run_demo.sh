#!/bin/bash

# Apollo Browser Engine - Demo Launcher
# Launches the interactive demo showcasing all components

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -f "Cargo.toml" ]; then
    log_error "Not in the browser engine root directory"
    exit 1
fi

# Check if demo binary exists
if [ ! -f "target/debug/simple_demo" ]; then
    log_warning "Demo binary not found. Building..."
    cargo build --bin simple_demo
fi

log_info "🚀 Starting Apollo Browser Engine Demo..."
echo ""
echo -e "${PURPLE}================================${NC}"
echo -e "${PURPLE}  Apollo Browser Engine Demo   ${NC}"
echo -e "${PURPLE}  Interactive Component Demo    ${NC}"
echo -e "${PURPLE}================================${NC}"
echo ""

# Show demo features
log_info "Demo Features:"
echo "✅ HTML Parser Demo - Parse HTML and build DOM"
echo "✅ CSS Parser Demo - Parse CSS and compute styles"
echo "✅ JavaScript Demo - Execute JavaScript code"
echo "✅ URL Parser Demo - Parse URLs per WHATWG standard"
echo "✅ HTTP Client Demo - Make network requests"
echo "✅ Layout Demo - Calculate element positions"
echo "✅ Rendering Demo - Draw graphics and text"
echo "✅ Interactive Shell - Try commands yourself"
echo ""

# Run the demo
log_success "Starting simple demo..."
exec ./target/debug/simple_demo "$@"
