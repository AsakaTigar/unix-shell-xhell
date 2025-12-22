@echo off
chcp 65001 >nul
echo ========================================
echo   Xhell项目 - 一键打包工具
echo ========================================
echo.

REM 检查Python是否安装
python --version >nul 2>&1
if errorlevel 1 (
    echo ❌ 错误: 未检测到Python，请先安装Python
    echo    下载地址: https://www.python.org/downloads/
    pause
    exit /b 1
)

echo ✅ Python已安装
python --version
echo.

REM 检查pip是否可用
pip --version >nul 2>&1
if errorlevel 1 (
    echo ❌ 错误: pip不可用
    pause
    exit /b 1
)

echo ✅ pip已安装
echo.

REM 询问是否安装依赖
echo 📦 准备安装依赖...
set /p INSTALL_DEPS="是否需要安装/更新依赖? (y/n, 默认y): "
if "%INSTALL_DEPS%"=="" set INSTALL_DEPS=y

if /i "%INSTALL_DEPS%"=="y" (
    echo.
    echo 🔧 安装项目依赖...
    pip install -r requirements.txt
    if errorlevel 1 (
        echo ❌ 依赖安装失败
        pause
        exit /b 1
    )
    
    echo.
    echo 🔧 安装PyInstaller...
    pip install pyinstaller
    if errorlevel 1 (
        echo ❌ PyInstaller安装失败
        pause
        exit /b 1
    )
    
    echo ✅ 依赖安装完成
)

echo.
echo ========================================
echo   开始打包...
echo ========================================
echo.

REM 运行打包脚本
python build_exe.py

if errorlevel 1 (
    echo.
    echo ❌ 打包失败！
    pause
    exit /b 1
)

echo.
echo ========================================
echo   ✅ 打包成功！
echo ========================================
echo.
echo 📦 可执行文件: dist\XhellDemo.exe
echo.
echo 📝 接下来的步骤:
echo    1. 进入 dist 文件夹
echo    2. 双击 XhellDemo.exe 测试
echo    3. 如果运行正常，即可用于课堂演示
echo.

REM 询问是否立即测试
set /p TEST_NOW="是否立即测试exe文件? (y/n, 默认n): "
if /i "%TEST_NOW%"=="y" (
    echo.
    echo 🚀 启动XhellDemo.exe...
    start "" "dist\XhellDemo.exe"
)

echo.
echo ✨ 打包完成！祝演示顺利！
echo.
pause
