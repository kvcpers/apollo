#!/bin/bash

# Apollo Browser Engine - Production Deployment Script
# Handles deployment to various environments

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
DEPLOY_ENV="${1:-development}"
VERSION="${2:-$(date +%Y.%m.%d)}"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
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
if [ ! -f "$PROJECT_DIR/Cargo.toml" ]; then
    log_error "Not in the browser engine root directory"
    exit 1
fi

# Load environment configuration
load_env_config() {
    local env_file="$PROJECT_DIR/.env.$DEPLOY_ENV"
    if [ -f "$env_file" ]; then
        log_info "Loading environment configuration from $env_file"
        source "$env_file"
    else
        log_warning "No environment configuration found for $DEPLOY_ENV"
    fi
}

# Build the application
build_application() {
    log_info "Building Chrome Browser Engine for $DEPLOY_ENV..."
    
    cd "$PROJECT_DIR"
    
    # Clean previous builds
    make clean
    
    # Build based on environment
    case "$DEPLOY_ENV" in
        "development")
            make build
            ;;
        "staging")
            make build
            make test
            ;;
        "production")
            make build
            make test
            make lint
            make audit
            ;;
        *)
            log_error "Unknown deployment environment: $DEPLOY_ENV"
            exit 1
            ;;
    esac
    
    log_success "Build complete"
}

# Package the application
package_application() {
    log_info "Packaging application..."
    
    cd "$PROJECT_DIR"
    make package
    
    # Create deployment package
    local package_name="chrome-browser-engine-$VERSION-$DEPLOY_ENV"
    local package_dir="deployments/$package_name"
    
    mkdir -p "$package_dir"
    cp -r dist/* "$package_dir/"
    
    # Add deployment-specific files
    cat > "$package_dir/deploy.json" << EOF
{
    "version": "$VERSION",
    "environment": "$DEPLOY_ENV",
    "build_date": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
    "git_commit": "$(git rev-parse HEAD 2>/dev/null || echo 'unknown')",
    "git_branch": "$(git branch --show-current 2>/dev/null || echo 'unknown')"
}
EOF
    
    # Create deployment archive
    cd deployments
    tar -czf "$package_name.tar.gz" "$package_name"
    zip -r "$package_name.zip" "$package_name"
    
    log_success "Package created: deployments/$package_name.tar.gz"
}

# Deploy to local environment
deploy_local() {
    log_info "Deploying to local environment..."
    
    local package_dir="deployments/chrome-browser-engine-$VERSION-$DEPLOY_ENV"
    
    if [ ! -d "$package_dir" ]; then
        log_error "Package not found: $package_dir"
        exit 1
    fi
    
    # Install to local system
    sudo mkdir -p /opt/chrome-browser-engine
    sudo cp -r "$package_dir"/* /opt/chrome-browser-engine/
    sudo chown -R root:root /opt/chrome-browser-engine
    sudo chmod +x /opt/chrome-browser-engine/bin/*
    
    # Create symlinks
    sudo ln -sf /opt/chrome-browser-engine/bin/browser /usr/local/bin/chrome-browser-engine
    sudo ln -sf /opt/chrome-browser-engine/bin/demo /usr/local/bin/chrome-browser-demo
    
    log_success "Local deployment complete"
}

# Deploy to Docker
deploy_docker() {
    log_info "Deploying to Docker..."
    
    cd "$PROJECT_DIR"
    
    # Build Docker image
    docker build -t chrome-browser-engine:$VERSION .
    docker tag chrome-browser-engine:$VERSION chrome-browser-engine:latest
    
    # Run container
    docker run -d \
        --name chrome-browser-engine \
        --restart unless-stopped \
        -p 8080:8080 \
        chrome-browser-engine:$VERSION
    
    log_success "Docker deployment complete"
}

# Deploy to cloud (AWS/GCP/Azure)
deploy_cloud() {
    log_info "Deploying to cloud environment..."
    
    # This would integrate with your cloud provider
    # Examples for different cloud providers:
    
    case "${CLOUD_PROVIDER:-aws}" in
        "aws")
            deploy_aws
            ;;
        "gcp")
            deploy_gcp
            ;;
        "azure")
            deploy_azure
            ;;
        *)
            log_error "Unknown cloud provider: ${CLOUD_PROVIDER}"
            exit 1
            ;;
    esac
}

# Deploy to AWS
deploy_aws() {
    log_info "Deploying to AWS..."
    
    # Check for AWS CLI
    if ! command -v aws &> /dev/null; then
        log_error "AWS CLI not found. Please install it first."
        exit 1
    fi
    
    # Upload to S3
    local package_name="chrome-browser-engine-$VERSION-$DEPLOY_ENV"
    aws s3 cp "deployments/$package_name.tar.gz" "s3://${AWS_S3_BUCKET:-chrome-browser-engine}/releases/"
    
    # Deploy to EC2/ECS/Lambda
    # This would depend on your AWS setup
    log_success "AWS deployment complete"
}

# Deploy to GCP
deploy_gcp() {
    log_info "Deploying to GCP..."
    
    # Check for gcloud CLI
    if ! command -v gcloud &> /dev/null; then
        log_error "gcloud CLI not found. Please install it first."
        exit 1
    fi
    
    # Upload to Cloud Storage
    local package_name="chrome-browser-engine-$VERSION-$DEPLOY_ENV"
    gsutil cp "deployments/$package_name.tar.gz" "gs://${GCP_BUCKET:-chrome-browser-engine}/releases/"
    
    # Deploy to Cloud Run/GKE
    # This would depend on your GCP setup
    log_success "GCP deployment complete"
}

# Deploy to Azure
deploy_azure() {
    log_info "Deploying to Azure..."
    
    # Check for Azure CLI
    if ! command -v az &> /dev/null; then
        log_error "Azure CLI not found. Please install it first."
        exit 1
    fi
    
    # Upload to Blob Storage
    local package_name="chrome-browser-engine-$VERSION-$DEPLOY_ENV"
    az storage blob upload \
        --file "deployments/$package_name.tar.gz" \
        --container-name releases \
        --name "$package_name.tar.gz"
    
    # Deploy to Container Instances/App Service
    # This would depend on your Azure setup
    log_success "Azure deployment complete"
}

# Health check
health_check() {
    log_info "Running health check..."
    
    # Check if browser is running
    if command -v chrome-browser-engine &> /dev/null; then
        if chrome-browser-engine --version &> /dev/null; then
            log_success "Health check passed"
        else
            log_error "Health check failed - browser not responding"
            exit 1
        fi
    else
        log_warning "Browser not found in PATH"
    fi
}

# Rollback deployment
rollback() {
    log_info "Rolling back deployment..."
    
    # This would depend on your deployment strategy
    # For now, just show what would happen
    log_warning "Rollback not implemented yet"
    log_info "Would rollback to previous version"
}

# Show help
show_help() {
    echo "Chrome Browser Engine - Deployment Script"
    echo "=========================================="
    echo ""
    echo "Usage: $0 [ENVIRONMENT] [VERSION]"
    echo ""
    echo "Environments:"
    echo "  development  - Deploy to development environment"
    echo "  staging      - Deploy to staging environment"
    echo "  production   - Deploy to production environment"
    echo ""
    echo "Options:"
    echo "  --docker     - Deploy using Docker"
    echo "  --cloud      - Deploy to cloud provider"
    echo "  --local      - Deploy to local system"
    echo "  --rollback   - Rollback deployment"
    echo "  --help       - Show this help"
    echo ""
    echo "Environment variables:"
    echo "  CLOUD_PROVIDER  - Cloud provider (aws/gcp/azure)"
    echo "  AWS_S3_BUCKET   - AWS S3 bucket name"
    echo "  GCP_BUCKET      - GCP bucket name"
    echo ""
    echo "Examples:"
    echo "  $0 development"
    echo "  $0 production 1.0.0"
    echo "  $0 staging --docker"
    echo "  $0 production --cloud"
}

# Main deployment logic
main() {
    # Parse arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --docker)
                DEPLOY_METHOD="docker"
                shift
                ;;
            --cloud)
                DEPLOY_METHOD="cloud"
                shift
                ;;
            --local)
                DEPLOY_METHOD="local"
                shift
                ;;
            --rollback)
                rollback
                exit 0
                ;;
            --help)
                show_help
                exit 0
                ;;
            *)
                if [ -z "$DEPLOY_ENV" ]; then
                    DEPLOY_ENV="$1"
                elif [ -z "$VERSION" ]; then
                    VERSION="$1"
                fi
                shift
                ;;
        esac
    done
    
    # Set default deployment method
    DEPLOY_METHOD="${DEPLOY_METHOD:-local}"
    
    log_info "Starting Apollo Browser Engine deployment to $DEPLOY_ENV environment"
    log_info "Version: $VERSION"
    log_info "Method: $DEPLOY_METHOD"
    
    # Load environment configuration
    load_env_config
    
    # Build and package
    build_application
    package_application
    
    # Deploy based on method
    case "$DEPLOY_METHOD" in
        "local")
            deploy_local
            ;;
        "docker")
            deploy_docker
            ;;
        "cloud")
            deploy_cloud
            ;;
        *)
            log_error "Unknown deployment method: $DEPLOY_METHOD"
            exit 1
            ;;
    esac
    
    # Health check
    health_check
    
    log_success "Deployment to $DEPLOY_ENV complete!"
}

# Run main function
main "$@"
