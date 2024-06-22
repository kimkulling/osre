/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "Platform/AbstractDynamicLoader.h"

namespace OSRE {
namespace Platform {
        
//-------------------------------------------------------------------------------------------------
/// @ingroup	Engine
///
/// @brief  This class implements how to load shared libs / dlls using the SDL"-platform 
/// abstraction.
//-------------------------------------------------------------------------------------------------
class SDL2DynamicLoader final : public AbstractDynamicLoader {
public:
    /// @brief The class constructor.
    SDL2DynamicLoader();

    /// @brief The class destructor.    
    ~SDL2DynamicLoader() override;

    /// @brief Tries to the lod dynlib descriped by its name.
    /// @param libName  The library name,
    /// @return The handle or nullptr in case of an error.
    LibHandle *load( const String &libName ) override;

    /// @brief Tries to look for a given lib.
    /// @param libName  The library name.
    /// @return The library or nullptr if the library does not exists.
    LibHandle *lookupLib( const String &libName )  override;

    /// @brief WIll try to unload the library.
    /// @param libName THe library name.
    void unload( const String &libName )  override;

    /// @brief Will look for an exported function in a loaded lib.
    /// @param name The function name.
    /// @return The handle of the function or nullptr in case of an error.
    void *loadFunction( const String &name ) override;
};

} // Namespace Platform
} // Namespace OSRE
