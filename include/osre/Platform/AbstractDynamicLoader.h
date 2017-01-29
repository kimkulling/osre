/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/Common/StringUtils.h>

#include <cppcore/Container/THashMap.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Platform {

/// @brief  A lib handle.
struct OSRE_EXPORT LibHandle {
    i32 m_index;
    void *m_handle; ///< The lib handle.

    LibHandle();
    ~LibHandle();
};

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
///	@brief  This class declares the abstract interface for dynamic library loaders.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractDynamicLoader {
public:
    /// @brief  The class destructor.
    virtual ~AbstractDynamicLoader();

    /// @brief  Loads a dynamic library.
    /// @param  libName     [in] The name of the lib.
    /// @return The lib handle or nullptr if dynamic library wasn't loaded.
    virtual LibHandle *load( const String &libName ) = 0;

    /// @brief  Performs a lookup, if the library was already loaded.
    /// @param  libName     [in] The name of the lib.
    /// @return The lib handle or nullptr if the dynamic library wasn't loaded.
    virtual LibHandle *lookupLib( const String &libName ) = 0;
    
    /// @brief  Unloads a loaded dynamic library.
    /// @param  libName     [in] The name of the lib.
    virtual void unload( const String &libName ) = 0;

    /// @brief  Loads a function from a dynamic library
    /// @param  name    [in] The function name to search for,
    /// @return The function pointer or nullptr, if the function was not found.
    virtual void *loadFunction( const String &name ) = 0;

public:
    void addLib( const String &libName, LibHandle *libHandle );
    void removeLib( const String &libName );
    LibHandle *findLib( const String &libName );
    void setActiveLib( LibHandle *handle );
    LibHandle *getActiveLib() const;

protected:
    /// @brief  The class constructor.
    AbstractDynamicLoader();

private:
    CPPCore::THashMap<ui32, LibHandle*> m_libmap;
    CPPCore::TArray<LibHandle*> m_handles;
    LibHandle *m_activeLib;
};

} // Namespace Platform
} // Namespace OSRE
