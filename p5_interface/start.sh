#!/bin/bash

# Xhell P5 Interface - One-Click Start Script
# For WSL / Linux environments

echo "=================================================="
echo "   XHELL P5 INTERFACE // PHANTOM THIEVES EDITION  "
echo "=================================================="

# 1. Determine script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR"

# 2. Check for Python
if ! command -v python3 &> /dev/null; then
    echo "[ERROR] Python3 could not be found. Please install python3."
    exit 1
fi

# 3. Setup Virtual Environment (Lightweight)
VENV_DIR=".venv"
if [ ! -d "$VENV_DIR" ]; then
    echo "[INFO] Creating virtual environment (.venv)..."
    python3 -m venv $VENV_DIR
fi

# 4. Install Dependencies using the venv's pip
PYTHON_BIN="$VENV_DIR/bin/python3"
PIP_BIN="$VENV_DIR/bin/pip"

echo "[INFO] Checking dependencies..."
$PIP_BIN install -r requirements.txt

if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to install dependencies."
    exit 1
fi

# 5. Check Xhell Binary
if [ ! -f "./xhell/xhell" ]; then
    echo "[WARNING] Xhell binary not found in ./xhell/xhell"
    echo "[INFO] Attempting to compile..."
    if [ -d "./xhell" ]; then
        cd xhell
        make
        cd ..
    fi
fi

# Ensure executable permissions
if [ -f "./xhell/xhell" ]; then
    chmod +x ./xhell/xhell
fi

# 6. Run the App
echo "=================================================="
# Kill any existing process on port 8505
echo "[INFO] Ensuring port 8505 is free..."
fuser -k 8505/tcp > /dev/null 2>&1 || true
# Wait a moment for the process to actually die
sleep 1

echo "[SUCCESS] System Initialized."
echo "[INFO] Starting Server on http://localhost:8505"
echo "Press Ctrl+C to stop."
echo "=================================================="

$PYTHON_BIN app.py
