#!/bin/bash

# Apollo Browser Engine - Production Installation Script
# Installs the complete browser engine with all dependencies

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
INSTALL_DIR="${INSTALL_DIR:-/opt/chrome-browser-engine}"
USER="${INSTALL_USER:-chrome-browser}"
SERVICE_NAME="chrome-browser-engine"

# Colors
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

log_header() {
    echo -e "${PURPLE}================================${NC}"
    echo -e "${PURPLE}$1${NC}"
    echo -e "${PURPLE}================================${NC}"
}

# Check if running as root
check_root() {
    if [[ $EUID -eq 0 ]]; then
        log_warning "Running as root. This is not recommended for production."
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
}

# Detect operating system
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
        if command -v apt-get &> /dev/null; then
            DISTRO="debian"
        elif command -v yum &> /dev/null; then
            DISTRO="redhat"
        elif command -v pacman &> /dev/null; then
            DISTRO="arch"
        else
            DISTRO="unknown"
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        DISTRO="homebrew"
    elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
        OS="windows"
        DISTRO="unknown"
    else
        OS="unknown"
        DISTRO="unknown"
    fi
    
    log_info "Detected OS: $OS ($DISTRO)"
}

# Install system dependencies
install_dependencies() {
    log_header "Installing System Dependencies"
    
    case "$OS" in
        "linux")
            case "$DISTRO" in
                "debian")
                    log_info "Installing dependencies for Debian/Ubuntu..."
                    sudo apt-get update
                    sudo apt-get install -y \
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
                        curl \
                        git \
                        wget \
                        unzip
                    ;;
                "redhat")
                    log_info "Installing dependencies for RedHat/CentOS..."
                    sudo yum groupinstall -y "Development Tools"
                    sudo yum install -y \
                        cmake \
                        pkgconfig \
                        openssl-devel \
                        alsa-lib-devel \
                        libX11-devel \
                        libXext-devel \
                        libXrender-devel \
                        libXrandr-devel \
                        libXi-devel \
                        libXcursor-devel \
                        libXScrnSaver-devel \
                        mesa-libGL-devel \
                        mesa-libGLU-devel \
                        libxkbcommon-devel \
                        fontconfig-devel \
                        freetype-devel \
                        harfbuzz-devel \
                        fribidi-devel \
                        curl \
                        git \
                        wget \
                        unzip
                    ;;
                "arch")
                    log_info "Installing dependencies for Arch Linux..."
                    sudo pacman -S --noconfirm \
                        base-devel \
                        cmake \
                        pkgconfig \
                        openssl \
                        alsa-lib \
                        libx11 \
                        libxext \
                        libxrender \
                        libxrandr \
                        libxi \
                        libxcursor \
                        libxss \
                        mesa \
                        libxkbcommon \
                        fontconfig \
                        freetype2 \
                        harfbuzz \
                        fribidi \
                        curl \
                        git \
                        wget \
                        unzip
                    ;;
                *)
                    log_error "Unsupported Linux distribution: $DISTRO"
                    exit 1
                    ;;
            esac
            ;;
        "macos")
            log_info "Installing dependencies for macOS..."
            if ! command -v brew &> /dev/null; then
                log_info "Installing Homebrew..."
                /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            fi
            
            brew install \
                cmake \
                pkg-config \
                openssl \
                curl \
                git \
                wget
            ;;
        "windows")
            log_error "Windows installation not supported yet"
            exit 1
            ;;
        *)
            log_error "Unsupported operating system: $OS"
            exit 1
            ;;
    esac
    
    log_success "System dependencies installed"
}

# Install Rust
install_rust() {
    log_header "Installing Rust"
    
    if command -v rustc &> /dev/null; then
        log_info "Rust already installed: $(rustc --version)"
    else
        log_info "Installing Rust..."
        curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
        source ~/.cargo/env
    fi
    
    # Install additional Rust components
    rustup component add rustfmt clippy
    cargo install cargo-audit cargo-watch
    
    log_success "Rust installed: $(rustc --version)"
}

# Install Docker (optional)
install_docker() {
    if command -v docker &> /dev/null; then
        log_info "Docker already installed: $(docker --version)"
        return
    fi
    
    read -p "Install Docker? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        log_info "Installing Docker..."
        
        case "$OS" in
            "linux")
                curl -fsSL https://get.docker.com -o get-docker.sh
                sudo sh get-docker.sh
                sudo usermod -aG docker $USER
                ;;
            "macos")
                brew install --cask docker
                ;;
        esac
        
        log_success "Docker installed"
    fi
}

# Create system user
create_user() {
    log_header "Creating System User"
    
    if id "$USER" &>/dev/null; then
        log_info "User $USER already exists"
    else
        log_info "Creating user $USER..."
        sudo useradd -r -s /bin/false -d "$INSTALL_DIR" "$USER"
        log_success "User $USER created"
    fi
}

# Build the application
build_application() {
    log_header "Building Chrome Browser Engine"
    
    cd "$PROJECT_DIR"
    
    # Clean previous builds
    make clean
    
    # Build everything
    make build
    
    # Run tests
    make test
    
    # Create package
    make package
    
    log_success "Application built successfully"
}

# Install the application
install_application() {
    log_header "Installing Chrome Browser Engine"
    
    # Create installation directory
    sudo mkdir -p "$INSTALL_DIR"
    sudo chown root:root "$INSTALL_DIR"
    sudo chmod 755 "$INSTALL_DIR"
    
    # Copy application files
    sudo cp -r dist/* "$INSTALL_DIR/"
    sudo chown -R root:root "$INSTALL_DIR"
    sudo chmod +x "$INSTALL_DIR/bin"/*
    
    # Create symlinks
    sudo ln -sf "$INSTALL_DIR/bin/browser" /usr/local/bin/chrome-browser-engine
    sudo ln -sf "$INSTALL_DIR/bin/demo" /usr/local/bin/chrome-browser-demo
    
    # Set ownership
    sudo chown -R "$USER:$USER" "$INSTALL_DIR/data" 2>/dev/null || true
    sudo chown -R "$USER:$USER" "$INSTALL_DIR/logs" 2>/dev/null || true
    sudo chown -R "$USER:$USER" "$INSTALL_DIR/cache" 2>/dev/null || true
    
    log_success "Application installed to $INSTALL_DIR"
}

# Create systemd service
create_service() {
    log_header "Creating System Service"
    
    cat > /tmp/chrome-browser-engine.service << EOF
[Unit]
Description=Chrome Browser Engine
After=network.target

[Service]
Type=simple
User=$USER
Group=$USER
WorkingDirectory=$INSTALL_DIR
ExecStart=$INSTALL_DIR/bin/browser
ExecReload=/bin/kill -HUP \$MAINPID
Restart=always
RestartSec=5
StandardOutput=journal
StandardError=journal
SyslogIdentifier=chrome-browser-engine

# Security settings
NoNewPrivileges=true
PrivateTmp=true
ProtectSystem=strict
ProtectHome=true
ReadWritePaths=$INSTALL_DIR/data $INSTALL_DIR/logs $INSTALL_DIR/cache

# Resource limits
LimitNOFILE=65536
LimitNPROC=4096
MemoryMax=2G
CPUQuota=100%

[Install]
WantedBy=multi-user.target
EOF

    sudo mv /tmp/chrome-browser-engine.service /etc/systemd/system/
    sudo systemctl daemon-reload
    sudo systemctl enable chrome-browser-engine
    
    log_success "System service created"
}

# Configure firewall
configure_firewall() {
    log_header "Configuring Firewall"
    
    if command -v ufw &> /dev/null; then
        log_info "Configuring UFW firewall..."
        sudo ufw allow 8080/tcp
        sudo ufw allow 80/tcp
        sudo ufw allow 443/tcp
        log_success "UFW firewall configured"
    elif command -v firewall-cmd &> /dev/null; then
        log_info "Configuring firewalld..."
        sudo firewall-cmd --permanent --add-port=8080/tcp
        sudo firewall-cmd --permanent --add-port=80/tcp
        sudo firewall-cmd --permanent --add-port=443/tcp
        sudo firewall-cmd --reload
        log_success "Firewalld configured"
    else
        log_warning "No firewall detected. Please configure manually if needed."
    fi
}

# Setup monitoring
setup_monitoring() {
    log_header "Setting Up Monitoring"
    
    read -p "Install monitoring stack (Prometheus + Grafana)? (y/N): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        log_info "Setting up monitoring..."
        
        # Create monitoring directory
        sudo mkdir -p /etc/chrome-browser-engine/monitoring
        sudo cp -r monitoring/* /etc/chrome-browser-engine/monitoring/
        
        # Install Prometheus
        if ! command -v prometheus &> /dev/null; then
            log_info "Installing Prometheus..."
            wget https://github.com/prometheus/prometheus/releases/latest/download/prometheus-linux-amd64.tar.gz
            tar xzf prometheus-linux-amd64.tar.gz
            sudo cp prometheus-linux-amd64/prometheus /usr/local/bin/
            sudo cp prometheus-linux-amd64/promtool /usr/local/bin/
            rm -rf prometheus-linux-amd64*
        fi
        
        # Install Grafana
        if ! command -v grafana-server &> /dev/null; then
            log_info "Installing Grafana..."
            case "$DISTRO" in
                "debian")
                    wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
                    echo "deb https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list
                    sudo apt-get update
                    sudo apt-get install -y grafana
                    ;;
                "redhat")
                    sudo yum install -y https://dl.grafana.com/oss/release/grafana-10.0.0-1.x86_64.rpm
                    ;;
            esac
        fi
        
        # Start monitoring services
        sudo systemctl enable prometheus
        sudo systemctl enable grafana-server
        sudo systemctl start prometheus
        sudo systemctl start grafana-server
        
        log_success "Monitoring stack installed"
        log_info "Prometheus: http://localhost:9090"
        log_info "Grafana: http://localhost:3000 (admin/admin)"
    fi
}

# Start services
start_services() {
    log_header "Starting Services"
    
    # Start the browser engine
    sudo systemctl start chrome-browser-engine
    sudo systemctl status chrome-browser-engine --no-pager
    
    log_success "Chrome Browser Engine started"
}

# Verify installation
verify_installation() {
    log_header "Verifying Installation"
    
    # Check if service is running
    if systemctl is-active --quiet chrome-browser-engine; then
        log_success "Service is running"
    else
        log_error "Service is not running"
        return 1
    fi
    
    # Check if binary is accessible
    if command -v chrome-browser-engine &> /dev/null; then
        log_success "Binary is accessible"
        chrome-browser-engine --version
    else
        log_error "Binary is not accessible"
        return 1
    fi
    
    # Check health endpoint
    sleep 5  # Give service time to start
    if curl -f http://localhost:8080/health &> /dev/null; then
        log_success "Health check passed"
    else
        log_warning "Health check failed (service may still be starting)"
    fi
    
    log_success "Installation verification complete"
}

# Show post-installation info
show_post_install_info() {
    log_header "Installation Complete!"
    
    echo ""
    echo "🎉 Apollo Browser Engine has been successfully installed!"
    echo ""
    echo "📊 Service Status:"
    echo "   sudo systemctl status chrome-browser-engine"
    echo ""
    echo "🔧 Management Commands:"
    echo "   sudo systemctl start chrome-browser-engine"
    echo "   sudo systemctl stop chrome-browser-engine"
    echo "   sudo systemctl restart chrome-browser-engine"
    echo "   sudo systemctl enable chrome-browser-engine"
    echo ""
    echo "🌐 Access URLs:"
    echo "   Browser Engine: http://localhost:8080"
    echo "   Demo: http://localhost:8080/demo"
    echo "   Health Check: http://localhost:8080/health"
    echo "   Metrics: http://localhost:8080/metrics"
    echo ""
    echo "📁 Installation Directory: $INSTALL_DIR"
    echo "👤 Service User: $USER"
    echo ""
    echo "📚 Documentation:"
    echo "   README.md - Basic usage"
    echo "   docs/PRODUCTION.md - Production guide"
    echo ""
    echo "🆘 Support:"
    echo "   GitHub Issues: https://github.com/your-org/chrome-browser-engine/issues"
    echo "   Documentation: https://github.com/your-org/chrome-browser-engine/wiki"
    echo ""
    echo "🚀 Ready to use Apollo Browser Engine!"
}

# Main installation function
main() {
    log_header "Apollo Browser Engine - Production Installation"
    
    # Parse arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --install-dir)
                INSTALL_DIR="$2"
                shift 2
                ;;
            --user)
                USER="$2"
                shift 2
                ;;
            --skip-docker)
                SKIP_DOCKER=true
                shift
                ;;
            --skip-monitoring)
                SKIP_MONITORING=true
                shift
                ;;
            --help)
                echo "Chrome Browser Engine Installation Script"
                echo ""
                echo "Usage: $0 [OPTIONS]"
                echo ""
                echo "Options:"
                echo "  --install-dir DIR    Installation directory (default: /opt/chrome-browser-engine)"
                echo "  --user USER          System user (default: chrome-browser)"
                echo "  --skip-docker        Skip Docker installation"
                echo "  --skip-monitoring    Skip monitoring setup"
                echo "  --help               Show this help"
                exit 0
                ;;
            *)
                log_error "Unknown option: $1"
                exit 1
                ;;
        esac
    done
    
    # Run installation steps
    check_root
    detect_os
    install_dependencies
    install_rust
    
    if [[ "$SKIP_DOCKER" != "true" ]]; then
        install_docker
    fi
    
    create_user
    build_application
    install_application
    create_service
    configure_firewall
    
    if [[ "$SKIP_MONITORING" != "true" ]]; then
        setup_monitoring
    fi
    
    start_services
    verify_installation
    show_post_install_info
}

# Run main function
main "$@"
