Super-Lang
==========

一门支持自动推导所有权、显式错误处理、内置构建工具链的现代系统级编程语言。

特性
----

* 自动所有权推导：编译期自动管理内存，无需手写生命周期
* 显式错误处理：return err 模式，强制处理错误
* 内置工具链：super init/run/build/tidy/mod/file 一站式开发
* C 互操作：直接生成 C 代码，零依赖编译
* 并发原语：内置 CSP 风格通道
* 泛型支持：编译期单态化，零运行时开销

快速开始
--------

# 构建编译器
make -j4

# 创建新项目
./build/bin/superc init myproj
cd myproj

# 编写代码 (main.super)
# #main
# fn main() -> int {
#     out("Hello, Super!")
#     return 0
# }

# 运行
./build/bin/superc run main.super

# 构建可执行文件
./build/bin/superc build main.super

语法示例
--------

#main

include fmt

struct Point {
    x int
    y int
}

fn distance(p Point) -> float {
    return math_sqrt(p.x * p.x + p.y * p.y)
}

pub fn main() -> int {
    p Point = {3, 4}
    d float = distance(p)
    fmt_printf("Distance: %f\n", d)
    return 0
}

项目结构
--------

Super-Lang/
├── src/
│   ├── cmd/superc/       # 编译器入口
│   ├── super/            # 编译器核心包
│   │   ├── lexer/        # 词法分析
│   │   ├── parser/       # 语法分析
│   │   ├── ast/          # 抽象语法树
│   │   ├── sema/         # 语义分析
│   │   ├── codegen/      # C 代码生成
│   │   └── driver/       # 驱动与诊断
│   ├── runtime/          # 运行时
│   └── fmt/os/sys/stk/math/  # 标准库
├── tests/                # 测试套件
├── docs/                 # 语言文档
├── AI docs/              # 内部设计文档
└── Makefile              # 构建系统

开发路线图
----------

详见 AI docs/ROADMAP.md

* P0: 语言规范冻结
* P1: C 引导编译器
* P2: 标准库与工具链
* P3: 自举编译器
* P4: Go 重写与生产化

贡献
----

请阅读 CONTRIBUTING.txt 和 SECURITY.md

许可证
------

MIT License - 详见 LICENSE.txt