# Apollo Browser Engine - Production Makefile
# Builds, tests, and packages the complete browser engine

.PHONY: all build test clean install package release docker

# Default target
all: build test

# Build configuration
RUST_TARGET ?= release
CARGO_FLAGS ?= --release
CMAKE_BUILD_TYPE ?= Release
INSTALL_PREFIX ?= /usr/local

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[1;33m
BLUE = \033[0;34m
NC = \033[0m

# Print colored output
print_info = @echo -e "$(BLUE)[INFO]$(NC) $1"
print_success = @echo -e "$(GREEN)[SUCCESS]$(NC) $1"
print_warning = @echo -e "$(YELLOW)[WARNING]$(NC) $1"
print_error = @echo -e "$(RED)[ERROR]$(NC) $1"

# Check dependencies
check-deps:
	$(call print_info,Checking dependencies...)
	@command -v cargo >/dev/null 2>&1 || { $(call print_error,Cargo not found. Install Rust: https://rustup.rs/); exit 1; }
	@command -v cmake >/dev/null 2>&1 || { $(call print_warning,CMake not found. C++ components will be skipped.); }
	@command -v make >/dev/null 2>&1 || { $(call print_error,Make not found.); exit 1; }
	$(call print_success,Dependencies checked)

# Clean build artifacts
clean:
	$(call print_info,Cleaning build artifacts...)
	@cargo clean
	@rm -rf build/
	@rm -rf dist/
	@rm -rf target/
	@rm -rf .cargo/
	$(call print_success,Clean complete)

# Build Rust components
build-rust: check-deps
	$(call print_info,Building Rust components...)
	@cargo build $(CARGO_FLAGS)
	$(call print_success,Rust components built)

# Build C++ components
build-cpp:
	$(call print_info,Building C++ components...)
	@mkdir -p build/cpp
	@cd build/cpp && cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) ../../
	@cd build/cpp && make -j$$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
	$(call print_success,C++ components built)

# Build everything
build: build-rust build-cpp
	$(call print_success,All components built)

# Run tests
test: build-rust
	$(call print_info,Running tests...)
	@cargo test $(CARGO_FLAGS)
	$(call print_success,All tests passed)

# Run benchmarks
bench: build-rust
	$(call print_info,Running benchmarks...)
	@cargo test --test benchmarks $(CARGO_FLAGS)
	$(call print_success,Benchmarks complete)

# Run linting
lint:
	$(call print_info,Running lints...)
	@cargo fmt --check
	@cargo clippy -- -D warnings
	$(call print_success,Linting complete)

# Format code
fmt:
	$(call print_info,Formatting code...)
	@cargo fmt
	$(call print_success,Code formatted)

# Security audit
audit:
	$(call print_info,Running security audit...)
	@cargo audit
	$(call print_success,Security audit complete)

# Create distribution package
package: build test
	$(call print_info,Creating distribution package...)
	@mkdir -p dist/bin
	@mkdir -p dist/lib
	@mkdir -p dist/include
	@mkdir -p dist/share
	@mkdir -p dist/etc
	
	# Copy Rust binaries
	@cp target/$(RUST_TARGET)/browser dist/bin/ 2>/dev/null || true
	@cp target/$(RUST_TARGET)/demo dist/bin/ 2>/dev/null || true
	
	# Copy C++ libraries if built
	@find build/cpp -name "*.a" -o -name "*.so" -o -name "*.dylib" | xargs -I {} cp {} dist/lib/ 2>/dev/null || true
	@find . -name "*.h" -path "*/include/*" | xargs -I {} cp {} dist/include/ 2>/dev/null || true
	
	# Copy documentation
	@cp README.md dist/
	@cp LICENSE dist/ 2>/dev/null || true
	@cp -r docs/ dist/share/ 2>/dev/null || true
	
	# Create launcher scripts
	@echo '#!/bin/bash' > dist/run_browser.sh
	@echo 'exec "$$(dirname "$$0")/bin/browser" "$$@"' >> dist/run_browser.sh
	@chmod +x dist/run_browser.sh
	
	@echo '#!/bin/bash' > dist/run_demo.sh
	@echo 'exec "$$(dirname "$$0")/bin/demo" "$$@"' >> dist/run_demo.sh
	@chmod +x dist/run_demo.sh
	
	# Create package info
	@echo "Chrome Browser Engine $(shell date +%Y.%m.%d)" > dist/VERSION
	@echo "Built on: $(shell date)" >> dist/VERSION
	@echo "Rust version: $(shell cargo --version)" >> dist/VERSION
	@echo "Target: $(RUST_TARGET)" >> dist/VERSION
	
	$(call print_success,Distribution package created)

# Install to system
install: package
	$(call print_info,Installing to system...)
	@sudo mkdir -p $(INSTALL_PREFIX)/bin
	@sudo mkdir -p $(INSTALL_PREFIX)/lib
	@sudo mkdir -p $(INSTALL_PREFIX)/include
	@sudo mkdir -p $(INSTALL_PREFIX)/share/chrome-browser-engine
	
	@sudo cp dist/bin/* $(INSTALL_PREFIX)/bin/
	@sudo cp dist/lib/* $(INSTALL_PREFIX)/lib/ 2>/dev/null || true
	@sudo cp dist/include/* $(INSTALL_PREFIX)/include/ 2>/dev/null || true
	@sudo cp -r dist/share/* $(INSTALL_PREFIX)/share/chrome-browser-engine/ 2>/dev/null || true
	
	# Create symlinks
	@sudo ln -sf $(INSTALL_PREFIX)/bin/browser /usr/local/bin/chrome-browser-engine 2>/dev/null || true
	
	$(call print_success,Installation complete)

# Uninstall from system
uninstall:
	$(call print_info,Uninstalling from system...)
	@sudo rm -f $(INSTALL_PREFIX)/bin/browser
	@sudo rm -f $(INSTALL_PREFIX)/bin/demo
	@sudo rm -f /usr/local/bin/chrome-browser-engine
	@sudo rm -rf $(INSTALL_PREFIX)/share/chrome-browser-engine
	$(call print_success,Uninstallation complete)

# Create release package
release: clean build test package
	$(call print_info,Creating release package...)
	@VERSION=$$(date +%Y.%m.%d); \
	RELEASE_NAME="chrome-browser-engine-$$VERSION"; \
	mkdir -p releases/$$RELEASE_NAME; \
	cp -r dist/* releases/$$RELEASE_NAME/; \
	cd releases && tar -czf $$RELEASE_NAME.tar.gz $$RELEASE_NAME; \
	cd ..; \
	echo "Release created: releases/$$RELEASE_NAME.tar.gz"
	$(call print_success,Release package created)

# Docker build
docker:
	$(call print_info,Building Docker image...)
	@docker build -t chrome-browser-engine:latest .
	$(call print_success,Docker image built)

# Docker run
docker-run:
	$(call print_info,Running Docker container...)
	@docker run -it --rm chrome-browser-engine:latest
	$(call print_success,Docker container started)

# Development setup
dev-setup:
	$(call print_info,Setting up development environment...)
	@rustup component add rustfmt clippy
	@cargo install cargo-audit cargo-watch
	$(call print_success,Development environment ready)

# Production deployment
deploy: clean build test package
	$(call print_info,Deploying to production...)
	@echo "Production deployment would go here"
	@echo "Configure your deployment target in deploy.sh"
	$(call print_success,Production deployment ready)

# Show help
help:
    @echo "Apollo Browser Engine - Production Makefile"
	@echo "==========================================="
	@echo ""
	@echo "Available targets:"
	@echo "  all          - Build and test everything"
	@echo "  build        - Build all components"
	@echo "  build-rust   - Build Rust components only"
	@echo "  build-cpp    - Build C++ components only"
	@echo "  test         - Run all tests"
	@echo "  bench        - Run benchmarks"
	@echo "  lint         - Run linting"
	@echo "  fmt          - Format code"
	@echo "  audit        - Security audit"
	@echo "  package      - Create distribution package"
	@echo "  install      - Install to system"
	@echo "  uninstall    - Uninstall from system"
	@echo "  release      - Create release package"
	@echo "  docker       - Build Docker image"
	@echo "  docker-run   - Run Docker container"
	@echo "  dev-setup    - Setup development environment"
	@echo "  deploy       - Deploy to production"
	@echo "  clean        - Clean build artifacts"
	@echo "  help         - Show this help"
	@echo ""
	@echo "Environment variables:"
	@echo "  RUST_TARGET     - Rust build target (debug/release)"
	@echo "  CARGO_FLAGS     - Cargo build flags"
	@echo "  CMAKE_BUILD_TYPE - CMake build type"
	@echo "  INSTALL_PREFIX  - Installation prefix"
