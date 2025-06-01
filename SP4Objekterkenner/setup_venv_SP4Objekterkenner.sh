#!/bin/bash

set -e

# Configuration
VENV_NAME="venv_SP4Objekterkenner"
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
VENV_PATH="${SCRIPT_DIR}/${VENV_NAME}"

echo "===== SP4Objekterkenner Environment Setup ====="
echo "Setting up environment at: ${VENV_PATH}"

# Check if Python 3 is available
if ! command -v python3.12 &> /dev/null; then
    echo "Error: Python 3 is required but not found!"
    exit 1
fi

# Check if pip is available
if ! command -v pip3 &> /dev/null; then
    echo "Error: pip3 is required but not found!"
    exit 1
fi

# Check if virtualenv is installed
if ! command -v virtualenv &> /dev/null; then
    echo "virtualenv not found, installing..."
    pip3 install virtualenv
fi

# Create virtual environment if it doesn't exist
if [ ! -d "$VENV_PATH" ]; then
    echo "Creating virtual environment at ${VENV_PATH}..."
    virtualenv -p python3.12 "$VENV_PATH"
else
    echo "Virtual environment already exists at ${VENV_PATH}"
fi

# Activate virtual environment
echo "Activating virtual environment..."
source "${VENV_PATH}/bin/activate"

# Install required packages (some are probably not needed ¯\_(ツ)_/¯)
echo "Installing required packages..."
pip install --upgrade pip
pip install setuptools wheel
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu126
pip install ultralytics opencv-python-headless numpy pillow matplotlib huggingface_hub
pip install -q git+https://github.com/sunsmarterjie/yolov12.git supervision flash-attn pycocotools

# Print confirmation
echo "===== Installation Complete ====="
echo "SP4Objekterkenner environment successfully set up at: ${VENV_PATH}"
echo "To activate, run: source ${VENV_PATH}/bin/activate"

# Deactivate virtual environment
deactivate
