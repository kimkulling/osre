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
#include <osre/Assets/AssetData.h>
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

AssetData::Chunk::Chunk()
: m_id(-1)
, m_size(0)
, m_data(nullptr) {
    // empty
}

AssetData::Chunk::~Chunk() {
    // empty
}

AssetData::AssetDataDict::AssetDataDict()
: m_numEntries( 0 )
, m_entries(nullptr) {
    // empty
}

AssetData::AssetDataDict::~AssetDataDict() {
    m_entries = nullptr;
}

bool AssetData::checkReadState( IO::Stream &stream ) {
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

bool AssetData::checkWriteState( IO::Stream &stream ) {
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

uc8 *AssetData::allocChunkData( ui32 size ) {
    return new uc8[ size ];
}

void AssetData::freeChunkData( uc8 *buffer ) {
    delete[] buffer;
}

ui32 AssetData::readChunkData( IO::Stream &stream, ui32 offset, i32 &id, uc8 *buffer, ui32 &size ) {
    offset += stream.readI32( id );
    offset += stream.readUI32( size );
    buffer = allocChunkData( size );
    offset += stream.read( buffer, size );

    return offset;
}

ui32 AssetData::writeChunkData( IO::Stream &stream, ui32 offset, i32 id, uc8 *buffer, ui32 size ) {
    if ( nullptr == buffer ) {
        return offset;
    }

    offset += stream.writeI32( id );
    offset += stream.writeUI32( size );
    offset += stream.write( buffer, size );

    return offset;
}

bool AssetData::readHeader( IO::Stream &stream ) {
    ui32 offset( stream.seek( 0, IO::Stream::Origin::Begin ) );
    c8 bufferMagic[ 4 ];
    offset += stream.read( bufferMagic, sizeof( c8 ) * 4 );
    if ( 0 == strncmp( bufferMagic, MagicOSRE, strlen( MagicOSRE ) ) ) {
        return true;
    }

    i32 version( 0 );
    offset += stream.readI32( version );

    i32 id( -1 );
    uc8 *buffer( nullptr );

    // read dict chunk 
    AssetDataDict dict;
    offset = readDict( stream, dict );
    for ( ui32 i = 0; i < dict.m_numEntries; i++ ) {
        DictEntry &entry = dict.m_entries[ i ];
    }

    return false;
}

bool AssetData::writeHeader( IO::Stream &stream ) {
    ui32 offset( stream.seek( 0, Stream::Origin::Begin ) );
    offset = stream.write( MagicOSRE, sizeof( uc8 ) * 4 );
    offset = stream.writeI32( Version );

    return true;
}

ui32 AssetData::readDict( IO::Stream &stream, AssetDataDict &dict ) {
    ui32 size( 0 );
    i32 id( -1 );
    uc8 *buffer( nullptr );
    ui32 offset = readChunkData( stream, offset, id, buffer, size );
    ::memcpy( &dict.m_numEntries, buffer, sizeof( ui32 ) );
    // TODO!
    //dict.m_entries = buffer;

    return 0;
}

ui32 AssetData::writeDict() {
    return 0;
}

AssetData::AssetData() {

}

AssetData::~AssetData() {

}

bool AssetData::read( IO::Stream &stream ) {
    if ( !checkReadState( stream ) ) {
        return false;
    }

    if ( !readHeader( stream ) ) {
        return false;
    }

    return true;
}

bool AssetData::write( IO::Stream &stream ) {
    if ( !checkWriteState( stream ) ) {
        return false;
    }

    if ( !writeHeader( stream ) ) {
        return false;
    }

    return true;
}

bool AssetData::isLoaded() const {
    return false;
}

}
}
