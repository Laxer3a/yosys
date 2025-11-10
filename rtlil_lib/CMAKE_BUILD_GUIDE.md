# CMake Build Guide for RTLIL Library

This guide shows how to build the RTLIL library using CMake on different platforms.

## Table of Contents
- [Windows - Visual Studio](#windows---visual-studio)
- [Linux/Unix - Make](#linuxunix---make)
- [Build Targets](#build-targets)
- [CMake Options](#cmake-options)

---

## Windows - Visual Studio

### Prerequisites
- CMake 3.15 or higher ([Download](https://cmake.org/download/))
- Visual Studio 2017 or higher (Community/Professional/Enterprise)
- Windows SDK (included with Visual Studio)

### Building with Visual Studio GUI

1. **Generate Visual Studio Solution:**
   ```cmd
   cd rtlil_lib
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" -A x64
   ```

   For other Visual Studio versions:
   - Visual Studio 2022: `cmake .. -G "Visual Studio 17 2022" -A x64`
   - Visual Studio 2019: `cmake .. -G "Visual Studio 16 2019" -A x64`
   - Visual Studio 2017: `cmake .. -G "Visual Studio 15 2017" -A x64`

2. **Open Solution in Visual Studio:**
   ```cmd
   start RTLILLibrary.sln
   ```

3. **Build in Visual Studio:**
   - Select build configuration: `Debug` or `Release`
   - Build → Build Solution (F7)
   - Or right-click on specific project and select Build

4. **Run Tests:**
   - Set `simple_example` or `complex_example` as startup project
   - Press F5 to run
   - Or run from command line: `bin\Release\simple_example.exe`

### Building from Command Line (Windows)

1. **Generate and Build (Release):**
   ```cmd
   cd rtlil_lib
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" -A x64
   cmake --build . --config Release
   ```

2. **Generate and Build (Debug):**
   ```cmd
   cmake --build . --config Debug
   ```

3. **Run Tests:**
   ```cmd
   bin\Release\simple_example.exe
   bin\Release\complex_example.exe
   ```

### Output Locations (Windows)

After building, you'll find:
- **Library:** `build/lib/Release/rtlil.lib` (or `Debug/rtlil.lib`)
- **Executables:** `build/bin/Release/simple_example.exe` and `complex_example.exe`
- **JSON output:** `build/bin/Release/complex_example.json`

---

## Linux/Unix - Make

### Prerequisites
- CMake 3.15 or higher
- GCC 7+ or Clang 5+
- Make

### Building

1. **Generate Makefiles:**
   ```bash
   cd rtlil_lib
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

2. **Build:**
   ```bash
   make -j$(nproc)  # Parallel build
   ```

3. **Run Tests:**
   ```bash
   ./bin/simple_example
   ./bin/complex_example
   ```

### Build Configurations

**Release build (optimized):**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

**Debug build (with symbols):**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### Output Locations (Linux)

After building, you'll find:
- **Library:** `build/lib/librtlil.a`
- **Executables:** `build/bin/simple_example` and `build/bin/complex_example`
- **JSON output:** `build/bin/complex_example.json`

---

## Build Targets

The CMake build system provides several targets:

### Library Target
- **`rtlil`** - Static library containing all RTLIL functionality
  ```bash
  cmake --build . --target rtlil
  ```

### Executable Targets
- **`simple_example`** - Basic RTLIL usage example
  ```bash
  cmake --build . --target simple_example
  ```

- **`complex_example`** - Advanced example with registers and muxes
  ```bash
  cmake --build . --target complex_example
  ```

### Test Targets
- **`run_simple_test`** - Build and run simple example
  ```bash
  cmake --build . --target run_simple_test
  ```

- **`run_complex_test`** - Build and run complex example
  ```bash
  cmake --build . --target run_complex_test
  ```

- **`run_all_tests`** - Build and run all tests
  ```bash
  cmake --build . --target run_all_tests
  ```

### Utility Targets
- **`ALL_BUILD`** (Windows) / **`all`** (Unix) - Build everything
- **`CLEAN`** (Windows) / **`clean`** (Unix) - Remove build artifacts
  ```bash
  cmake --build . --target clean
  ```

---

## CMake Options

### Generator Selection

**Windows:**
- Visual Studio 2022: `-G "Visual Studio 17 2022" -A x64`
- Visual Studio 2019: `-G "Visual Studio 16 2019" -A x64`
- Visual Studio 2017: `-G "Visual Studio 15 2017" -A x64`
- Ninja: `-G Ninja` (requires Ninja installed)

**Linux/Unix:**
- Unix Makefiles (default): `-G "Unix Makefiles"`
- Ninja: `-G Ninja`

### Build Type (Unix Makefiles only)
```bash
-DCMAKE_BUILD_TYPE=Release    # Optimized
-DCMAKE_BUILD_TYPE=Debug      # With debug symbols
-DCMAKE_BUILD_TYPE=RelWithDebInfo  # Optimized + debug symbols
-DCMAKE_BUILD_TYPE=MinSizeRel # Optimized for size
```

### Installation

**Install to custom location:**
```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build . --target install
```

**Default install locations:**
- Library: `<prefix>/lib/librtlil.a` (or `.lib` on Windows)
- Headers: `<prefix>/include/rtlil/*.h`
- Executables: `<prefix>/bin/`

---

## Project Structure in Visual Studio

When you open the solution in Visual Studio, you'll see:

```
Solution 'RTLILLibrary'
├── rtlil (static library)
│   ├── Header Files
│   │   ├── Kernel (rtlil.h, log.h, etc.)
│   │   ├── BigInt (BigInteger.hh, etc.)
│   │   └── SHA1 (sha1.h)
│   └── Source Files
│       ├── Kernel (rtlil.cc, log.cc, etc.)
│       ├── BigInt (BigInteger.cc, etc.)
│       └── SHA1 (sha1.cpp)
├── simple_example (executable)
│   └── test/simple_example.cc
└── complex_example (executable)
    └── test/complex_example.cc
```

---

## Troubleshooting

### Windows Issues

**Problem:** CMake can't find Visual Studio
```cmd
# List available generators
cmake --help
# Use specific generator
cmake .. -G "Visual Studio 16 2019" -A x64
```

**Problem:** Build fails with C2220 (warning as error)
- Open project properties in Visual Studio
- C/C++ → General → Treat Warnings As Errors → No

**Problem:** LNK errors (undefined symbols)
- Clean and rebuild: Build → Clean Solution, then Build → Rebuild Solution

### Linux Issues

**Problem:** CMake version too old
```bash
# Check version
cmake --version
# Upgrade on Ubuntu/Debian
sudo apt update
sudo apt install cmake
```

**Problem:** Compiler not found
```bash
# Install GCC
sudo apt install build-essential

# Or use specific compiler
cmake .. -DCMAKE_CXX_COMPILER=g++-9
```

---

## Quick Reference

### Windows (Visual Studio)
```cmd
cd rtlil_lib
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
bin\Release\complex_example.exe
```

### Linux (Make)
```bash
cd rtlil_lib
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./bin/complex_example
```

### Clean Build
```bash
# Remove build directory and start over
rm -rf build
mkdir build && cd build
cmake .. [options]
cmake --build .
```

---

## Using the Library in Your Project

### CMake Integration

Add to your project's `CMakeLists.txt`:

```cmake
# Add RTLIL library subdirectory
add_subdirectory(path/to/rtlil_lib)

# Link against RTLIL
add_executable(my_program main.cpp)
target_link_libraries(my_program PRIVATE rtlil)
```

### Visual Studio Project

1. Add `rtlil.lib` to Additional Dependencies:
   - Project Properties → Linker → Input → Additional Dependencies
   - Add: `path/to/rtlil_lib/build/lib/Release/rtlil.lib`

2. Add include directories:
   - Project Properties → C/C++ → General → Additional Include Directories
   - Add: `path/to/rtlil_lib/include` and `path/to/rtlil_lib/libs`

3. Add preprocessor definitions:
   - Project Properties → C/C++ → Preprocessor → Preprocessor Definitions
   - Add: `_YOSYS_;YOSYS_NAMESPACE_BEGIN=namespace Yosys {;YOSYS_NAMESPACE_END=};YOSYS_NAMESPACE_PREFIX=Yosys::`

---

## Support

For issues or questions:
- Check the main `README.md` for library usage
- See `API_DOCUMENTATION.md` for API reference
- Review `STUBS_AND_MODIFICATIONS.md` for limitations
