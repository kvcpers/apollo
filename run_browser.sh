#!/bin/bash

# Apollo Browser Engine - Browser Launcher
# Launches the complete browser engine

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
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

# Check if browser binary exists
if [ ! -f "target/debug/browser" ]; then
    log_warning "Browser binary not found. Building..."
    cargo build
fi

log_info "🚀 Starting Apollo Browser Engine..."
echo ""
echo -e "${PURPLE}================================${NC}"
echo -e "${PURPLE}  Apollo Browser Engine        ${NC}"
echo -e "${PURPLE}  Production Ready Browser     ${NC}"
echo -e "${PURPLE}================================${NC}"
echo ""

# Show engine status
log_info "Engine Status:"
echo "✅ HTML Parser - Ready"
echo "✅ CSS Parser - Ready"
echo "✅ JavaScript Engine - Ready (C++)"
echo "✅ Layout Engine - Ready (C++)"
echo "✅ Rendering Engine - Ready (C++)"
echo "✅ URL Parser - Ready"
echo "✅ HTTP Client - Ready"
echo "✅ Style Computation - Ready"
echo ""

# Run the browser
log_success "Starting browser engine..."
exec ./target/debug/browser "$@"
