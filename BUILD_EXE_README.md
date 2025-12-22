# Xhell项目打包成exe指南

本文档说明如何将Xhell演示项目打包成Windows可执行文件(.exe)，方便在课堂演示时使用。

## 📋 方案特点

✅ **一键启动** - 双击exe文件即可自动启动  
✅ **自动打开浏览器** - 无需手动输入URL  
✅ **自包含** - 所有依赖（包括streamlit）都打包在内  
✅ **免安装** - 无需在演示电脑上安装Python或任何依赖  
✅ **端口自动检测** - 自动查找可用端口，避免冲突  

## 🛠️ 打包步骤

### 方法一：在Windows系统上打包（推荐）

#### 1. 准备环境

```bash
# 安装PyInstaller和项目依赖
pip install pyinstaller
pip install -r requirements.txt
```

#### 2. 运行打包脚本

```bash
python build_exe.py
```

#### 3. 获取可执行文件

打包完成后，在 `dist` 目录下会生成 `XhellDemo.exe`

```
dist/
└── XhellDemo.exe  <-- 这就是你需要的文件！
```

#### 4. 测���

双击 `XhellDemo.exe`，应该会：
- 自动启动Streamlit服务
- 自动打开浏览器
- 显示Xhell演示界面

### 方法二：在Linux上交叉编译（高级）

如果你需要在Linux上打包Windows exe，可以使用Wine环境：

```bash
# 安装Wine和相关工具（Ubuntu/Debian）
sudo apt-get install wine wine64 wine32

# 安装Windows版本的Python（在Wine环境中）
# 然后在Wine环境中运行打包脚本
```

⚠️ **注意**：这个方法比较复杂，建议直接在Windows系统上打包。

### 方法三：使用虚拟机或远程Windows系统

1. 将整个项目文件夹复制到Windows系统
2. 在Windows上按照"方法一"执行打包
3. 将生成的exe文件复制回来

## 📦 自定义打包配置

如果需要自定义��包配置，可以编辑 `build_exe.py` 或使用生成的 `XhellDemo.spec` 文件：

### 显示控制台窗口

如果想在运行时看到详细的日志信息（调试用），修改 `build_exe.py`：

```python
# 注释掉或删除这行
'--windowed',  # 不显示控制台窗口
```

### 添加程序图标

1. 准备一个 `.ico` 格式的图标文件（例如 `icon.ico`）
2. 在 `build_exe.py` 中取消注释：

```python
'--icon=icon.ico',
```

### 调整打包内容

在 `build_exe.py` 中可以添加更多文件：

```python
# 添加其他数据文件
f'--add-data={current_dir / "docs"};docs',
f'--add-data={current_dir / "README.md"};.',
```

## 📝 使用说明

### 在课堂演示时

1. **提前准备**：
   - 将 `XhellDemo.exe` 复制到U盘或演示电脑
   - 提前测试一次，确保能正常运行

2. **开始演示**：
   - 双击 `XhellDemo.exe`
   - 等待3-5秒，浏览器会自动打开
   - 开始你的演示！

3. **结束演示**：
   - 关闭浏览器窗口
   - 如果看到控制台窗口，按 `Ctrl+C` 停止服务
   - 或者直接关闭控制台窗口

### 常见问题

#### Q1: exe文件很大（几百MB）？

A: 这是正常的，因为打包了整个Python运行时和所有依赖库。如果想减小体积：
- 使用 `--onefile` 打包成单文件（已默认）
- 或者使用 `--onedir` 打包成文件夹（体积不变但启动更快）

#### Q2: 杀毒软件报警？

A: PyInstaller打包的exe可能被某些杀毒软件误报。解决方法：
- 添加到杀毒软件白名单
- 或者使用 `--noupx` 参数打包

#### Q3: 启动很慢？

A: 首次启动会慢一些（10-20秒），因为要解压文件。解决方法：
- 使用 `--onedir` 模式（不使用 `--onefile`）
- 提前启动，不要在演示时才第一次运行

#### Q4: 端口被占用？

A: launcher.py 已经实现了自动端口检测，会从8501开始查找可用端口。

#### Q5: 在演示电脑上无法运行？

A: 检查：
- Windows版本是否兼容（建议Windows 10或更高）
- 是否被防火墙拦截
- 是否有足够的磁盘空间（临时文件需要约1GB）

## 🔧 高级配置

### 使用spec文件

运行一次 `build_exe.py` 后会生成 `XhellDemo.spec` 文件，之后可以直接使用：

```bash
pyinstaller XhellDemo.spec
```

### 性能优化

在 `XhellDemo.spec` 中可以添加：

```python
# 优化选项
upx=True,  # 使用UPX压缩（需要安装UPX）
strip=False,  # 不去除符号表
```

### 添加版本信息

创建 `version_info.txt`：

```
VSVersionInfo(
  ffi=FixedFileInfo(
    filevers=(1, 0, 0, 0),
    prodvers=(1, 0, 0, 0),
    mask=0x3f,
    flags=0x0,
    OS=0x40004,
    fileType=0x1,
    subtype=0x0,
    date=(0, 0)
  ),
  kids=[
    StringFileInfo(
      [
      StringTable(
        u'040904B0',
        [StringStruct(u'CompanyName', u'GDUT'),
        StringStruct(u'FileDescription', u'Xhell Demo'),
        StringStruct(u'FileVersion', u'1.0.0.0'),
        StringStruct(u'ProductName', u'Xhell Shell Interpreter'),
        StringStruct(u'ProductVersion', u'1.0.0.0')])
      ]
    ),
    VarFileInfo([VarStruct(u'Translation', [1033, 1200])])
  ]
)
```

然后在 `build_exe.py` 中添加：

```python
'--version-file=version_info.txt',
```

## 📊 文件大小预估

- **单文件exe**: 约 80-150MB
- **文件夹模式**: 约 100-200MB（但启动更快）

## 🚀 快速开始（TL;DR）

如果你赶时间，只需要运行：

```bash
# Windows系统
pip install pyinstaller
python build_exe.py

# 完成后，dist/XhellDemo.exe 就是你要的文件
```

## 📚 相关资源

- [PyInstaller官方文档](https://pyinstaller.org/)
- [Streamlit文档](https://docs.streamlit.io/)

## 💡 提示

1. **提前测试**：在演示前一天测试exe文件
2. **备份方案**：准备一个在线版本作为备份
3. **录屏备份**：提前录制演示视频作为备份
4. **网络独立**：打包后的exe不需要网络连接即可运行

---

如有问题，欢迎查看项目文档或联系开发者。

祝演示顺利！🎉
