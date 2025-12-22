#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Xhell演示项目启动器
双击运行即可自动启动Streamlit服务并打开浏览器
"""

import os
import sys
import time
import subprocess
import webbrowser
import socket
from pathlib import Path
import threading
import signal


def find_free_port(start_port=8501):
    """查找可用端口"""
    port = start_port
    while port < start_port + 100:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.bind(('localhost', port))
                return port
        except OSError:
            port += 1
    return start_port


def get_base_path():
    """获取应用程序基础路径（支持打包后的exe）"""
    if getattr(sys, 'frozen', False):
        # 如果是打包后的exe
        return Path(sys._MEIPASS)
    else:
        # 如果是开发环境
        return Path(__file__).parent


def check_xhell_binary():
    """检查xhell可执行文件是否存在"""
    base_path = get_base_path()
    xhell_path = base_path / "xhell" / "bin" / "xhell"
    
    if not xhell_path.exists():
        print(f"⚠️  警告: 未找到xhell可执行文件: {xhell_path}")
        print("   某些功能可能无法正常工作")
        return False
    return True


def start_streamlit(port):
    """启动Streamlit服务"""
    base_path = get_base_path()
    app_path = base_path / "streamlit_demo" / "app.py"
    
    if not app_path.exists():
        print(f"❌ 错误: 找不到应用文件 {app_path}")
        return None
    
    # 设置环境变量
    env = os.environ.copy()
    env['STREAMLIT_SERVER_PORT'] = str(port)
    env['STREAMLIT_SERVER_HEADLESS'] = 'true'
    env['STREAMLIT_BROWSER_GATHER_USAGE_STATS'] = 'false'
    
    # 启动streamlit
    cmd = [
        sys.executable,
        '-m', 'streamlit',
        'run',
        str(app_path),
        '--server.port', str(port),
        '--server.headless', 'true',
        '--browser.gatherUsageStats', 'false',
        '--server.address', 'localhost'
    ]
    
    try:
        process = subprocess.Popen(
            cmd,
            env=env,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=str(base_path)
        )
        return process
    except Exception as e:
        print(f"❌ 启动Streamlit失败: {e}")
        return None


def wait_for_server(port, timeout=30):
    """等待服务器启动"""
    start_time = time.time()
    while time.time() - start_time < timeout:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(1)
                s.connect(('localhost', port))
                return True
        except (socket.timeout, ConnectionRefusedError):
            time.sleep(0.5)
    return False


def open_browser(url):
    """在默认浏览器中打开URL"""
    try:
        webbrowser.open(url)
        return True
    except Exception as e:
        print(f"⚠️  自动打开浏览器失败: {e}")
        print(f"   请手动访问: {url}")
        return False


def print_banner():
    """打印欢迎横幅"""
    banner = """
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║              🚀 Xhell 项目演示系统 🚀                    ║
║                                                           ║
║         Linux操作系统课程设计 - Shell解释器              ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
"""
    print(banner)


def main():
    """主函数"""
    print_banner()
    
    # 检查xhell二进制文件
    print("🔍 检查项目文件...")
    check_xhell_binary()
    
    # 查找可用端口
    print("🔍 查找可用端口...")
    port = find_free_port()
    print(f"✅ 使用端口: {port}")
    
    # 启动Streamlit
    print("🚀 启动Streamlit服务...")
    process = start_streamlit(port)
    
    if not process:
        print("\n❌ 启动失败！按任意键退出...")
        input()
        sys.exit(1)
    
    # 等待服务器启动
    print("⏳ 等待服务器就绪...")
    if wait_for_server(port, timeout=30):
        print("✅ 服务器已就绪！")
        
        # 打开浏览器
        url = f"http://localhost:{port}"
        print(f"🌐 打开浏览器: {url}")
        time.sleep(1)  # 稍微等待确保服务器完全就绪
        open_browser(url)
        
        print("\n" + "="*60)
        print("✅ Xhell演示系统已启动！")
        print(f"📱 访问地址: {url}")
        print("⚠️  请勿关闭此窗口，否则演示系统将停止运行")
        print("🛑 按 Ctrl+C 停止服务")
        print("="*60 + "\n")
        
        # 等待进程
        try:
            process.wait()
        except KeyboardInterrupt:
            print("\n\n🛑 正在停止服务...")
            process.terminate()
            try:
                process.wait(timeout=5)
            except subprocess.TimeoutExpired:
                process.kill()
            print("✅ 服务已停止")
    else:
        print("❌ 服务器启动超时")
        process.terminate()
        print("\n按任意键退出...")
        input()
        sys.exit(1)


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"\n❌ 发生错误: {e}")
        import traceback
        traceback.print_exc()
        print("\n按任意键退出...")
        input()
        sys.exit(1)
