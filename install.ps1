<#
.SYNOPSIS
    Super-Lang 交互式安装脚本 (Windows PowerShell)
.DESCRIPTION
    支持交互式安装 Super-Lang 工具链到 Windows 系统
    支持中英文、代理配置、安装模式选择
.EXAMPLE
    # 交互式安装
    iwr https://github.com/super-lang/Super-Lang/releases/download/v0.0.1/install.ps1 | iex
    
    # 指定版本
    iwr https://github.com/super-lang/Super-Lang/releases/download/v0.0.1/install.ps1 | iex -Version 0.0.1
    
    # 非交互式安装
    iwr https://github.com/super-lang/Super-Lang/releases/download/v0.0.1/install.ps1 | iex -NonInteractive
#>

param(
    [string]$Version = "latest",
    [string]$InstallDir = "C:\Program Files\Super",
    [switch]$NonInteractive,
    [string]$Language = "",
    [string]$InstallMode = "",
    [string]$ProxyType = "",
    [string]$ProxyUrl = ""
)

# 版本信息
$ScriptVersion = "1.0.0"
$RepoOwner = "super-lang"
$RepoName = "Super-Lang"
$GitHubApi = "https://api.github.com"
$GitHubReleases = "https://github.com/$RepoOwner/$RepoName/releases/download"

# 颜色定义
$ColorRed = [ConsoleColor]::Red
$ColorGreen = [ConsoleColor]::Green
$ColorYellow = [ConsoleColor]::Yellow
$ColorBlue = [ConsoleColor]::Cyan
$ColorDefault = [ConsoleColor]::Gray

# 语言文本
$TextsZh = @{
    welcome = "欢迎使用 Super-Lang 安装向导"
    selectLang = "请选择语言 / Please select language:"
    langZh = "1) 中文"
    langEn = "2) English"
    enterChoice = "请输入选择 [1-2]: "
    invalidChoice = "无效选择，请重试"
    selectInstallMode = "选择安装模式:"
    modeBinary = "1) 二进制安装 (推荐，快速)"
    modeSource = "2) 源码构建 (需要编译器工具链)"
    modeAuto = "3) 自动检测"
    enterInstallMode = "请输入选择 [1-3]: "
    proxyConfig = "配置代理 (用于下载加速):"
    proxyNone = "1) 不使用代理"
    proxyHttp = "2) HTTP/HTTPS 代理"
    proxySocks5 = "3) SOCKS5 代理"
    enterProxy = "请输入选择 [1-3]: "
    proxyUrlPrompt = "请输入代理地址 (例: http://127.0.0.1:7890): "
    installDirPrompt = "安装目录 [默认: C:\Program Files\Super]: "
    confirmInstall = "即将安装 Super-Lang {0} 到 {1}`n继续安装? [Y/n]: "
    downloading = "正在下载..."
    downloadFailed = "下载失败"
    installing = "正在安装..."
    installSuccess = "安装成功！"
    verifySuccess = "验证通过: super --version"
    installFailed = "安装失败"
    runSuperHelp = "运行 'super help' 查看使用帮助"
    needAdmin = "需要管理员权限安装到系统目录"
    creatingTemp = "创建临时目录..."
    extracting = "解压文件..."
    settingPermissions = "设置执行权限..."
    movingBinary = "移动二进制文件到安装目录..."
    updatingPath = "更新 PATH 环境变量..."
    done = "完成！"
    architecture = "架构"
    osDetected = "检测到系统"
    latestVersion = "最新版本"
    binaryInstall = "二进制安装"
    sourceInstall = "源码构建"
    autoDetect = "自动检测"
}

$TextsEn = @{
    welcome = "Welcome to Super-Lang Installation Wizard"
    selectLang = "Please select language:"
    langZh = "1) Chinese"
    langEn = "2) English"
    enterChoice = "Enter choice [1-2]: "
    invalidChoice = "Invalid choice, please try again"
    selectInstallMode = "Select installation mode:"
    modeBinary = "1) Binary install (recommended, fast)"
    modeSource = "2) Build from source (requires compiler toolchain)"
    modeAuto = "3) Auto detect"
    enterInstallMode = "Enter choice [1-3]: "
    proxyConfig = "Configure proxy (for download acceleration):"
    proxyNone = "1) No proxy"
    proxyHttp = "2) HTTP/HTTPS proxy"
    proxySocks5 = "3) SOCKS5 proxy"
    enterProxy = "Enter choice [1-3]: "
    proxyUrlPrompt = "Enter proxy URL (e.g. http://127.0.0.1:7890): "
    installDirPrompt = "Install directory [default: C:\Program Files\Super]: "
    confirmInstall = "About to install Super-Lang {0} to {1}`nContinue? [Y/n]: "
    downloading = "Downloading..."
    downloadFailed = "Download failed"
    installing = "Installing..."
    installSuccess = "Installation successful!"
    verifySuccess = "Verification passed: super --version"
    installFailed = "Installation failed"
    runSuperHelp = "Run 'super help' for usage"
    needAdmin = "Administrator privileges required for system directory"
    creatingTemp = "Creating temporary directory..."
    extracting = "Extracting files..."
    settingPermissions = "Setting execute permissions..."
    movingBinary = "Moving binary to install directory..."
    updatingPath = "Updating PATH environment variable..."
    done = "Done!"
    architecture = "Architecture"
    osDetected = "Detected OS"
    latestVersion = "Latest version"
    binaryInstall = "Binary install"
    sourceInstall = "Build from source"
    autoDetect = "Auto detect"
}

# 当前语言文本
$Texts = $TextsZh

function Write-Color($message, $color = $ColorDefault) {
    Write-Host $message -ForegroundColor $color
}

function Write-Info($message) { Write-Color "[INFO] $message" $ColorBlue }
function Write-Success($message) { Write-Color "[SUCCESS] $message" $ColorGreen }
function Write-Warning($message) { Write-Color "[WARN] $message" $ColorYellow }
function Write-Error($message) { Write-Color "[ERROR] $message" $ColorRed }

function Get-Text($key) {
    return $Texts[$key]
}

# 选择语言
function Select-Language {
    Write-Host (Get-Text welcome)
    Write-Host (Get-Text selectLang)
    Write-Host (Get-Text langZh)
    Write-Host (Get-Text langEn)
    
    while ($true) {
        $choice = Read-Host -Prompt (Get-Text enterChoice)
        switch ($choice) {
            1 { $global:Language = "zh"; $Texts = $TextsZh; break }
            2 { $global:Language = "en"; $Texts = $TextsEn; break }
            default { Write-Error (Get-Text invalidChoice) }
        }
    }
}

# 选择安装模式
function Select-InstallMode {
    Write-Host (Get-Text selectInstallMode)
    Write-Host (Get-Text modeBinary)
    Write-Host (Get-Text modeSource)
    Write-Host (Get-Text modeAuto)
    
    while ($true) {
        $choice = Read-Host -Prompt (Get-Text enterInstallMode)
        switch ($choice) {
            1 { $global:InstallMode = "binary"; break }
            2 { $global:InstallMode = "source"; break }
            3 { $global:InstallMode = "auto"; break }
            default { Write-Error (Get-Text invalidChoice) }
        }
    }
}

# 配置代理
function Configure-Proxy {
    Write-Host (Get-Text proxyConfig)
    Write-Host (Get-Text proxyNone)
    Write-Host (Get-Text proxyHttp)
    Write-Host (Get-Text proxySocks5)
    
    while ($true) {
        $choice = Read-Host -Prompt (Get-Text enterProxy)
        switch ($choice) {
            1 { $global:UseProxy = ""; $global:ProxyUrl = ""; break }
            2 { $global:UseProxy = "http"; $global:ProxyUrl = Read-Host -Prompt (Get-Text proxyUrlPrompt); break }
            3 { $global:UseProxy = "socks5"; $global:ProxyUrl = Read-Host -Prompt (Get-Text proxyUrlPrompt); break }
            default { Write-Error (Get-Text invalidChoice) }
        }
    }
}

# 获取安装目录
function Get-InstallDir {
    $dir = Read-Host -Prompt (Get-Text installDirPrompt)
    if (-not $dir) { $dir = "C:\Program Files\Super" }
    $global:InstallDir = $dir
}

# 确认安装
function Confirm-Install {
    if ($global:SkipConfirm) { return $true }
    
    $msg = (Get-Text confirmInstall) -f $global:Version, $global:InstallDir
    $confirm = Read-Host -Prompt $msg
    return $confirm -match "^[Yy]?$"
}

# 检测平台
function Detect-Platform {
    $arch = (Get-CimInstance Win32_Processor).AddressWidth
    if ($arch -eq 64) { $global:Arch = "amd64" } else { $global:Arch = "386" }
    $global:Platform = "windows-$global:Arch"
    $global:BinaryName = "superc-$global:Platform.exe"
    
    Write-Info "$(Get-Text osDetected): Windows $global:Arch"
}

# 获取最新版本
function Get-LatestVersion {
    if ($global:Version -eq "latest") {
        Write-Info "获取最新版本信息..."
        try {
            $headers = @{}
            if ($global:UseProxy) { $headers["Proxy"] = $global:ProxyUrl }
            $response = Invoke-RestMethod -Uri "$GitHubApi/repos/$RepoOwner/$RepoName/releases/latest" -Headers $headers -ErrorAction Stop
            $global:Version = $response.tag_name -replace '^v', ''
            Write-Info "$(Get-Text latestVersion): v$global:Version"
        } catch {
            Write-Warning "无法获取最新版本，使用默认版本 0.0.1"
            $global:Version = "0.0.1"
        }
    }
}

# 下载文件
function Download-File {
    param($Url, $Output)
    
    Write-Info "$(Get-Text downloading): $Url"
    try {
        $params = @{
            Uri = $Url
            OutFile = $Output
            ErrorAction = "Stop"
        }
        if ($global:UseProxy) { $params.Proxy = $global:ProxyUrl }
        Invoke-WebRequest @params
        return $true
    } catch {
        Write-Error "$(Get-Text downloadFailed): $Url"
        return $false
    }
}

# 二进制安装
function Install-Binary {
    Get-LatestVersion
    Detect-Platform
    
    $downloadUrl = "$GitHubReleases/v$global:Version/$global:BinaryName"
    $tempDir = [IO.Path]::GetTempPath() + "super-install-" + [Guid]::NewGuid()
    $tempFile = Join-Path $tempDir $global:BinaryName
    
    Write-Info "$(Get-Text creatingTemp): $tempDir"
    New-Item -ItemType Directory -Path $tempDir -Force | Out-Null
    
    $success = Download-File $downloadUrl $tempFile
    
    if (-not $success) {
        # 尝试备用下载：zip
        $archiveUrl = "$GitHubReleases/v$global:Version/superc-$global:Platform.zip"
        $archiveFile = Join-Path $tempDir "superc-$global:Platform.zip"
        Write-Info "尝试下载压缩包..."
        if (Download-File $archiveUrl $archiveFile) {
            Write-Info $(Get-Text extracting)
            Expand-Archive -Path $archiveFile -DestinationPath $tempDir -Force
            $tempFile = Join-Path $tempDir "superc-$global:Platform.exe"
        } else {
            Remove-Item -Recurse -Force $tempDir -ErrorAction SilentlyContinue
            return $false
        }
    }
    
    Write-Info $(Get-Text movingBinary)
    
    # 创建安装目录
    if (-not (Test-Path $global:InstallDir)) {
        New-Item -ItemType Directory -Path $global:InstallDir -Force
    }
    
    # 复制文件（需要管理员权限）
    try {
        Copy-Item -Path $tempFile -Destination "$global:InstallDir\superc.exe" -Force -ErrorAction Stop
    } catch {
        Write-Warning $(Get-Text needAdmin)
        Start-Process powershell -ArgumentList "-Command", "Copy-Item -Path '$tempFile' -Destination '$global:InstallDir\superc.exe' -Force" -Verb RunAs -Wait
    }
    
    # 更新 PATH
    $userPath = [Environment]::GetEnvironmentVariable("PATH", "User")
    if ($userPath -notlike "*$global:InstallDir*") {
        Write-Info $(Get-Text updatingPath)
        [Environment]::SetEnvironmentVariable("PATH", $userPath + ";$global:InstallDir", "User")
        $env:PATH += ";$global:InstallDir"
    }
    
    Remove-Item -Recurse -Force $tempDir -ErrorAction SilentlyContinue
    return $true
}

# 源码构建安装
function Install-FromSource {
    Write-Info $(Get-Text sourceInstall)
    Write-Warning "需要安装: git, make, gcc/clang (建议使用 MSYS2 或 WSL)"
    
    $tempDir = [IO.Path]::GetTempPath() + "super-source-" + [Guid]::NewGuid()
    Write-Info "克隆源码..."
    git clone --depth 1 --branch "v$global:Version" "https://github.com/$RepoOwner/$RepoName.git" $tempDir
    
    Write-Info "编译中..."
    Set-Location $tempDir
    make -j$(Get-CpuCount)
    
    Write-Info $(Get-Text movingBinary)
    if (-not (Test-Path $global:InstallDir)) {
        New-Item -ItemType Directory -Path $global:InstallDir -Force
    }
    Copy-Item -Path "build\bin\superc.exe" -Destination "$global:InstallDir\superc.exe" -Force
    
    # 更新 PATH
    $userPath = [Environment]::GetEnvironmentVariable("PATH", "User")
    if ($userPath -notlike "*$global:InstallDir*") {
        Write-Info $(Get-Text updatingPath)
        [Environment]::SetEnvironmentVariable("PATH", $userPath + ";$global:InstallDir", "User")
        $env:PATH += ";$global:InstallDir"
    }
    
    Set-Location $originalDir
    Remove-Item -Recurse -Force $tempDir -ErrorAction SilentlyContinue
    return $true
}

function Get-CpuCount {
    try { return (Get-CimInstance Win32_Processor).NumberOfLogicalProcessors }
    catch { return 4 }
}

# 验证安装
function Verify-Installation {
    if (Get-Command super -ErrorAction SilentlyContinue) {
        Write-Success $(Get-Text installSuccess)
        super --version
        Write-Host ""
        Write-Info $(Get-Text runSuperHelp)
        return $true
    } else {
        Write-Error "$(Get-Text installFailed): super 命令不可用"
        Write-Warning "请确保 $global:InstallDir 在 PATH 中，或重启终端"
        return $false
    }
}

# 主函数
function Main {
    # 非交互模式
    if ($NonInteractive -or -not [Console]::IsInputRedirected) {
        $global:SkipConfirm = $true
        $global:Language = "en"
    }
    
    # 设置语言
    if ($Language) {
        $global:Language = $Language
    }
    if ($global:Language -eq "en") { $Texts = $TextsEn } else { $Texts = $TextsZh }
    
    # 交互式配置
    if (-not $NonInteractive) {
        Select-Language
        if ($InstallMode) { $global:InstallMode = $InstallMode } else { Select-InstallMode }
        if ($ProxyType) {
            switch ($ProxyType) {
                "none" { $global:UseProxy = ""; $global:ProxyUrl = "" }
                "http" { $global:UseProxy = "http"; $global:ProxyUrl = $ProxyUrl }
                "socks5" { $global:UseProxy = "socks5"; $global:ProxyUrl = $ProxyUrl }
            }
        } else { Configure-Proxy }
        if ($InstallDir) { $global:InstallDir = $InstallDir } else { Get-InstallDir }
        if (-not (Confirm-Install)) { Write-Warning "安装已取消"; exit 0 }
    }
    
    Write-Info $(Get-Text installing)
    
    $success = $false
    switch ($global:InstallMode) {
        "binary" { $success = Install-Binary }
        "source" { $success = Install-FromSource }
        "auto" {
            if (-not (Install-Binary)) {
                Write-Warning "二进制安装失败，尝试源码构建..."
                $success = Install-FromSource
            } else { $success = $true }
        }
    }
    
    if (-not $success) { Write-Error $(Get-Text installFailed); exit 1 }
    
    Verify-Installation
    Write-Success $(Get-Text done)
}

# 全局变量
$global:Version = $Version
$global:InstallDir = $InstallDir
$global:SkipConfirm = $false
$global:Language = "zh"
$global:InstallMode = ""
$global:UseProxy = ""
$global:ProxyUrl = ""
$global:Arch = ""
$global:Platform = ""
$global:BinaryName = ""
$originalDir = Get-Location

# 运行主函数
try {
    Main
} catch {
    Write-Error "安装异常: $_"
    exit 1
}