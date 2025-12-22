# 🚀 Xhell Demo - 快速打包指南

## ⚡ 最快速的方法（Windows系统）

### 第一步：准备环境
确保你的Windows电脑上安装了Python 3.8或更高版本。

### 第二步：一键打包
1. 将整个项目文件夹复制到Windows电脑
2. 双击运行 `build_exe.bat`
3. 按提示操作（建议全选"是"）

### 第三步：测试
打包完成后：
1. 进入 `dist` 文件夹
2. 双击 `XhellDemo.exe`
3. 浏览器会自动打开演示界面

### 第四步：课堂使用
- 将 `dist/XhellDemo.exe` 复制到U盘
- 在演示电脑上双击运行即可

## 📋 详细选项

如果自动打包脚本遇到问题，可以手动执行：

```bash
# 1. 安装依赖
pip install -r requirements.txt
pip install pyinstaller

# 2. 运行打包
python build_exe.py

# 或者使用spec文件
pyinstaller XhellDemo.spec
```

## 🎯 三种打包方式对比

| 方式 | 命令 | 优点 | 缺点 |
|------|------|------|------|
| **批处理（推荐）** | `build_exe.bat` | 全自动，最简单 | 仅Windows |
| **Python脚本** | `python build_exe.py` | 跨平台，可定制 | 需要手动装依赖 |
| **Spec文件** | `pyinstaller XhellDemo.spec` | 配置灵活 | 需要理解spec语法 |

## ❓ 常见问题速查

### Q: 我在Linux系统上，怎么打包Windows exe？

**A:** 有三个选项：
1. **推荐**：找一台Windows电脑或虚拟机打包
2. 使用Wine（复杂，不推荐）
3. 使用GitHub Actions自动构建（见下文）

### Q: exe文件太大了！

**A:** 这是正常的（80-150MB），因为包含了Python和所有库。如果要优化：
- 已经使用了 `--onefile`（单文件）
- 可以启用UPX压缩（spec文件中设置 `upx=True`）
- 或者使用文件夹模式（略大但启动更快）

### Q: 打包后运行报错？

**A:** 检查以下几点：
1. 在spec文件中设置 `console=True` 查看错误信息
2. 确保 `streamlit_demo` 文件夹被正确打包
3. 查看是否缺少隐藏导入
4. 尝试在打包的同一台电脑上测试

### Q: 演示时如何避免意外？

**A:** 三重保险：
1. **主方案**：打包的exe文件
2. **备份方案1**：提前在演示电脑上测试一次
3. **备份方案2**：准备录屏视频

## 📂 项目结构

```
linuxclass/
├── launcher.py              # 启动器（exe的主程序）
├── build_exe.py            # Python打包脚本
├── build_exe.bat           # Windows一键打包脚本
├── XhellDemo.spec          # PyInstaller配置
├── BUILD_EXE_README.md     # 详细文档
├── QUICKSTART.md           # 本文件
├── streamlit_demo/         # Streamlit应用（会被打包）
│   ├── app.py
│   ├── xhell_wrapper.py
│   └── demo_scenarios.py
├── xhell/                  # xhell程序（会被打包）
└── dist/                   # 打包输出目录
    └── XhellDemo.exe       # 最终的exe文件
```

## 🔧 高级：使用GitHub Actions自动构建

如果你想自动化构建Windows exe，可以创建 `.github/workflows/build.yml`：

```yaml
name: Build EXE

on:
  push:
    branches: [ main ]
  workflow_dispatch:

jobs:
  build:
    runs-on: windows-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up Python
      uses: actions/setup-python@v4
      with:
        python-version: '3.10'
    
    - name: Install dependencies
      run: |
        pip install -r requirements.txt
        pip install pyinstaller
    
    - name: Build EXE
      run: python build_exe.py
    
    - name: Upload artifact
      uses: actions/upload-artifact@v3
      with:
        name: XhellDemo
        path: dist/XhellDemo.exe
```

提交代码后，GitHub会自动构建exe文件，你可以从Actions页面下载。

## 📞 需要帮助？

查看完整文档：`BUILD_EXE_README.md`

## ✨ 快速检查清单

打包前：
- [ ] Python已安装（3.8+）
- [ ] 在Windows系统上（或准备好虚拟机）
- [ ] 项目文件完整

打包后：
- [ ] exe文件已生成（dist/XhellDemo.exe）
- [ ] 测试运行正常
- [ ] 浏览器能自动打开

演示前：
- [ ] 已将exe复制到U盘
- [ ] 提前在演示电脑上测试过
- [ ] 准备了备份方案

---

**祝你演示成功！** 🎉

如果有任何问题，欢迎查看详细文档或寻求帮助。
