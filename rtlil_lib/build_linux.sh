#!/bin/bash
# Linux/Unix build script for RTLIL Library using CMake

set -e  # Exit on error

echo "================================================================"
echo "RTLIL Library - Linux/Unix Build Script"
echo "================================================================"
echo ""

# Check if CMake is available
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found in PATH"
    echo "Please install CMake:"
    echo "  Ubuntu/Debian: sudo apt install cmake"
    echo "  Fedora/RHEL:   sudo dnf install cmake"
    echo "  Arch:          sudo pacman -S cmake"
    echo "  macOS:         brew install cmake"
    exit 1
fi

# Check CMake version
echo "Checking CMake version..."
cmake --version
echo ""

# Check for compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "ERROR: No C++ compiler found (g++ or clang++)"
    echo "Please install a C++ compiler:"
    echo "  Ubuntu/Debian: sudo apt install build-essential"
    echo "  Fedora/RHEL:   sudo dnf groupinstall 'Development Tools'"
    echo "  Arch:          sudo pacman -S base-devel"
    echo "  macOS:         xcode-select --install"
    exit 1
fi

# Determine number of CPU cores for parallel build
if command -v nproc &> /dev/null; then
    NCORES=$(nproc)
elif [ -f /proc/cpuinfo ]; then
    NCORES=$(grep -c ^processor /proc/cpuinfo)
else
    NCORES=4
fi

echo "Using $NCORES parallel jobs for build"
echo ""

# Choose build type
BUILD_TYPE="${1:-Release}"
if [ "$BUILD_TYPE" != "Release" ] && [ "$BUILD_TYPE" != "Debug" ] && [ "$BUILD_TYPE" != "RelWithDebInfo" ]; then
    echo "Invalid build type: $BUILD_TYPE"
    echo "Valid options: Release, Debug, RelWithDebInfo"
    echo "Usage: $0 [Release|Debug|RelWithDebInfo]"
    exit 1
fi

echo "Build type: $BUILD_TYPE"
echo ""

# Create build directory
mkdir -p build
cd build

echo "================================================================"
echo "Generating Build Files..."
echo "================================================================"
echo ""

# Generate Makefiles
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

echo ""
echo "================================================================"
echo "Building RTLIL Library..."
echo "================================================================"
echo ""

# Build
cmake --build . -j$NCORES

echo ""
echo "================================================================"
echo "Build completed successfully!"
echo "================================================================"
echo ""
echo "Output files:"
echo "  Library:  build/lib/librtlil.a"
echo "  Examples: build/bin/simple_example"
echo "            build/bin/complex_example"
echo ""
echo "To run the examples:"
echo "  ./bin/simple_example"
echo "  ./bin/complex_example"
echo ""

# Ask if user wants to run tests
read -p "Do you want to run the complex example now? (y/N): " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo ""
    echo "Running complex_example..."
    echo "================================================================"
    ./bin/complex_example
    echo ""
    if [ -f complex_example.json ]; then
        echo "JSON output saved to: build/complex_example.json"
        echo "Size: $(du -h complex_example.json | cut -f1)"
    fi
fi

echo ""
echo "Build script completed!"
