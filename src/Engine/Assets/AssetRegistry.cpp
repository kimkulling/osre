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
#include <osre/Assets/AssetRegistry.h>
#include <osre/Common/Logger.h>
#include <osre/Common/StringUtils.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Stream.h>

namespace OSRE {
namespace Assets {

using namespace ::OSRE::Common;

AssetRegistry *AssetRegistry::s_instance( nullptr );
static const String Tag = "AssetRegistry";

static const i32 HeaderId    = 1;
static const c8  MagicOSRE[] = "OSRE";

static bool
checkReadState( IO::Stream &stream ) {
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

static bool
checkWriteState( IO::Stream &stream ) {
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

static bool 
readHeader( IO::Stream &stream ) {
    ui32 offset( stream.seek( 0, IO::Stream::Begin ) );
    c8 buffer[ 4 ];
    offset = stream.read( buffer, sizeof( c8 ) * 4 );
    if ( 0 == strncmp( buffer, MagicOSRE, strlen( MagicOSRE ) ) ) {
        return true;
    }

    return false;
}

static bool
writeHeader( IO::Stream &stream ) {
    ui32 offset( stream.seek( 0, IO::Stream::Begin ) );
    stream.write( MagicOSRE, sizeof( uc8 ) * 4 );

    return true;
}

AssetData::AssetData() {

}

AssetData::~AssetData() {

}

bool 
AssetData::read( IO::Stream &stream ) {
    if ( !checkReadState( stream ) ) {
        return false;
    }

    if ( !readHeader( stream ) ) {
        return false;
    }

    return true;
}

bool 
AssetData::write( IO::Stream &stream ) {
    if ( !checkWriteState( stream ) ) {
        return false;
    }

    if ( !writeHeader( stream ) ) {
        return false;
    }

    return true;
}

bool 
AssetData::isLoaded() const {
    return false;
}

AssetRegistry *AssetRegistry::create() {
    if ( nullptr == s_instance ) {
        s_instance = new AssetRegistry;
    }

    return s_instance;
}

void AssetRegistry::destroy() {
    if ( nullptr==s_instance ) {
        return;
    }

    delete s_instance;
    s_instance = nullptr;
}

AssetRegistry *AssetRegistry::getInstance() {
    return s_instance;
}

void AssetRegistry::registerAssetPath( const String &mount, const String &path ) {
    ui32 hashId( StringUtils::hashName( mount.c_str() ) );
    m_name2pathMap.insert( hashId, path );
}

bool AssetRegistry::hasPath( const String &mount ) const {
    ui32 hashId( StringUtils::hashName( mount.c_str() ) );
    if ( !m_name2pathMap.hasKey( hashId ) ) {
        return false;
    }

    return true;
}

static const String Dummy("");

String AssetRegistry::getPath( const String &mount ) const {
    ui32 hashId( StringUtils::hashName( mount.c_str() ) );
    if ( !m_name2pathMap.hasKey( hashId ) ) {
        return Dummy;
    }

    String path;
    if ( m_name2pathMap.getValue( hashId, path ) ) {
        return path;
    }

    return Dummy;
}

String AssetRegistry::resolvePathFromUri( const IO::Uri &location ) {
    if ( location.isEmpty() ) {
        return Dummy;
    }

    const String pathToCheck( location.getAbsPath() );
    String absPath( pathToCheck );
    String::size_type pos = pathToCheck.find( "/" );
    String mountPoint = pathToCheck.substr( 0, pos );
    if ( hasPath( mountPoint ) ) {
        absPath = getPath( mountPoint );
        if ( absPath[ absPath.size()-1 ]!='/' ) {
            absPath += '/';
        }
        String::size_type endPos = pathToCheck.rfind( "/" );
        absPath += pathToCheck.substr( pos+1, pathToCheck.size()-( pos + endPos-2 ));
        if ( absPath[ absPath.size()-1 ]!='/' ) {
            absPath += '/';
        }
    }
    
    return absPath;
}

void AssetRegistry::clear() {
    m_name2pathMap.clear();
}

AssetRegistry::AssetRegistry() 
: m_name2pathMap() {
    // empty
}

AssetRegistry::~AssetRegistry() {

}

}
}
