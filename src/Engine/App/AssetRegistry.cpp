/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/AssetRegistry.h>
#include <osre/Common/Logger.h>
#include <osre/Common/StringUtils.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Stream.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;

AssetRegistry *AssetRegistry::s_instance = nullptr;
static const   String Tag                = "AssetRegistry";

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


bool AssetRegistry::registerAssetPath( const String &mount, const String &path ) {
    if ( nullptr == s_instance ) {
        return false;
    }
    const ui32 hashId( StringUtils::hashName( mount ) );
    s_instance->m_name2pathMap.insert( hashId, path );

    return true;
}

bool AssetRegistry::hasPath( const String &mount ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    const ui32 hashId( StringUtils::hashName( mount ) );
    if ( !s_instance->m_name2pathMap.hasKey( hashId ) ) {
        return false;
    }

    return true;
}

static const String Dummy("");

String AssetRegistry::getPath( const String &mount ) {
    if ( nullptr == s_instance ) {
        return Dummy;
    }

    const HashId hashId( StringUtils::hashName( mount ) );
    if ( !s_instance->m_name2pathMap.hasKey( hashId ) ) {
        return Dummy;
    }

    String path;
    if ( s_instance->m_name2pathMap.getValue( hashId, path ) ) {
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
    const String::size_type pos = pathToCheck.find( "/" );
    String mountPoint = pathToCheck.substr( 0, pos );
    String::size_type offset = pos + mountPoint.size() + 1;
    if ( hasPath( mountPoint ) ) {
        absPath = getPath( mountPoint );
        if ( absPath[ absPath.size()-1 ]!='/' ) {
            absPath += '/';
            offset++;
        }
        const String rest = pathToCheck.substr( pos+1, pathToCheck.size() - pos-1 );
        absPath += rest;
    }
    
    return absPath;
}

bool AssetRegistry::clear() {
    if ( nullptr == s_instance ) {
        return false;
    }

    s_instance->m_name2pathMap.clear();

    return true;
}

AssetRegistry::AssetRegistry() 
: m_name2pathMap() {
    // empty
}

AssetRegistry::~AssetRegistry() {
    // empty
}

} // Namespace Assets
} // Namespace OSRE
