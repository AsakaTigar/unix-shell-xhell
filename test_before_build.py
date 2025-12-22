#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试launcher.py是否能正常工作
在打包之前运行此脚本进行测试
"""

import sys
import subprocess
import time
from pathlib import Path


def test_launcher():
    """测试启动器"""
    print("="*60)
    print("  Launcher测试工具")
    print("="*60)
    print()
    
    # 检查launcher.py是否存在
    launcher = Path("launcher.py")
    if not launcher.exists():
        print("❌ 错误: 找不到 launcher.py")
        return False
    
    print("✅ launcher.py 存在")
    
    # 检查streamlit_demo是否存在
    streamlit_demo = Path("streamlit_demo")
    if not streamlit_demo.exists():
        print("❌ 错误: 找不到 streamlit_demo 目录")
        return False
    
    print("✅ streamlit_demo 目录存在")
    
    # 检查必要的文件
    app_py = streamlit_demo / "app.py"
    if not app_py.exists():
        print("❌ 错误: 找不到 streamlit_demo/app.py")
        return False
    
    print("✅ streamlit_demo/app.py 存在")
    
    # 检查依赖
    print("\n检查Python依赖...")
    # 注意：包名和导入名可能不同
    required_packages = {
        'streamlit': 'streamlit',
        'pandas': 'pandas', 
        'numpy': 'numpy',
        'pygments': 'Pygments'  # 包名是小写，显示名是大写
    }
    missing_packages = []
    
    for import_name, display_name in required_packages.items():
        try:
            __import__(import_name)
            print(f"✅ {display_name} 已安装")
        except ImportError:
            print(f"❌ {display_name} 未安装")
            missing_packages.append(display_name)
    
    if missing_packages:
        print(f"\n⚠️  缺少以下依赖: {', '.join(missing_packages)}")
        print("   请运行: pip install -r requirements.txt")
        return False
    
    print("\n" + "="*60)
    print("  ✅ 所有检查通过！")
    print("="*60)
    print()
    
    # 询问是否测试运行
    response = input("是否测试运行launcher.py? (y/n, 默认n): ").strip().lower()
    
    if response == 'y':
        print("\n🚀 启动launcher.py...")
        print("⚠️  测试运行将启动Streamlit服务")
        print("   按Ctrl+C可以停止")
        print()
        
        try:
            subprocess.run([sys.executable, "launcher.py"])
        except KeyboardInterrupt:
            print("\n\n✅ 测试完成")
    
    return True


def check_pyinstaller():
    """检查PyInstaller是否已安装"""
    print("\n" + "="*60)
    print("  PyInstaller检查")
    print("="*60)
    print()
    
    try:
        import PyInstaller
        print(f"✅ PyInstaller已安装 (版本: {PyInstaller.__version__})")
        return True
    except ImportError:
        print("❌ PyInstaller未安装")
        print("   请运行: pip install pyinstaller")
        return False


def main():
    """主函数"""
    print()
    
    # 测试launcher
    if not test_launcher():
        print("\n❌ 测试失败")
        sys.exit(1)
    
    # 检查PyInstaller
    check_pyinstaller()
    
    print("\n" + "="*60)
    print("  📝 下一步")
    print("="*60)
    print()
    print("如果所有检查都通过，可以开始打包：")
    print()
    print("Windows系统:")
    print("  1. 双击 build_exe.bat（推荐）")
    print("  或")
    print("  2. 运行: python build_exe.py")
    print()
    print("Linux系统:")
    print("  运行: python build_exe.py")
    print("  （注意：会生成Linux可执行文件，不是Windows exe）")
    print()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n已取消")
        sys.exit(0)
    except Exception as e:
        print(f"\n❌ 发生错误: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
