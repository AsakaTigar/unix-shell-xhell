# XShell

A Unix-like shell implementation written in C, featuring pipeline support, I/O redirection, built-in commands, and a modern Streamlit web interface for demonstration.

## 🚀 Features

### Core Shell Capabilities
- **Multi-level Pipelines**: Chain commands with `|` operator (`cmd1 | cmd2 | cmd3`)
- **I/O Redirection**: Support for `<`, `>`, `>>`, and `2>` operators
- **Built-in Commands**: 18+ built-in commands including file operations, system utilities
- **External Program Execution**: Execute system binaries via `fork()` + `execv()`
- **Command History**: Persistent command history across sessions

### Advanced Features
- **xsearch**: Built-in grep-like text search utility
- **xcalc**: Command-line calculator for arithmetic operations
- **xsh**: Script interpreter for `.x` script files
- **xsysinfo**: System resource monitoring
- **Colorized Output**: ANSI-colored `xls` output for better readability

### Modern UI
- **Streamlit Web Interface**: Interactive web-based terminal emulator
- **One-Click Demos**: Pre-configured demonstration buttons
- **Real-time Output**: Live command execution visualization
- **File Workspace**: Integrated file browser and editor

## 📦 Installation

### Prerequisites
```bash
# Linux/Unix system
gcc make python3 pip
```

### Build from Source
```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/xshell.git
cd xshell

# Compile the shell
cd xhell
make
./xhell
```

### Run Web Interface
```bash
# Install Python dependencies
pip install streamlit

# Launch the demo
./start_demo.sh

# Open browser at http://localhost:8501
```

## 🎮 Usage

### Interactive Mode
```bash
$ ./xhell
[xshell]# xls -l          # List files with details
[xshell]# xpwd            # Print working directory
[xshell]# xcat file.txt   # Display file contents
[xshell]# quit            # Exit shell
```

### Pipeline Examples
```bash
# Count files in directory
xls | wc -l

# Search in command output
xls -l | xsearch .txt

# Chained grep-like operations
xcat log.txt | xsearch ERROR | wc -l
```

### Redirection Examples
```bash
# Output redirection
xecho "Hello World" > output.txt

# Append mode
xecho "New line" >> output.txt

# Input redirection
xcat < input.txt

# Error redirection
xcp nonexist.txt dst.txt 2> error.log
```

### Built-in Commands
| Command | Description |
|---------|-------------|
| `xpwd` | Print working directory |
| `xcd [dir]` | Change directory |
| `xls [-l] [dir]` | List directory contents (with colors) |
| `xtouch <file>` | Create empty file |
| `xcat <file>` | Display file contents |
| `xcp [-r] <src> <dst>` | Copy files/directories |
| `xmv <src> <dst>` | Move/rename files |
| `xrm [-r] <path>` | Remove files/directories |
| `xecho [text]` | Print text to stdout |
| `xsearch <term> [file]` | Search text in file or stdin |
| `xcalc <n1> <op> <n2>` | Calculate arithmetic expression |
| `xsh <script.x>` | Execute shell script |
| `xhistory` | Show command history |
| `xsysinfo` | Display system information |
| `xhelp` | Show all available commands |

## 🏗️ Architecture

```
┌─────────────────────────────────────────┐
│           User Input (CLI/Web)          │
└──────────────────┬──────────────────────┘
                   │
                   ▼
         ┌─────────────────┐
         │  Parser Module  │
         │  (parser.c)     │
         └────────┬────────┘
                  │
                  ▼
         ┌────────────────────┐
         │  Pipeline Structure │
         └────────┬───────────┘
                  │
                  ▼
    ┌─────────────────────────────┐
    │   Executor (pipe.c)         │
    │   - fork() child processes  │
    │   - pipe() for IPC          │
    │   - dup2() for redirection  │
    └──────────┬──────────────────┘
               │
       ┌───────┴────────┐
       ▼                ▼
┌─────────────┐   ┌────────────────┐
│  Built-in   │   │   External     │
│  Commands   │   │   Programs     │
│(builtins.c) │   │(external_exec.c)│
└─────────────┘   └────────────────┘
```

## 📁 Project Structure

```
xshell/
├── xhell/                  # Core C implementation
│   ├── src/
│   │   ├── main.c         # REPL loop
│   │   ├── parser.c       # Command parser
│   │   ├── pipe.c         # Pipeline executor
│   │   ├── builtin_commands.c
│   │   ├── redirection.c
│   │   ├── external_exec.c
│   │   ├── utils.c
│   │   └── logger.c
│   ├── include/
│   │   └── xhell.h
│   └── Makefile
├── streamlit_demo/        # Web interface
│   ├── app.py
│   └── xhell_wrapper.py
├── start_demo.sh
└── README.md
```

## 🔧 Technical Highlights

1. **Process Management**: Robust `fork()`/`execv()`/`waitpid()` implementation
2. **IPC via Pipes**: Multi-stage pipeline with proper file descriptor management
3. **Resource Safety**: Automatic cleanup of zombie processes and file descriptors
4. **ANSI Colors**: Terminal output enhancement with escape sequences
5. **Error Handling**: Comprehensive `perror()` and status code management

## 📝 License

MIT License - feel free to use this project for learning and development.

## 🙏 Acknowledgments

Built as a demonstration of Unix systems programming concepts including:
- Process creation and management
- Inter-process communication
- File I/O and system calls
- Signal handling
- POSIX API usage

---

**Note**: This is an educational project. For production use, consider mature shells like Bash, Zsh, or Fish.
