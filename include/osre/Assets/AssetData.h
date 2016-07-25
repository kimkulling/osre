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

//--------
//  + Magic number
//  + Version id
//---
//  Dictionary chunk
//  + Number of chunks
//-----
//-----
class AssetData {
public:
    struct Chunk {
        i32 m_id;
        ui32 m_size;
        uc8 *m_data;
    };

    struct AssetDataDict {
        ui32 m_numChunks;
    };
    
public:
    AssetData();
    ~AssetData();
    bool read( IO::Stream &stream );
    bool write( IO::Stream &stream );
    bool isLoaded() const;

    static bool checkReadState( IO::Stream &stream );
    static bool checkWriteState( IO::Stream &stream );
    static uc8* allocChunkData( ui32 size );
    static void freeChunkData( uc8 *buffer );
    static ui32 readChunkData( IO::Stream &stream, ui32 offset, i32 &id, uc8 *buffer, ui32 &size );
    static ui32 writeChunkData( IO::Stream &stream, ui32 offset, i32 id, uc8 *buffer, ui32 size );
    static bool readHeader( IO::Stream &stream );
    static bool writeHeader( IO::Stream &stream );
    static ui32 readDict( IO::Stream &stream, AssetDataDict &dict );
    static ui32 writeDict();

private:

};

}
}

