#!/bin/bash

# Browser Engine Build Script

set -e

echo "Building Browser Engine..."

# Check if Rust is installed
if ! command -v cargo &> /dev/null; then
    echo "Error: Cargo is not installed. Please install Rust first."
    exit 1
fi

# Check if CMake is installed (for future C++ components)
if ! command -v cmake &> /dev/null; then
    echo "Warning: CMake is not installed. C++ components will not be built."
fi

# Clean previous builds
echo "Cleaning previous builds..."
cargo clean

# Format code
echo "Formatting code..."
cargo fmt

# Run clippy for linting
echo "Running clippy..."
cargo clippy --all-targets --all-features -- -D warnings

# Run tests
echo "Running tests..."
cargo test --all

# Build in release mode
echo "Building in release mode..."
cargo build --release

# Build in debug mode for development
echo "Building in debug mode..."
cargo build

echo "Build complete!"
echo ""
echo "To run the browser:"
echo "  cargo run"
echo ""
echo "To run tests:"
echo "  cargo test"
echo ""
echo "To run specific component tests:"
echo "  cargo test -p html-parser"
echo "  cargo test -p css-parser"
echo "  cargo test -p browser-core"
