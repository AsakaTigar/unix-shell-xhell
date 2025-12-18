#!/bin/bash

# Xhell Demo Start Script
# Usage: ./start_demo.sh

echo "========================================"
echo "    Starting Xhell Interactive Demo"
echo "========================================"

# 1. Check for Python
if command -v python3 &> /dev/null; then
    PYTHON_CMD=python3
elif command -v python &> /dev/null; then
    PYTHON_CMD=python
else
    echo "Error: Python is not installed."
    echo "Please install Python 3.8+ first."
    exit 1
fi

# 2. Check for Streamlit
if ! $PYTHON_CMD -c "import streamlit" &> /dev/null; then
    echo "Installing dependencies..."
    $PYTHON_CMD -m pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple
fi

# 3. Create workspace if missing
if [ ! -d "demo_workspace" ]; then
    echo "Creating demo workspace..."
    mkdir -p demo_workspace
fi

# 4. Check if xhell is compiled
if [ ! -f "xhell/xhell" ]; then
    echo "Compiling Xhell..."
    cd xhell && make && cd ..
fi

# 5. Start the demo
echo "Starting Streamlit Interface..."
echo "Please open the URL shown below in your browser."
echo "========================================"

# Run with python -m to avoid path issues
$PYTHON_CMD -m streamlit run streamlit_demo/app.py --server.address=0.0.0.0 --server.enableCORS=false --server.enableXsrfProtection=false
