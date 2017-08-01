/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Assets/AssetDataArchive.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Stream.h>

namespace OSRE {
namespace Assets {

using namespace ::OSRE::IO;

static const String Tag = "AssetData";

static const i32 HeaderId    = 0;
static const i32 DictId      = 1;
static const c8  MagicOSRE[] = "OSRE";
static const i32 Version     = 1;

enum class TokenType : ui32 {
    WorldToken    = 1,
    StageToken    = 2,
    ViewToken     = 3,
    NodeToken     = 4,
    GeometryToken = 5
};

AssetDataArchive::Chunk::Chunk()
: m_id(-1)
, m_size(0)
, m_data(nullptr) {
    // empty
}

AssetDataArchive::Chunk::~Chunk() {
    // empty
}

AssetDataArchive::AssetDataDict::AssetDataDict()
: m_numEntries( 0 )
, m_entries(nullptr) {
    // empty
}

AssetDataArchive::AssetDataDict::~AssetDataDict() {
    m_entries = nullptr;
}

bool AssetDataArchive::checkReadState( IO::Stream &stream ) {
    if ( !stream.isOpen() ) {
        osre_debug( Tag, "Stream is not open." );
        return false;
    }
    if ( !stream.canRead() ) {
        osre_debug( Tag, "Stream is not in read state." );
        return false;
    }

    return true;
}

bool AssetDataArchive::checkWriteState( IO::Stream &stream ) {
    if ( !stream.isOpen() ) {
        osre_debug( Tag, "Stream is not open." );
        return false;
    }

    if ( !stream.canWrite() ) {
        osre_debug( Tag, "Stream is not in write state." );
        return false;
    }

    return true;
}

uc8 *AssetDataArchive::allocChunkData( ui32 size ) {
    return new uc8[ size ];
}

void AssetDataArchive::freeChunkData( uc8 *buffer ) {
    delete[] buffer;
}

ui32 AssetDataArchive::readChunkData( IO::Stream &stream, ui32 offset, i32 &id, uc8 *buffer, ui32 &size ) {
    offset += stream.readI32( id );
    offset += stream.readUI32( size );
    buffer = allocChunkData( size );
    offset += stream.read( buffer, size );

    return offset;
}

ui32 AssetDataArchive::writeChunkData( IO::Stream &stream, ui32 offset, i32 id, uc8 *buffer, ui32 size ) {
    if ( nullptr == buffer ) {
        return offset;
    }

    offset += stream.writeI32( id );
    offset += stream.writeUI32( size );
    offset += stream.write( buffer, size );

    return offset;
}

bool AssetDataArchive::readHeader( IO::Stream &stream, ui32 minVersion, AssetDataDict &dict, ui32 &offset ) {
    offset = stream.seek( 0, IO::Stream::Origin::Begin );
    c8 bufferMagic[ 4 ];
    offset += stream.read( bufferMagic, sizeof( c8 ) * 4 );
    if ( 0 != memcmp( bufferMagic, MagicOSRE, strlen( MagicOSRE ) ) ) {
        return false;
    }

    // validate min version
    i32 version( 0 );
    offset += stream.readI32( version );
    if ( static_cast<ui32>( version ) < minVersion ) {
        osre_error( Tag, "Version is less than min version." );
        return false;
    }

    // read dict chunk 
    readDict( stream, dict, offset );

    return true;
}

bool AssetDataArchive::writeHeader( IO::Stream &stream, ui32 &offset ) {
    offset =  stream.seek( 0, Stream::Origin::Begin );
    offset += stream.write( MagicOSRE, sizeof( uc8 ) * 4 );
    offset += stream.writeI32( Version );

    return true;
}

void AssetDataArchive::readDict( IO::Stream &stream, AssetDataDict &dict, ui32 &offset ) {
    ui32 size( 0 );
    i32 id( -1 );
    uc8 *buffer( nullptr );
    offset = readChunkData( stream, offset, id, buffer, size );
    if ( nullptr == buffer ) {
        osre_error( Tag, "Reading chunk failed." );
        return;
    }
    
    ::memcpy( &dict.m_numEntries, &buffer[ offset ], sizeof( ui32 ) );
    offset += sizeof( ui32 );
    
    ::memcpy( &dict.m_entries, &buffer[ offset ], dict.m_numEntries * sizeof( DictEntry ) );
    offset += dict.m_numEntries * sizeof( DictEntry );
}

ui32 AssetDataArchive::writeDict() {
    return 0;
}

AssetDataArchive::AssetDataArchive( ui32 minVersion )
: m_dict()
, m_minVersion( minVersion ) {
    // empty
}

AssetDataArchive::~AssetDataArchive() {
    // empty
}

bool AssetDataArchive::read( IO::Stream &stream ) {
    if ( !checkReadState( stream ) ) {
        return false;
    }
    ui32 offset( 0 );
    if ( !readHeader( stream, m_minVersion, m_dict, offset ) ) {
        return false;
    }

    // read all entries
    bool result( true );
    for ( ui32 i = 0; i < m_dict.m_numEntries; i++ ) {
        DictEntry &entry = m_dict.m_entries[ i ];
        switch ( static_cast<TokenType>( entry.m_id ) ) {
            case TokenType::WorldToken:
                result = readWorldData();
                break;
            case TokenType::StageToken:
                result = readStageData();
                break;
            case TokenType::ViewToken:
                result = readViewData();
                break;
            case TokenType::NodeToken:
                result = readNodeData();
                break;
            case TokenType::GeometryToken:
                result = readGeometryData();
                break;
            default:
                osre_error( Tag, "Unknown token!" );
                break;
        }
    }
    return true;
}

bool AssetDataArchive::write( IO::Stream &stream ) {
    if ( !checkWriteState( stream ) ) {
        return false;
    }

    ui32 offset( 0 );
    if ( !writeHeader( stream, offset ) ) {
        return false;
    }

    return true;
}

bool AssetDataArchive::isLoaded() const {
    return false;
}

bool AssetDataArchive::readWorldData() {
    return false;
}

bool AssetDataArchive::writeWorldData() {
    return false;
}

bool AssetDataArchive::readStageData() {
    return false;
}

bool AssetDataArchive::writeStageData() {
    return false;
}

bool AssetDataArchive::readViewData() {
    return false;
}

bool AssetDataArchive::writeViewData() {
    return false;
}

bool AssetDataArchive::readNodeData() {
    return false;
}

bool AssetDataArchive::writeNodeData() {
    return false;
}

bool AssetDataArchive::readGeometryData() {
    return false;
}

bool AssetDataArchive::writeGeometryData() {
    return false;
}

} // Namespace Assets
} // Namespace OSRE
