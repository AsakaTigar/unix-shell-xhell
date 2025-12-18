import streamlit as st
import os
import sys
import time
import re
from xhell_wrapper import XhellWrapper

# ANSI color code stripper
def strip_ansi(text):
    """Remove ANSI escape sequences from text"""
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    return ansi_escape.sub('', text)

# Set page configuration
st.set_page_config(
    page_title="Xhell 演示控制台",
    page_icon="🐚",
    layout="wide",
    initial_sidebar_state="expanded"
)

# Custom Styling
st.markdown("""
<style>
    /* Terminal Output Style */
    .stCodeBlock {
        background-color: #0e1117 !important;
    }
    .terminal-output {
        font-family: 'Courier New', monospace;
        background-color: #1e1e1e;
        color: #4af626;
        padding: 15px;
        border-radius: 8px;
        border: 1px solid #333;
        margin-bottom: 20px;
        max-height: 400px;
        overflow-y: auto;
    }
    .command-prompt {
        color: #00bcd4;
        font-weight: bold;
    }
    /* Button Group Styling */
    .stButton button {
        width: 100%;
        text-align: left !important;
        justify-content: flex-start !important;
        padding-left: 20px !important;
    }
    /* File Viewer Style */
    .file-preview {
        border: 1px solid #444;
        padding: 10px;
        border-radius: 5px;
        background-color: #2b2b2b;
    }
</style>
""", unsafe_allow_html=True)

# Initialize Session State
if 'xhell' not in st.session_state:
    workspace_dir = "./demo_workspace"
    st.session_state.xhell = XhellWrapper(workspace_dir=workspace_dir)
    # Ensure workspace is clean initially if needed, or keep it
    if not os.path.exists(workspace_dir):
        os.makedirs(workspace_dir)

if 'history' not in st.session_state:
    st.session_state.history = []

if 'last_cmd' not in st.session_state:
    st.session_state.last_cmd = ""

def run_command(cmd, desc=None, prefix=None, rerun=True):
    """Execute command and update state"""
    st.session_state.last_cmd = cmd
    
    # Add a visual marker for execution
    with st.spinner(f"正在执行: {cmd}"):
        time.sleep(0.2) # Short delay for visual feedback
        result = st.session_state.xhell.execute_command(cmd)
        
        entry = {
            'command': cmd,
            'desc': desc,
            'prefix': prefix,
            'output': strip_ansi(result['stdout']),
            'error': strip_ansi(result['stderr']),
            'timestamp': time.strftime("%H:%M:%S")
        }
        st.session_state.history.insert(0, entry) # Prepend to show newest first
        
        # Only rerun if requested (default True)
        # This allows chaining multiple commands in one button click
        if rerun:
            st.rerun()

# --- Sidebar: Command Palette ---
with st.sidebar:
    st.title("🚀 快捷指令面板")
    st.info("点击按钮直接执行命令")
    
    # 1. Basic Commands
    with st.expander("📂 1. 基础命令 (文件/目录)", expanded=True):
        c1, c2 = st.columns(2)
        with c1:
            if st.button("1-1. 查看当前路径 (xpwd)"):
                run_command("xpwd", "显示当前工作目录", "1-1")
            if st.button("1-2. 列出文件 (xls)"):
                run_command("xls", "列出当前目录下的文件", "1-2")
            if st.button("1-5. 复制 (xcp)"):
                run_command("xtouch src.txt", "准备源文件", "1-5-PRE", rerun=False)
                run_command("xcp src.txt dst.txt", "复制文件 src -> dst", "1-5")
            if st.button("1-7. 删除 (xrm)"):
                run_command("xtouch junk.txt", "准备垃圾文件", "1-7-PRE", rerun=False)
                run_command("xrm junk.txt", "删除文件", "1-7")

        with c2:
            if st.button("1-3. 详细列表 (xls -l)"):
                run_command("xls -l", "查看文件详细信息", "1-3")
            if st.button("1-4. 创建测试文件"):
                run_command("xtouch test.txt", "创建空文件", "1-4")
            if st.button("1-6. 移动 (xmv)"):
                run_command("xtouch old.txt", "准备源文件", "1-6-PRE", rerun=False)
                run_command("xmv old.txt new.txt", "移动/重命名 old -> new", "1-6")
            if st.button("1-8. 切换目录 (xcd ..)"):
                run_command("xcd ..", "返回上一级目录", "1-8")
            
    # 2. I/O Redirection
    with st.expander("📝 2. I/O 重定向", expanded=True):
        if st.button("2-1. 写入文件 (>)"):
            run_command("xecho 'Hello Xshell' > hello.txt", "覆盖写入文件", "2-1")
            
        if st.button("2-2. 追加内容 (>>)"):
            run_command("xecho 'New Line' >> hello.txt", "追加内容到文件", "2-2")
            
        if st.button("2-3. 查看内容 (xcat)"):
            run_command("xcat hello.txt", "查看文件内容", "2-3")

    # 3. Pipes
    with st.expander("🔗 3. 管道操作", expanded=True):
        if st.button("3-1. 统计单词 (wc -w)"):
            run_command("xecho 'A B C D' | wc -w", "通过管道统计单词数", "3-1")
            
        if st.button("3-2. 统计文件数 (wc -l)"):
            run_command("xls | wc -l", "统计当前目录文件数", "3-2")
            
        if st.button("3-3. 双向输出 (xtee)"):
            run_command("xecho 'Pipe & File' | xtee tee_out.txt", "同时输出到屏幕和文件", "3-3")

        if st.button("3-4. 管道搜索 (grep/xsearch)"):
            run_command("xls -l | xsearch .txt", "在文件列表中过滤.txt", "3-4")
            
    # 4. System & History
    with st.expander("⚙️ 4. 系统管理 (基础)"):
        if st.button("4-1. 查看历史 (xhistory)"):
            run_command("xhistory", "查看命令历史", "4-1")
            
        if st.button("4-2. 系统日志 (xjournalctl)"):
            run_command("xjournalctl", "查看 Xhell 内部日志", "4-2")
        
        if st.button("4-3. 清空环境 (Reset)"):
            # Clean workspace
            os.system("rm -rf demo_workspace/*")
            st.session_state.history = []
            st.rerun()

    # 5. Extra Features
    with st.expander("🌟 5. 扩展功能 (加分项)", expanded=True):
        c1, c2 = st.columns(2)
        with c1:
            if st.button("5-1. 系统信息 (xsysinfo)"):
                run_command("xsysinfo", "查看 CPU/内存/内核信息", "5-1")
                
            if st.button("5-3. 外部程序: whoami"):
                 run_command("/usr/bin/whoami", "调用外部程序显示当前用户", "5-3")
                 
        with c2:
            if st.button("5-2. 帮助菜单 (xhelp)"):
                run_command("xhelp", "列出所有支持的命令", "5-2")
                
            if st.button("5-4. 外部程序: ps"):
                 run_command("/bin/ps aux | head -n 10", "调用外部程序显示进程列表", "5-4")

        if st.button("5-5. 错误处理演示"):
             run_command("xcp non_exist.txt dst.txt", "演示文件不存在错误", "5-4", rerun=False)
             run_command("non_exist_cmd", "演示非法命令", "5-4")

    # 6. Bonus Features (New)
    with st.expander("💎 6. 特色功能 (Bonus)", expanded=True):
        c1, c2 = st.columns(2)
        with c1:
            if st.button("6-1. 计算器 (xcalc)"):
                run_command("xcalc 128 * 32", "命令行计算: 128 * 32", "6-1", rerun=False)
                run_command("xcalc 100 / 3", "支持浮点数除法", "6-1")
        with c2:
            if st.button("6-2. 执行脚本 (xsh)"):
                # Create a temporary script first
                script_content = """xecho "--- Script Start ---"
xtouch script_test.txt
xecho "Created by xsh" > script_test.txt
xls -l
xecho "--- Script End ---"
"""
                with open("demo_workspace/test.x", "w") as f:
                    f.write(script_content)
                
                run_command("xsh test.x", "批量执行脚本文件", "6-2")

            if st.button("6-3. 内容搜索 (xsearch)"):
                run_command("xecho 'Error: Something wrong' > log.txt", "准备日志文件", "6-3-PRE", rerun=False)
                run_command("xsearch Error log.txt", "搜索关键字", "6-3")

# --- Main Interface ---

col_term, col_file = st.columns([1.5, 1])

# Left Column: Terminal Output
with col_term:
    st.subheader("💻 终端输出")
    
    # Manual Input
    with st.form("manual_cmd"):
        user_cmd = st.text_input("手动输入命令:", placeholder="例如: xtouch myfile.txt")
        if st.form_submit_button("执行"):
            if user_cmd:
                run_command(user_cmd, "手动执行", "User")
                st.rerun()

    # History Display (Newest First)
    if not st.session_state.history:
        st.info("暂无操作记录，请点击左侧按钮开始演示。")
    
    for item in st.session_state.history:
        with st.container():
            prefix_str = f"**[{item['prefix']}]** " if item.get('prefix') else ""
            st.markdown(f"**[{item['timestamp']}]** {prefix_str}`{item['command']}`" + (f" ({item['desc']})" if item['desc'] else ""))
            
            output_content = ""
            if item['output']:
                output_content += item['output']
            if item['error']:
                output_content += f"\n[ERROR] {item['error']}"
                
            if output_content.strip():
                st.code(output_content, language="bash")
            else:
                st.caption("无输出")
            st.divider()

# Right Column: Live File Inspector
with col_file:
    st.subheader("📂 工作区文件监控")
    
    workspace_path = "./demo_workspace"
    if not os.path.exists(workspace_path):
        os.makedirs(workspace_path)
        
    try:
        files = sorted([f for f in os.listdir(workspace_path) if not f.startswith('.')])
        if not files:
            st.warning("工作区为空 (demo_workspace)")
        else:
            # File List
            selected_file = st.radio("选择文件查看内容:", files)
            
            # File Content Preview
            if selected_file:
                file_path = os.path.join(workspace_path, selected_file)
                st.markdown("---")
                st.markdown(f"**📄 {selected_file}**")
                try:
                    with open(file_path, 'r') as f:
                        content = f.read(2000) # Limit size
                        if content.strip():
                            st.code(content)
                        else:
                            st.info("文件为空")
                except Exception as e:
                    st.error(f"无法读取文件: {e}")
                    
    except Exception as e:
        st.error(f"文件系统错误: {e}")
        
    # Auto-refresh hint
    if st.button("🔄 刷新文件列表"):
        st.rerun()

