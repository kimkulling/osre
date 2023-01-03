/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <cppcore/Container/THashMap.h>

namespace OSRE {
    
// Forward declarations ---------------------------------------------------------------------------
namespace IO {
    class Stream;
    class Uri;
}

namespace RenderBackend {
    struct BufferData;
}

namespace App {

class World;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AssetRegistry {
public:
    static AssetRegistry *create();
    static void destroy();
    static bool registerAssetPath( const String &mount, const String &path );
    static bool hasPath( const String &mount );
    static String getPath( const String &mount );
    static String resolvePathFromUri( const IO::Uri &location );
    static bool clear();

private:
    AssetRegistry();
    ~AssetRegistry();

private:
    static AssetRegistry *s_instance;

    typedef cppcore::THashMap<ui32, String> Name2PathMap;
    Name2PathMap m_name2pathMap;
};

} // Namespace Assets
} // Namespace OSRE
