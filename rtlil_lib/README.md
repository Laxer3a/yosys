# RTLIL Standalone Library

This is an extracted standalone version of Yosys RTLIL (Register Transfer Level Intermediate Language) library.

RTLIL is a powerful intermediate representation for digital circuits, providing a complete API for creating, manipulating, and exporting hardware netlists programmatically.

## Features

- **Complete RTLIL Implementation** - Full extraction from Yosys with 99.9% completeness
- **BigInt Support** - Arbitrary precision arithmetic for large constants
- **SHA1 Library** - Design fingerprinting and hashing
- **Cross-Platform** - Builds on Linux, macOS, and Windows (Visual Studio)
- **JSON Export** - Export designs to JSON format
- **Rich API** - Create modules, wires, cells, and connections programmatically
- **All Cell Types** - Support for combinational, sequential, and arithmetic cells

## Structure

```
rtlil_lib/
├── include/          # Header files (18 files)
│   ├── rtlil.h       # Core RTLIL definitions
│   ├── log.h         # Logging system
│   ├── yosys.h       # Main include file
│   └── ...
├── src/              # Implementation files (7 files)
│   ├── rtlil.cc      # Core RTLIL implementation
│   ├── fmt.cc        # Formatting utilities
│   └── ...
├── libs/
│   ├── bigint/       # BigInt library (5 files)
│   └── sha1/         # SHA1 implementation
├── test/             # Example programs
│   ├── simple_example.cc    # Basic usage
│   └── complex_example.cc   # Advanced example (counter with muxes)
├── Makefile          # Traditional Make build
├── CMakeLists.txt    # CMake build (cross-platform)
└── Documentation
    ├── API_DOCUMENTATION.md          # Complete API reference
    ├── CMAKE_BUILD_GUIDE.md          # CMake build instructions
    ├── STUBS_AND_MODIFICATIONS.md    # Completeness audit
    └── RTLIL_EXTRACTION_SUMMARY.md   # Extraction details
```

## Quick Start

### Linux/Unix/macOS

**Option 1: Using Make**
```bash
make
./test/simple_example
```

**Option 2: Using CMake**
```bash
./build_linux.sh
# Or manually:
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
./bin/simple_example
```

### Windows (Visual Studio)

**Option 1: Automated build script**
```cmd
build_windows.bat
```

**Option 2: Manual CMake**
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
bin\Release\simple_example.exe
```

See **[CMAKE_BUILD_GUIDE.md](CMAKE_BUILD_GUIDE.md)** for detailed build instructions.

## Usage Example

```cpp
#include "yosys.h"

using namespace Yosys;

int main() {
    // Initialize library
    rtlil_init();

    // Create design
    RTLIL::Design *design = new RTLIL::Design;

    // Create module
    RTLIL::Module *module = design->addModule(ID(top));

    // Add wires
    RTLIL::Wire *a = module->addWire(ID(a), 1);
    a->port_input = true;

    RTLIL::Wire *y = module->addWire(ID(y), 8);
    y->port_output = true;

    // Create adder cell
    RTLIL::Cell *add = module->addCell(ID(add1), ID($add));
    add->setPort(ID::A, RTLIL::SigSpec(a));
    add->setPort(ID::B, RTLIL::Const(42, 8));
    add->setPort(ID::Y, RTLIL::SigSpec(y));
    add->setParam(ID::A_WIDTH, 1);
    add->setParam(ID::B_WIDTH, 8);
    add->setParam(ID::Y_WIDTH, 8);

    // Export design
    module->check();
    design->check();

    delete design;
    return 0;
}
```

See **[API_DOCUMENTATION.md](API_DOCUMENTATION.md)** for complete API reference and more examples.

## Documentation

| File | Description |
|------|-------------|
| [API_DOCUMENTATION.md](API_DOCUMENTATION.md) | Complete API reference with examples |
| [CMAKE_BUILD_GUIDE.md](CMAKE_BUILD_GUIDE.md) | Detailed build instructions for all platforms |
| [STUBS_AND_MODIFICATIONS.md](STUBS_AND_MODIFICATIONS.md) | Audit of all modifications (99.9% complete) |
| [RTLIL_EXTRACTION_SUMMARY.md](RTLIL_EXTRACTION_SUMMARY.md) | Technical details of extraction |

## Supported Cell Types

The library supports all standard RTLIL cell types:

- **Combinational Logic**: `$and`, `$or`, `$xor`, `$not`, `$mux`, `$pmux`
- **Arithmetic**: `$add`, `$sub`, `$mul`, `$div`, `$mod`, `$pow`
- **Comparison**: `$eq`, `$ne`, `$lt`, `$le`, `$gt`, `$ge`
- **Sequential**: `$dff`, `$dffe`, `$adff`, `$sdff`, `$dlatch`
- **Reduction**: `$reduce_and`, `$reduce_or`, `$reduce_xor`
- **Shift**: `$shl`, `$shr`, `$sshl`, `$sshr`, `$shift`, `$shiftx`
- **Logical**: `$logic_and`, `$logic_or`, `$logic_not`
- And many more...

## Build Outputs

After building, you will find:

**Using Make:**
- Library: `librtlil.a`
- Examples: `test/simple_example`, `test/complex_example`

**Using CMake:**
- Library: `build/lib/librtlil.a` (or `.lib` on Windows)
- Examples: `build/bin/simple_example`, `build/bin/complex_example`
- JSON output: `build/bin/complex_example.json`

## Integration

To use this library in your own project:

**CMake:**
```cmake
add_subdirectory(path/to/rtlil_lib)
target_link_libraries(your_target PRIVATE rtlil)
```

**Manual:**
- Include: `-I path/to/rtlil_lib/include -I path/to/rtlil_lib/libs`
- Link: `-L path/to/rtlil_lib -lrtlil`
- Defines: `-D_YOSYS_ -DYOSYS_NAMESPACE_BEGIN='namespace Yosys {' -DYOSYS_NAMESPACE_END='}' -DYOSYS_NAMESPACE_PREFIX='Yosys::'`

## Completeness

This library is **99.9% complete** compared to the original Yosys RTLIL implementation:

- ✅ Core RTLIL: 100% (all data structures and APIs)
- ✅ BigInt: 100% (full arbitrary precision arithmetic)
- ✅ SHA1: 100% (complete implementation)
- ✅ Formatting: 100% (all utilities)
- ✅ Cell Types: 100% (all standard cells)
- ⚠️ String Parsing: 99% (workaround available for `SigSpec::parse`)

See [STUBS_AND_MODIFICATIONS.md](STUBS_AND_MODIFICATIONS.md) for detailed analysis.

## Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+ (for CMake builds)
- Make (for Makefile builds on Unix)

## License

ISC License (same as Yosys) - see individual files for copyright notices.

## Source

Extracted from [Yosys Open SYnthesis Suite](https://github.com/YosysHQ/yosys) with minimal modifications to create a standalone library suitable for integration with custom HDL parsers and tools.
