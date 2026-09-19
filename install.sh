#!/bin/bash
# Super-Lang 交互式安装脚本
# 支持 Linux / macOS
# 用法: curl -fsSL <url>/install.sh | bash -s [version]

set -e

# 版本信息
SCRIPT_VERSION="1.0.0"
DEFAULT_VERSION="latest"
REPO_OWNER="super-lang"
REPO_NAME="Super-Lang"
GITHUB_API="https://api.github.com"
GITHUB_RELEASES="https://github.com/${REPO_OWNER}/${REPO_NAME}/releases/download"

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认配置
VERSION="${1:-${DEFAULT_VERSION}}"
INSTALL_DIR="/usr/local/bin"
USE_PROXY=""
PROXY_URL=""
INSTALL_MODE="binary"  # binary, source
LANGUAGE="zh"  # zh, en
SKIP_CONFIRM=false

# 语言文本
declare -A TEXTS_ZH=(
    ["welcome"]="欢迎使用 Super-Lang 安装向导"
    ["select_lang"]="请选择语言 / Please select language:"
    ["lang_zh"]="1) 中文"
    ["lang_en"]="2) English"
    ["enter_choice"]="请输入选择 [1-2]: "
    ["invalid_choice"]="无效选择，请重试"
    ["select_install_mode"]="选择安装模式:"
    ["mode_binary"]="1) 二进制安装 (推荐，快速)"
    ["mode_source"]="2) 源码构建 (需要编译器工具链)"
    ["mode_auto"]="3) 自动检测"
    ["enter_install_mode"]="请输入选择 [1-3]: "
    ["proxy_config"]="配置代理 (用于下载加速):"
    ["proxy_none"]="1) 不使用代理"
    ["proxy_http"]="2) HTTP/HTTPS 代理"
    ["proxy_socks5"]="3) SOCKS5 代理"
    ["enter_proxy"]="请输入选择 [1-3]: "
    ["proxy_url_prompt"]="请输入代理地址 (例: http://127.0.0.1:7890): "
    ["install_dir_prompt"]="安装目录 [默认: /usr/local/bin]: "
    ["confirm_install"]="即将安装 Super-Lang ${VERSION} 到 ${INSTALL_DIR}\n继续安装? [Y/n]: "
    ["downloading"]="正在下载..."
    ["download_failed"]="下载失败"
    ["installing"]="正在安装..."
    ["install_success"]="安装成功！"
    ["verify_success"]="验证通过: super --version"
    ["install_failed"]="安装失败"
    ["run_super_help"]="运行 'super help' 查看使用帮助"
    ["need_sudo"]="需要管理员权限安装到系统目录"
    ["creating_temp"]="创建临时目录..."
    ["extracting"]="解压文件..."
    ["setting_permissions"]="设置执行权限..."
    ["moving_binary"]="移动二进制文件到安装目录..."
    ["updating_path"]="更新 PATH 环境变量..."
    ["done"]="完成！"
)

declare -A TEXTS_EN=(
    ["welcome"]="Welcome to Super-Lang Installation Wizard"
    ["select_lang"]="Please select language:"
    ["lang_zh"]="1) Chinese"
    ["lang_en"]="2) English"
    ["enter_choice"]="Enter choice [1-2]: "
    ["invalid_choice"]="Invalid choice, please try again"
    ["select_install_mode"]="Select installation mode:"
    ["mode_binary"]="1) Binary install (recommended, fast)"
    ["mode_source"]="2) Build from source (requires compiler toolchain)"
    ["mode_auto"]="3) Auto detect"
    ["enter_install_mode"]="Enter choice [1-3]: "
    ["proxy_config"]="Configure proxy (for download acceleration):"
    ["proxy_none"]="1) No proxy"
    ["proxy_http"]="2) HTTP/HTTPS proxy"
    ["proxy_socks5"]="3) SOCKS5 proxy"
    ["enter_proxy"]="Enter choice [1-3]: "
    ["proxy_url_prompt"]="Enter proxy URL (e.g. http://127.0.0.1:7890): "
    ["install_dir_prompt"]="Install directory [default: /usr/local/bin]: "
    ["confirm_install"]="About to install Super-Lang ${VERSION} to ${INSTALL_DIR}\nContinue? [Y/n]: "
    ["downloading"]="Downloading..."
    ["download_failed"]="Download failed"
    ["installing"]="Installing..."
    ["install_success"]="Installation successful!"
    ["verify_success"]="Verification passed: super --version"
    ["install_failed"]="Installation failed"
    ["run_super_help"]="Run 'super help' for usage"
    ["need_sudo"]="Administrator privileges required for system directory"
    ["creating_temp"]="Creating temporary directory..."
    ["extracting"]="Extracting files..."
    ["setting_permissions"]="Setting execute permissions..."
    ["moving_binary"]="Moving binary to install directory..."
    ["updating_path"]="Updating PATH environment variable..."
    ["done"]="Done!"
)

# 获取文本
t() {
    local key="$1"
    if [[ "$LANGUAGE" == "en" ]]; then
        echo "${TEXTS_EN[$key]:-$key}"
    else
        echo "${TEXTS_ZH[$key]:-$key}"
    fi
}

# 打印带颜色的消息
print_info() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARN]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

# 选择语言
select_language() {
    echo "$(t welcome)"
    echo "$(t select_lang)"
    echo "$(t lang_zh)"
    echo "$(t lang_en)"
    while true; do
        read -p "$(t enter_choice)" choice
        case $choice in
            1) LANGUAGE="zh"; break ;;
            2) LANGUAGE="en"; break ;;
            *) print_error "$(t invalid_choice)" ;;
        esac
    done
}

# 选择安装模式
select_install_mode() {
    echo "$(t select_install_mode)"
    echo "$(t mode_binary)"
    echo "$(t mode_source)"
    echo "$(t mode_auto)"
    while true; do
        read -p "$(t enter_install_mode)" choice
        case $choice in
            1) INSTALL_MODE="binary"; break ;;
            2) INSTALL_MODE="source"; break ;;
            3) INSTALL_MODE="auto"; break ;;
            *) print_error "$(t invalid_choice)" ;;
        esac
    done
}

# 配置代理
configure_proxy() {
    echo "$(t proxy_config)"
    echo "$(t proxy_none)"
    echo "$(t proxy_http)"
    echo "$(t proxy_socks5)"
    while true; do
        read -p "$(t enter_proxy)" choice
        case $choice in
            1) USE_PROXY=""; PROXY_URL=""; break ;;
            2) USE_PROXY="http"; read -p "$(t proxy_url_prompt)" PROXY_URL; break ;;
            3) USE_PROXY="socks5"; read -p "$(t proxy_url_prompt)" PROXY_URL; break ;;
            *) print_error "$(t invalid_choice)" ;;
        esac
    done
}

# 获取安装目录
get_install_dir() {
    read -p "$(t install_dir_prompt)" dir
    INSTALL_DIR="${dir:-/usr/local/bin}"
}

# 确认安装
confirm_install() {
    if [[ "$SKIP_CONFIRM" == "true" ]]; then
        return 0
    fi
    local msg=$(printf "$(t confirm_install)" "$VERSION" "$INSTALL_DIR")
    read -p "$(echo -e "$msg")" confirm
    [[ "$confirm" =~ ^[Yy]?$ ]] || { print_warning "安装已取消"; exit 0; }
}

# 检测平台
detect_platform() {
    OS=$(uname -s | tr '[:upper:]' '[:lower:]')
    ARCH=$(uname -m)
    
    case "$ARCH" in
        x86_64) ARCH="amd64" ;;
        aarch64|arm64) ARCH="arm64" ;;
        *) print_error "Unsupported architecture: $ARCH"; exit 1 ;;
    esac
    
    case "$OS" in
        linux) PLATFORM="linux-$ARCH" ;;
        darwin) PLATFORM="macos-$ARCH" ;;
        *) print_error "Unsupported OS: $OS"; exit 1 ;;
    esac
    
    BINARY_NAME="superc-${PLATFORM}"
    [[ "$OS" == "windows" ]] && BINARY_NAME="${BINARY_NAME}.exe"
}

# 获取最新版本号
get_latest_version() {
    if [[ "$VERSION" == "latest" ]]; then
        print_info "获取最新版本信息..."
        local api_url="${GITHUB_API}/repos/${REPO_OWNER}/${REPO_NAME}/releases/latest"
        local curl_cmd="curl -fsSL"
        [[ -n "$USE_PROXY" ]] && curl_cmd="$curl_cmd -x $PROXY_URL"
        VERSION=$($curl_cmd "$api_url" | grep '"tag_name"' | head -1 | sed 's/.*"tag_name": "\(.*\)".*/\1/' | sed 's/^v//')
        [[ -z "$VERSION" ]] && VERSION="0.0.1"
        print_info "最新版本: v${VERSION}"
    fi
}

# 下载文件
download_file() {
    local url="$1"
    local output="$2"
    local curl_cmd="curl -fsSL -o $output"
    [[ -n "$USE_PROXY" ]] && curl_cmd="$curl_cmd -x $PROXY_URL"
    
    print_info "$(t downloading): $url"
    if ! $curl_cmd "$url"; then
        print_error "$(t download_failed): $url"
        return 1
    fi
    return 0
}

# 安装二进制
install_binary() {
    get_latest_version
    detect_platform
    
    local download_url="${GITHUB_RELEASES}/v${VERSION}/${BINARY_NAME}"
    local temp_dir=$(mktemp -d)
    local temp_file="${temp_dir}/${BINARY_NAME}"
    
    print_info "$(t creating_temp): $temp_dir"
    
    if ! download_file "$download_url" "$temp_file"; then
        # 尝试备用下载：tar.gz
        local archive_url="${GITHUB_RELEASES}/v${VERSION}/superc-${PLATFORM}.tar.gz"
        local archive_file="${temp_dir}/superc-${PLATFORM}.tar.gz"
        print_info "尝试下载压缩包..."
        if download_file "$archive_url" "$archive_file"; then
            print_info "$(t extracting)"
            tar -xzf "$archive_file" -C "$temp_dir"
            temp_file="${temp_dir}/superc-${PLATFORM}"
        else
            rm -rf "$temp_dir"
            return 1
        fi
    fi
    
    print_info "$(t setting_permissions)"
    chmod +x "$temp_file"
    
    print_info "$(t moving_binary)"
    if [[ -w "$INSTALL_DIR" ]]; then
        mv "$temp_file" "${INSTALL_DIR}/superc"
    else
        print_warning "$(t need_sudo)"
        sudo mv "$temp_file" "${INSTALL_DIR}/superc"
    fi
    
    # 创建 super 符号链接
    if [[ -w "$INSTALL_DIR" ]]; then
        ln -sf "${INSTALL_DIR}/superc" "${INSTALL_DIR}/super"
    else
        sudo ln -sf "${INSTALL_DIR}/superc" "${INSTALL_DIR}/super"
    fi
    
    rm -rf "$temp_dir"
    return 0
}

# 源码构建安装
install_from_source() {
    print_info "从源码构建安装..."
    print_warning "需要安装: git, make, gcc/clang"
    
    local temp_dir=$(mktemp -d)
    cd "$temp_dir"
    
    print_info "克隆源码..."
    git clone --depth 1 --branch "v${VERSION}" "https://github.com/${REPO_OWNER}/${REPO_NAME}.git" .
    
    print_info "编译中..."
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    print_info "$(t moving_binary)"
    if [[ -w "$INSTALL_DIR" ]]; then
        cp build/bin/superc "${INSTALL_DIR}/superc"
        ln -sf "${INSTALL_DIR}/superc" "${INSTALL_DIR}/super"
    else
        print_warning "$(t need_sudo)"
        sudo cp build/bin/superc "${INSTALL_DIR}/superc"
        sudo ln -sf "${INSTALL_DIR}/superc" "${INSTALL_DIR}/super"
    fi
    
    cd - > /dev/null
    rm -rf "$temp_dir"
    return 0
}

# 验证安装
verify_installation() {
    if command -v super &> /dev/null; then
        print_success "$(t install_success)"
        super --version
        echo
        print_info "$(t run_super_help)"
        return 0
    else
        print_error "$(t install_failed): super 命令不可用"
        print_warning "请确保 ${INSTALL_DIR} 在 PATH 中"
        return 1
    fi
}

# 主函数
main() {
    # 非交互模式检测
    if [[ ! -t 0 ]] || [[ "$1" == "--non-interactive" ]]; then
        SKIP_CONFIRM=true
        LANGUAGE="en"
    fi
    
    # 交互式配置
    if [[ "$SKIP_CONFIRM" != "true" ]]; then
        select_language
        select_install_mode
        configure_proxy
        get_install_dir
        confirm_install
    fi
    
    print_info "$(t installing)"
    
    case "$INSTALL_MODE" in
        binary)
            install_binary || { print_error "$(t install_failed)"; exit 1; }
            ;;
        source)
            install_from_source || { print_error "$(t install_failed)"; exit 1; }
            ;;
        auto)
            if install_binary; then
                : # 成功
            else
                print_warning "二进制安装失败，尝试源码构建..."
                install_from_source || { print_error "$(t install_failed)"; exit 1; }
            fi
            ;;
    esac
    
    verify_installation
    print_success "$(t done)"
}

# 捕获中断信号
trap 'print_error "安装被中断"; exit 1' INT TERM

# 运行主函数
main "$@"