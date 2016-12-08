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

namespace OSRE {

// Forward declarations
namespace IO {
    class Stream;
    class Uri;
}

namespace RenderBackend {
    struct BufferData;
}

namespace Scene {
    class World;
}

namespace Assets {

//====================================
// Header
//------------------------------------
// + Magic number
// + Version id
//------------------------------------
// Dictionary chunk
// + Number of chunks
// + ChunkId
// + Offset
// ...
//------------------------------------
// Chunks
//------------------------------------
class OSRE_EXPORT AssetDataArchive {
public:
    struct Chunk {
        i32  m_id;
        ui32 m_size;
        uc8 *m_data;

        Chunk();
        ~Chunk();
    };

    struct DictEntry {
        ui32 m_id;
        ui32 m_offset;

        DictEntry();
        ~DictEntry();
    };

    struct AssetDataDict {
        ui32 m_numEntries;
        DictEntry *m_entries;

        AssetDataDict();
        ~AssetDataDict();
    };
    
public:
    AssetDataArchive( ui32 minVersion );
    ~AssetDataArchive();
    bool read( IO::Stream &stream );
    bool write( IO::Stream &stream );
    bool isLoaded() const;
    
    static bool checkReadState( IO::Stream &stream );
    static bool checkWriteState( IO::Stream &stream );
    static uc8* allocChunkData( ui32 size );
    static void freeChunkData( uc8 *buffer );
    static ui32 readChunkData( IO::Stream &stream, ui32 offset, i32 &id, uc8 *buffer, ui32 &size );
    static ui32 writeChunkData( IO::Stream &stream, ui32 offset, i32 id, uc8 *buffer, ui32 size );
    static bool readHeader( IO::Stream &stream, ui32 minVersion, AssetDataDict &dict, ui32 &offset );
    static bool writeHeader( IO::Stream &stream, ui32 &offset );
    static void readDict( IO::Stream &stream, AssetDataDict &dict, ui32 &offset );
    static ui32 writeDict();

private:
    AssetDataDict m_dict;
    ui32 m_minVersion;

};

}
}

