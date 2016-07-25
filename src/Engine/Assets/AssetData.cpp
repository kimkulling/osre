#include <osre/Assets/AssetData.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Stream.h>

namespace OSRE {
namespace Assets {

static const String Tag = "AssetData";

static const i32 HeaderId = 0;
static const i32 DictId = 1;
static const c8  MagicOSRE[] = "OSRE";
static const i32 Version = 1;

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
    ui32 offset( stream.seek( 0, IO::Stream::Begin ) );
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
    for ( ui32 i = 0; i < dict.m_numChunks; i++ ) {

    }

    return false;
}

bool AssetData::writeHeader( IO::Stream &stream ) {
    ui32 offset( stream.seek( 0, IO::Stream::Begin ) );
    offset = stream.write( MagicOSRE, sizeof( uc8 ) * 4 );
    offset = stream.writeI32( Version );

    return true;
}

ui32 AssetData::readDict( IO::Stream &stream, AssetDataDict &dict ) {
    ui32 size( 0 );
    i32 id( -1 );
    uc8 *buffer( nullptr );
    ui32 offset = readChunkData( stream, offset, id, buffer, size );
    ::memcpy( &dict.m_numChunks, buffer, sizeof( ui32 ) );

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
