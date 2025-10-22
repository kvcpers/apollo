# Apollo Browser Engine - Production Dockerfile
# Multi-stage build for production deployment

# Build stage
FROM rust:1.75-slim AS rust-builder

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libssl-dev \
    libasound2-dev \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxi-dev \
    libxcursor-dev \
    libxss-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxkbcommon-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libharfbuzz-dev \
    libfribidi-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy Cargo files
COPY Cargo.toml ./
COPY Cargo.lock* ./
COPY core/browser/Cargo.toml core/browser/
COPY parser/html/Cargo.toml parser/html/
COPY parser/css/Cargo.toml parser/css/
COPY parser/url/Cargo.toml parser/url/
COPY style/Cargo.toml style/
COPY network/http/Cargo.toml network/http/

# Build dependencies
RUN cargo build --release --workspace

# Copy source code
COPY . .

# Build the application
RUN cargo build --release --workspace

# C++ Build stage
FROM ubuntu:22.04 AS cpp-builder

# Install C++ build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libssl-dev \
    libasound2-dev \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxi-dev \
    libxcursor-dev \
    libxss-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxkbcommon-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libharfbuzz-dev \
    libfribidi-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy C++ source code
COPY layout/ renderer/ js/ CMakeLists.txt ./

# Build C++ components
RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

# Production stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libssl3 \
    libasound2 \
    libx11-6 \
    libxext6 \
    libxrender1 \
    libxrandr2 \
    libxinerama1 \
    libxi6 \
    libxcursor1 \
    libxss1 \
    libgl1-mesa-glx \
    libglu1-mesa \
    libxkbcommon0 \
    libfontconfig1 \
    libfreetype6 \
    libharfbuzz0b \
    libfribidi0 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create app user
RUN useradd -m -s /bin/bash app

# Set working directory
WORKDIR /app

# Copy Rust binaries from rust-builder
COPY --from=rust-builder /app/target/release/browser /app/bin/
COPY --from=rust-builder /app/target/release/demo /app/bin/

# Copy C++ libraries from cpp-builder
COPY --from=cpp-builder /app/build/layout/liblayout.a /app/lib/
COPY --from=cpp-builder /app/build/renderer/librenderer.a /app/lib/
COPY --from=cpp-builder /app/build/js/libjs.a /app/lib/

# Copy headers
COPY --from=cpp-builder /app/layout/include/ /app/include/layout/
COPY --from=cpp-builder /app/renderer/include/ /app/include/renderer/
COPY --from=cpp-builder /app/js/include/ /app/include/js/

# Copy documentation and assets
COPY README.md /app/
COPY demo.html /app/share/
COPY test_data/ /app/share/test_data/

# Create launcher scripts
RUN echo '#!/bin/bash' > /app/run_browser.sh && \
    echo 'exec /app/bin/browser "$@"' >> /app/run_browser.sh && \
    chmod +x /app/run_browser.sh

RUN echo '#!/bin/bash' > /app/run_demo.sh && \
    echo 'exec /app/bin/demo "$@"' >> /app/run_demo.sh && \
    chmod +x /app/run_demo.sh

# Set permissions
RUN chown -R app:app /app
USER app

# Expose ports (if needed for web interface)
EXPOSE 8080

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=5s --retries=3 \
    CMD /app/bin/browser --version || exit 1

# Default command
CMD ["/app/run_browser.sh"]

# Labels
LABEL maintainer="Apollo Browser Engine Team"
LABEL version="1.0.0"
LABEL description="Complete browser engine built with Rust and C++"
LABEL org.opencontainers.image.source="https://github.com/your-org/apollo-browser-engine"
LABEL org.opencontainers.image.licenses="MIT"
