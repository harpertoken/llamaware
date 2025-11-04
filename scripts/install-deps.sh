#!/bin/bash
# Llamaware Agent - Dependency Installer

set -e

echo "==> Installing Llamaware Agent dependencies..."

detect_os() {
  case "$OSTYPE" in
    linux-gnu*)   echo "linux" ;;
    darwin*)      echo "macos" ;;
    *)            echo "unknown" ;;
  esac
}

install_linux_deps() {
  echo "--> Linux detected. Installing packages via apt..."
  sudo apt update
  sudo apt install -y nlohmann-json3-dev cmake build-essential libcurl4-openssl-dev git

  if ! pkg-config --exists cpr; then
    echo "--> Building CPR from source..."
    TMP_DIR=$(mktemp -d)
    git clone --depth 1 https://github.com/libcpr/cpr.git "$TMP_DIR/cpr"
    cmake -S "$TMP_DIR/cpr" -B "$TMP_DIR/cpr/build" -DCPR_USE_SYSTEM_CURL=ON -DCMAKE_BUILD_TYPE=Release
    cmake --build "$TMP_DIR/cpr/build" --parallel
    sudo cmake --install "$TMP_DIR/cpr/build"
    rm -rf "$TMP_DIR"
  fi
}

install_macos_deps() {
  echo "--> macOS detected. Installing packages via Homebrew..."
  if ! command -v brew &>/dev/null; then
    echo "Error: Homebrew not found. Please install it first:" >&2
    echo "  /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"" >&2
    exit 1
  fi
  brew install cpr nlohmann-json cmake
}

show_manual_instructions() {
  cat <<EOF
Unsupported OS: $OSTYPE

Please install manually:
  - CMake 3.15+
  - nlohmann/json
  - CPR (C++ Requests)
  - libcurl development headers
EOF
  exit 1
}

case "$(detect_os)" in
  linux) install_linux_deps ;;
  macos) install_macos_deps ;;
  *) show_manual_instructions ;;
esac

cat <<EOF

✅ Dependencies installed successfully.

Next steps:
  1. Configure your API keys in .env
  2. Run: cmake -S . -B build
  3. Run: cmake --build build
  4. Run: ./build/bin/llamaware-agent
EOF
