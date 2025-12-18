# Xhell - Linux 终端模拟器

这是一个用 C 语言编写的 Linux Shell 自定义实现，包含一个基于 Streamlit 的可视化交互演示界面。

## 项目结构

```
.
├── xhell/              # C 语言源代码
│   ├── src/           # 源文件 (.c)
│   ├── include/       # 头文件 (.h)
│   └── Makefile       # 编译构建文件
├── streamlit_demo/     # 交互式演示程序
│   ├── app.py         # Streamlit 主程序
│   ├── xhell_wrapper.py # Python-C 调用接口
│   └── demo_scenarios.py # 预设演示场景
├── docs/              # 额外文档
└── README.md          # 英文说明文档
```

## 功能特性

### 内置命令
- `xpwd`: 显示当前工作目录
- `xcd`: 切换目录
- `xls`: 列出目录内容
- `xtouch`: 创建空文件
- `xecho`: 输出文本
- `xcat`: 查看文件内容
- `xcp`: 复制文件或目录
- `xrm`: 删除文件或目录
- `xmv`: 移动/重命名文件
- `xhistory`: 查看命令历史
- `xjournalctl`: 查看 Shell 运行日志
- `quit`: 退出程序

### 高级功能
- **I/O 重定向**: 支持 `>` (覆盖) 和 `>>` (追加)
- **管道操作**: 支持 `|` 命令链接
- **外部程序调用**: 支持运行标准 Linux 命令 (如 ls, date 等)
- **错误处理**: 完善的错误报告机制

## 快速开始

### 1. 编译 Xhell

首先需要编译 C 语言核心程序：

```bash
cd xhell
make
```

这将生成 `xhell` 可执行文件。

### 2. 运行终端

你可以直接在命令行中运行 Xhell：

```bash
./xhell/xhell
```

### 3. 一键启动无需配置

为了方便演示，本项目提供了一键启动脚本，自动处理编译、环境检查和启动：

```bash
# 添加执行权限（首次）
chmod +x start_demo.sh

# 启动演示
./start_demo.sh
```

## 机房/Windows 环境演示指南

由于本项目基于 Linux 系统调用开发，**不能直接在 Windows 下运行**。在机房演示时，请务必遵循以下步骤：

### 准备工作
1. 确保机房电脑已启用 **WSL (Windows Subsystem for Linux)** 或安装了 Ubuntu 虚拟机。
2. 将本项目文件夹完整拷贝到系统中。

### 演示步骤
1. 打开 WSL 终端（在文件夹空白处按 Shift+右键 -> "在此处打开 Linux Shell" 或输入 `wsl`）。
2. 进入项目目录。
3. 运行一键脚本：`./start_demo.sh`
4. 脚本会自动编译代码并启动 Web 服务。
5. 复制终端显示的地址（如 `http://localhost:8501`）到浏览器打开即可。

### 演示细节
- **独立工作区**: 演示过程中创建的所有文件都会保存在自动生成的 `demo_workspace/` 目录下，不会污染项目代码。
- **全中文界面**: 演示系统已预置全中文引导，点击左侧按钮即可自动输入命令。

## 演示场景


Web 界面包含多个预设场景，可用于快速演示功能：
- **基础命令**: 演示文件创建、目录跳转等
- **I/O 重定向**: 演示将输出写入文件
- **管道功能**: 演示命令组合使用
- **综合演示**: 全功能的完整流程演示

## 常见问题

**Q: 运行 streamlit 提示 "bad interpreter" 错误？**
A: 这是因为 Anaconda 环境路径缓存问题。请尝试使用 `python -m streamlit run ...` 命令运行，或者在终端执行 `hash -r` 清除缓存。
