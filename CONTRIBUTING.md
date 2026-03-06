# Contributing to OSRE

First off, thank you for considering contributing to OSRE (Open Source Render Engine)! It's people like you that make OSRE such a great tool.

## How to contribute

1. **Clone the repo**: Fork the repository and clone it to your local machine.
2. **Create a branch**: Create a new branch from `master` for your fix or improvement.
3. **Make your changes**: Implement your fix or feature.
4. **Push your changes**: Push the branch to your cloned repository.
5. **Create a pull-request**: Open a pull-request to the `master` branch of the main OSRE repository.
6. **Code Review**: Wait for the code review. Address any findings if necessary.
7. **Done**: Once approved, your changes will be merged!

## Build Instructions

### Prerequisites (Linux)
```bash
sudo apt-get install libsdl2-dev libglm-dev libglew-dev libgtest-dev googletest
```

### Configure and Build
```bash
# Configure with CMake (creates build directory)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build with Ninja (or use --build . with default generator)
cmake --build build --config Release -j 4
```

### Build Options
- `-DOSRE_BUILD_PLAYER=ON` - Build the player (default: ON)
- `-DOSRE_BUILD_SAMPLES=ON` - Build samples (default: ON)
- `-DOSRE_BUILD_TESTS=ON` - Build tests (default: ON)
- `-DOSRE_BUILD_DOC=ON` - Build Doxygen documentation (default: OFF)
- `-DOSRE_BUILD_ED=ON` - Build OSRE-Ed editor (default: ON)

## Running Tests

We use Google Test for unit testing. You can run tests using `ctest` or by executing the test binaries directly.

```bash
# Run all tests
cd build && ctest -C Release

# Run all tests with output on failure
cd build && ctest -C Release --output-on-failure

# Run single test (via ctest filter)
cd build && ctest -C Release -R ObjectTest

# Run unit test executable directly
./bin/osre_unittest
```

## Code Style

To keep the codebase clean and consistent, please follow these guidelines:

### Formatting
- **Tool**: `clang-format` with LLVM-based configuration (`.clang-format`).
- **Indent**: 4 spaces, no tabs.
- **Braces**: Attach (K&R style).
- **C++ standard**: C++17 (enforced in CMake).

### Naming Conventions
- **Classes**: `PascalCase` (e.g., `Object`, `Logger`)
- **Functions**: `PascalCase` (e.g., `getName()`, `setGuid()`)
- **Member variables**: `m` prefix followed by PascalCase (e.g., `mObjectName`, `mId`)
- **Constants**: `c8`, `c16` prefixes for char constants; otherwise `PascalCase`.
- **Namespaces**: `OSRE::Module` (e.g., `OSRE::Common`, `OSRE::App`)
- **Macros**: `ALL_CAPS` with underscores (e.g., `DECL_OSRE_LOG_MODULE`)

### File Organization
- **Headers**: `.h` extension with `#pragma once` include guard.
- **Implementation**: `.cpp` extension.
- **Include order**:
  1. Corresponding header (if .cpp)
  2. Project headers (e.g., `"Common/Object.h"`)
  3. `cppcore` library headers (e.g., `<cppcore/Container/TArray.h>`)
  4. System/STL headers

### License Header
Every source file must include the MIT license header:
```cpp
/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
```

## Project Structure

- `src/Engine/`: Core engine source code.
  - `App/`: Application layer (Scene, Entity, Components).
  - `Common/`: Base classes and utilities.
  - `RenderBackend/`: Graphics backend (OpenGL).
  - `UI/`: User interface components.
- `test/`: Unit and render tests.
- `samples/`: Example applications.

## Git Conventions
- Always branch from `master`.
- Pull requests should be targeted at `master`.
- Commit messages should be descriptive and clear.

Thanks a lot for your help!
