# XShell 依赖文档

本文档列出了编译和运行 XShell 所需的所有依赖项。

## 系统要求

### 操作系统
- **Linux** (推荐 Ubuntu 20.04+)
- **WSL2** (Windows Subsystem for Linux 2)
- 其他类 Unix 系统 (macOS, FreeBSD 等需要少量调整)

### 编译工具链

| 依赖 | 版本要求 | 说明 |
|------|----------|------|
| GCC | 7.0+ | GNU C 编译器 |
| Make | 4.0+ | 构建工具 |
| glibc | 2.17+ | GNU C 标准库 |

### 安装编译依赖 (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential
```

### 安装编译依赖 (CentOS/RHEL)

```bash
sudo yum groupinstall "Development Tools"
```

### 安装编译依赖 (Arch Linux)

```bash
sudo pacman -S base-devel
```

---

## Web 界面依赖

### P5 主题界面 (最终版本)

| 依赖 | 版本要求 | 说明 |
|------|----------|------|
| Python | 3.8+ | Python 解释器 |
| Flask | 3.0.0 | Web 框架 |

**安装方式：**
```bash
cd p5_interface
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

### Streamlit 界面 (v1 版本)

| 依赖 | 版本要求 | 说明 |
|------|----------|------|
| Python | 3.8+ | Python 解释器 |
| Streamlit | 1.28+ | Web 应用框架 |

**安装方式：**
```bash
cd streamlit_demo
pip install streamlit
```

---

## 快速开始

### 1. 编译 C 核心

```bash
cd xhell
make clean
make
```

编译成功后会生成 `xhell` 可执行文件。

### 2. 运行纯命令行模式

```bash
cd xhell
./xhell
```

进入交互式 Shell 后，可以使用 `xhelp` 查看所有可用命令。

### 3. 运行 Web 界面

**P5 主题界面：**
```bash
cd p5_interface
./start.sh
# 访问 http://localhost:8505
```

**Streamlit 界面：**
```bash
cd streamlit_demo
streamlit run app.py
# 访问 http://localhost:8501
```

---

## 项目结构

```
unix-shell-xhell/
├── xhell/                  # C 核心 (仅依赖 build-essential)
├── p5_interface/           # Flask Web 界面 (依赖 Python + Flask)
├── streamlit_demo/         # Streamlit 界面 (依赖 Python + Streamlit)
└── docs/                   # 文档和截图
```

---

## 常见问题

### Q: 编译时提示 "gcc: command not found"
A: 需要安装 build-essential：`sudo apt install build-essential`

### Q: Python 版本过低
A: XShell Web 界面需要 Python 3.8+，可以使用 pyenv 或 conda 管理 Python 版本。

### Q: WSL 中端口无法访问
A: 确保使用 `localhost` 而非 Windows IP 访问，或检查 Windows 防火墙设置。
