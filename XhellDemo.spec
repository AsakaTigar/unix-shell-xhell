# -*- mode: python ; coding: utf-8 -*-
"""
Xhell Demo PyInstaller配置文件
可以直接使用: pyinstaller XhellDemo.spec
"""

import sys
from pathlib import Path

# 当前目录
current_dir = Path.cwd()

# 数据文件
datas = [
    (str(current_dir / 'streamlit_demo'), 'streamlit_demo'),
]

# 如果xhell目录存在，也添加进去
if (current_dir / 'xhell').exists():
    datas.append((str(current_dir / 'xhell'), 'xhell'))

# 隐藏导入
hiddenimports = [
    'streamlit',
    'streamlit.web.cli',
    'streamlit.runtime',
    'streamlit.runtime.scriptrunner',
    'streamlit.runtime.scriptrunner.script_runner',
    'streamlit.components.v1',
    'pandas',
    'numpy',
    'Pygments',
    'click',
    'tornado',
    'altair',
    'pyarrow',
    'watchdog',
    'validators',
    'packaging',
    'toml',
    'blinker',
    'pympler',
    'pillow',
]

# 收集所有相关包
collect_all = [
    'streamlit',
    'altair',
    'pyarrow',
]

block_cipher = None

a = Analysis(
    ['launcher.py'],
    pathex=[],
    binaries=[],
    datas=datas,
    hiddenimports=hiddenimports,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[
        # 排除不需要的大型库以减小体积
        'matplotlib',
        'scipy',
        'IPython',
        'notebook',
        'pytest',
    ],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

# 收集所有指定的包
for package in collect_all:
    a.datas += collect_data_files(package)
    a.binaries += collect_dynamic_libs(package)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name='XhellDemo',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,  # 使用UPX压缩（如果可用）
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,  # 显示控制台窗口，方便调试。如果不需要可改为False
    disable_windowed_traceback=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=None,  # 如果有图标文件，可以在这里指定，例如 'icon.ico'
)

# 如果要打包成文件夹而不是单个exe，使用COLLECT代替上面的EXE
# coll = COLLECT(
#     exe,
#     a.binaries,
#     a.zipfiles,
#     a.datas,
#     strip=False,
#     upx=True,
#     upx_exclude=[],
#     name='XhellDemo',
# )
