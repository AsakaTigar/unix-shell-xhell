"""
Pre-configured demonstration scenarios for Xhell features
"""

# Built-in commands demonstrations
BUILTIN_DEMOS = {
    "基础命令": [
        "xpwd",
        "xecho Hello from Xhell!",
        "xtouch test_file.txt",
        "xls",
    ],
    "目录操作": [
        "xpwd",
        "xcd /tmp",
        "xpwd",
        "xcd -",
        "xpwd",
    ],
    "文件操作": [
        "xtouch demo.txt",
        "xecho This is a test > demo.txt",
        "xcat demo.txt",
        "xcp demo.txt demo_copy.txt",
        "xls",
    ],
    "历史和日志": [
        "xecho Command 1",
        "xecho Command 2",
        "xecho Command 3",
        "xhistory",
        "xjournalctl",
    ],
}

# I/O Redirection demonstrations
REDIRECTION_DEMOS = {
    "输出重定向 (>)": [
        "xecho Hello World > output.txt",
        "xcat output.txt",
        "xecho New content > output.txt",
        "xcat output.txt",
    ],
    "追加重定向 (>>)": [
        "xecho Line 1 > append_test.txt",
        "xecho Line 2 >> append_test.txt",
        "xecho Line 3 >> append_test.txt",
        "xcat append_test.txt",
    ],
}

# Pipe demonstrations
PIPE_DEMOS = {
    "简单管道": [
        "xecho Hello World | wc -w",
        "xls | wc -l",
    ],
}

# External program demonstrations
EXTERNAL_DEMOS = {
    "外部程序": [
        "ls -la",
        "pwd",
        "echo External command test",
        "date",
    ],
}

# Comprehensive demo
COMPREHENSIVE_DEMO = [
    "# 欢迎使用 Xhell 演示",
    "xpwd",
    "xecho === 创建测试文件 ===",
    "xtouch file1.txt",
    "xtouch file2.txt",
    "xecho Content for file1 > file1.txt",
    "xecho Content for file2 > file2.txt",
    "xls",
    "xecho === 查看文件内容 ===",
    "xcat file1.txt",
    "xcat file2.txt",
    "xecho === 复制和移动 ===",
    "xcp file1.txt file1_backup.txt",
    "xls",
    "xecho === 查看历史 ===",
    "xhistory",
]

def get_all_demos():
    """Get all demonstration scenarios"""
    return {
        "内置命令": BUILTIN_DEMOS,
        "I/O 重定向": REDIRECTION_DEMOS,
        "管道功能": PIPE_DEMOS,
        "外部程序": EXTERNAL_DEMOS,
        "综合演示": {"完整演示": COMPREHENSIVE_DEMO},
    }
