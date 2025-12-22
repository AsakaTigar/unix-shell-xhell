#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Xhell项目打包脚本
使用PyInstaller将项目打包成Windows exe文件
"""

import PyInstaller.__main__
import shutil
from pathlib import Path
import sys


def build_executable():
    """构建可执行文件"""
    
    # 当前目录
    current_dir = Path(__file__).parent
    
    # 确认必要文件存在
    launcher = current_dir / "launcher.py"
    if not launcher.exists():
        print(f"❌ 错误: 找不到 launcher.py")
        sys.exit(1)
    
    streamlit_demo = current_dir / "streamlit_demo"
    if not streamlit_demo.exists():
        print(f"❌ 错误: 找不到 streamlit_demo 目录")
        sys.exit(1)
    
    print("🔧 开始构建exe文件...")
    print(f"📁 项目目录: {current_dir}")
    
    # PyInstaller参数
    pyinstaller_args = [
        str(launcher),  # 主程序
        '--name=XhellDemo',  # 输出文件名
        '--onefile',  # 打包成单个exe
        '--windowed',  # 不显示控制台窗口（如果需要看日志可以去掉这个参数）
        '--noconfirm',  # 覆盖输出目录
        '--clean',  # 清理临时文件
        
        # 添加数据文件
        f'--add-data={streamlit_demo};streamlit_demo',
        
        # 添加xhell目录（如果存在）
        f'--add-data={current_dir / "xhell"};xhell' if (current_dir / "xhell").exists() else '',
        
        # 隐藏导入（确保这些模块被打包）
        '--hidden-import=streamlit',
        '--hidden-import=streamlit.web.cli',
        '--hidden-import=streamlit.runtime',
        '--hidden-import=streamlit.runtime.scriptrunner',
        '--hidden-import=streamlit.runtime.scriptrunner.script_runner',
        '--hidden-import=pandas',
        '--hidden-import=numpy',
        '--hidden-import=Pygments',
        '--hidden-import=click',
        '--hidden-import=tornado',
        '--hidden-import=altair',
        '--hidden-import=pyarrow',
        
        # 收集所有streamlit相关文件
        '--collect-all=streamlit',
        '--collect-all=altair',
        '--collect-all=pyarrow',
        
        # 图标（可选，如果有图标文件的话）
        # '--icon=icon.ico',
        
        # 输出目录
        '--distpath=dist',
        '--workpath=build',
        '--specpath=.',
    ]
    
    # 过滤空字符串
    pyinstaller_args = [arg for arg in pyinstaller_args if arg]
    
    print("🛠️  PyInstaller参数:")
    for arg in pyinstaller_args:
        print(f"   {arg}")
    
    try:
        PyInstaller.__main__.run(pyinstaller_args)
        print("\n✅ 构建完成！")
        print(f"📦 可执行文件位置: {current_dir / 'dist' / 'XhellDemo.exe'}")
        print("\n📝 使用说明:")
        print("   1. 将 dist/XhellDemo.exe 复制到任何位置")
        print("   2. 双击 XhellDemo.exe 即可启动")
        print("   3. 程序会自动打开浏览器显示演示界面")
        
    except Exception as e:
        print(f"\n❌ 构建失败: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    print("="*60)
    print("  Xhell项目打包工具")
    print("="*60)
    print()
    
    # 检查是否在Linux上运行
    if sys.platform.startswith('linux'):
        print("⚠️  警告: 你正在Linux系统上运行")
        print("   如果要打包成Windows exe，建议在Windows系统上运行此脚本")
        print("   或者使用 Wine 环境")
        response = input("\n是否继续? (y/n): ")
        if response.lower() != 'y':
            print("已取消")
            sys.exit(0)
    
    build_executable()
