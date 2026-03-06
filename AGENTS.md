# AGENTS.md - OSRE Agent Guidelines

## Project Overview

OSRE (Open Source Render Engine) is an experimental C++ render engine using CMake, C++17, OpenGL, and SDL2. It includes an editor (OSRE-Ed) and various samples.

## Build Commands

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

### Running Tests
```bash
# Run all tests
cd build && ctest -C Release

# Run all tests with output
cd build && ctest -C Release --output-on-failure

# Run single test (via ctest filter)
cd build && ctest -C Release -R ObjectTest

# Run unit test executable directly
./bin/osre_unittest
```

### Build Options
- `-DOSRE_BUILD_PLAYER=ON` - Build the player (default: ON)
- `-DOSRE_BUILD_SAMPLES=ON` - Build samples (default: ON)
- `-DOSRE_BUILD_TESTS=ON` - Build tests (default: ON)
- `-DOSRE_BUILD_DOC=ON` - Build Doxygen documentation (default: OFF)
- `-DOSRE_BUILD_ED=ON` - Build OSRE-Ed editor (default: ON)

## Code Style

### Formatting
- **Tool**: clang-format with LLVM-based configuration (`.clang-format`)
- **Indent**: 4 spaces, no tabs
- **Column limit**: 0 (unlimited)
- **Braces**: Attach (K&R style)
- **C++ standard**: C++11 (enforced in clang-format), C++17 (in CMake)

### Static Analysis
- **Tool**: clang-tidy (configuration in `.clang-tidy`)
- Key checks enabled: bugprone-*, modernize-*, performance-*, readability-*
- Run on changed files: `clang-tidy src/File.cpp`

### Naming Conventions
- **Classes**: PascalCase (e.g., `Object`, `Logger`, `TransformComponent`)
- **Functions**: PascalCase (e.g., `getName()`, `setGuid()`)
- **Member variables**: `mVariableName` prefix (e.g., `mObjectName`, `mId`)
- **Constants**: `c8`, `c16` prefixes for char constants; otherwise PascalCase
- **Namespaces**: `OSRE::Module` (e.g., `OSRE::Common`, `OSRE::App`, `OSRE::RenderBackend`)
- **Macros**: ALL_CAPS with underscores (e.g., `DECL_OSRE_LOG_MODULE`)

### File Organization
- **Headers**: `.h` extension
- **Implementation**: `.cpp` extension
- **Include order**:
  1. Corresponding header (if .cpp)
  2. Project headers (e.g., `"Common/Object.h"`)
  3. cppcore library headers (e.g., `<cppcore/Container/TArray.h>`)
  4. System/stl headers
- **Include guards**: Use `#pragma once`

### Code Patterns

#### Class Declaration
```cpp
#pragma once

#include "Common/osre_common.h"
#include <cppcore/Container/TArray.h>

namespace OSRE::Common {

class OSRE_EXPORT Object {
public:
    virtual ~Object() = default;
    
    void setName(const String &objName);
    const String &getName() const;

    // No copying
    Object(const Object &) = delete;
    Object &operator = (const Object &) = delete;

protected:
    Object(const String &objectName);

private:
    String mObjectName;
    guid mId;
};

} // namespace OSRE::Common
```

#### Error Handling
- Use exceptions for exceptional cases (construction failures, etc.)
- Use `try/catch` blocks with `...` catch for generic exception handling in tests
- Log errors via `Logger` class with severity levels
- Return `nullptr` or error codes for expected failure cases

#### DLL Exports
- Use `OSRE_EXPORT` macro for any class that needs to be exported from the DLL

#### Logging
```cpp
DECL_OSRE_LOG_MODULE(MyModule);

void myFunction() {
    Logger::Warning(Tag, "Warning message");
    Logger::Error(Tag, "Error message");
}
```

### License Header
Every source file must include the MIT license header:
```cpp
/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling
// ... (full MIT text)
-----------------------------------------------------------------------------------------------*/
```

## Project Structure

```
src/Engine/
  Animation/     - Animation components
  App/           - Application layer (Scene, Entity, Components)
  Common/        - Base classes, utilities (Object, Logger, Event)
  Debugging/     - Debug rendering
  IO/            - File I/O, URI handling
  Modules/       - Module system
  Platform/     - Platform abstraction (SDL2, Win32)
  Profiling/     - Performance counters
  Properties/    - Property system
  RenderBackend/ - Graphics backend (OGL, Vulkan placeholder)
  scripting/     - Scripting integration
  Threading/     - Task/threading system
  UI/            - User interface components

test/
  UnitTests/     - Google Test unit tests
  RenderTests/  - Render verification tests

samples/         - Example applications
```

## Key Libraries
- **cppcore**: Custom container classes (TArray, etc.)
- **Assimp**: Model loading (40+ formats)
- **SDL2**: Window/input handling
- **GLEW**: OpenGL extension management
- **GLM**: Math library

## Git Conventions
- Branch from `master`
- Pull requests go to `master`
- Commit messages should be descriptive
