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
#include <cppcore/Container/THashMap.h>

namespace OSRE {
    
// Forward declarations
namespace IO {
    class Stream;
    class Uri;
}

namespace RenderBackend {
    struct BufferData;
}

namespace Assets {
        
class AssetData {
public:
    AssetData();
    ~AssetData();
    bool read( IO::Stream &stream );
    bool write( IO::Stream &stream );
    bool isLoaded() const;

private:
    struct Chunk {
        ui32 m_offset;
        ui32 m_id;
        ui32 m_size;
        uc8 *m_data;
    };
};

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AssetRegistry {
public:
    static AssetRegistry *create();
    static void destroy();
    static AssetRegistry *getInstance();
    void registerAssetPath( const String &mount, const String &path );
    bool hasPath( const String &mount ) const;
    String getPath( const String &mount ) const;
    String resolvePathFromUri( const IO::Uri &location );
    void clear();

private:
    AssetRegistry();
    ~AssetRegistry();

private:
    static AssetRegistry *s_instance;
    typedef CPPCore::THashMap<ui32, String> Name2PathMap;
    Name2PathMap m_name2pathMap;
};

} // Namespace Assets
} // Namespace OSRE
