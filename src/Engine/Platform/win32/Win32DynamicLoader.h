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
/// @ingroup    Engine
///
///	@brief  This class implements the win32-specific dynamic-library loader.
//-------------------------------------------------------------------------------------------------
class Win32DynamicLoader : public AbstractDynamicLoader {
public:
    /// The class constructor.
    Win32DynamicLoader() = default;
    /// The class destructor, virtual.
    virtual ~Win32DynamicLoader() override = default;
    /// Loads a dll.
    virtual LibHandle *load( const String &libName ) override;
    /// Performs a lookup for a dll.
    virtual LibHandle *lookupLib( const String &libName ) override;
    /// Unloads a loaded dll.
    virtual void unload( const String &libName ) override;
    /// Loads the function symbol from a dll.
    virtual void *loadFunction( const String &name ) override;
};

} // namespace Platform
} // Namespace OSRE
