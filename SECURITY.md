# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 0.0.x   | :white_check_mark: |

## Reporting a Vulnerability

如果您发现安全漏洞，请**不要**公开提交 Issue。

请通过以下方式私下报告：

- 邮件：security@super-lang.org
- GitHub Security Advisories：使用 "Report a vulnerability" 功能

我们会在 48 小时内确认收到，并在 7 天内提供修复计划。

## 安全最佳实践

- 编译器默认开启栈保护和地址空间随机化
- 运行时内存分配器包含边界检查
- 建议生产环境使用 `#GC = True` 启用垃圾回收