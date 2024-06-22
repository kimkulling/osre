/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/AssetRegistry.h"
#include "Common/Logger.h"
#include "Common/StringUtils.h"
#include "IO/Uri.h"
#include "IO/Stream.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::IO;

AssetRegistry *AssetRegistry::sInstance = nullptr;

static constexpr c8 Tag[] = "AssetRegistry";

AssetRegistry *AssetRegistry::create() {
    if ( nullptr == sInstance ) {
        sInstance = new AssetRegistry;
    }

    return sInstance;
}

void AssetRegistry::destroy() {
    if ( nullptr == sInstance ) {
        return;
    }

    delete sInstance;
    sInstance = nullptr;
}


bool AssetRegistry::registerAssetPath(const String &mount, const String &path) {
    if (nullptr == sInstance) {
        return false;
    }
    
    const HashId hashId = StringUtils::hashName(mount);
    sInstance->mName2pathMap.insert(hashId, path);

    return true;
}

bool AssetRegistry::hasPath( const String &mount ) {
    if ( nullptr == sInstance ) {
        return false;
    }

    const HashId hashId = StringUtils::hashName(mount);
    if ( !sInstance->mName2pathMap.hasKey( hashId ) ) {
        return false;
    }

    return true;
}


String AssetRegistry::getPath( const String &mount ) {
    static const String Dummy("");
    if (nullptr == sInstance) {
        return Dummy;
    }

    const HashId hashId = StringUtils::hashName(mount);
    if ( !sInstance->mName2pathMap.hasKey(hashId)) {
        return Dummy;
    }

    String path;
    if (sInstance->mName2pathMap.getValue(hashId, path) ) {
        return path;
    }

    return Dummy;
}

String AssetRegistry::resolvePathFromUri(const IO::Uri &location) {
    static const String Dummy("");
    if (location.isEmpty()) {
        osre_debug(Tag, "Enpty path detected.");
        return Dummy;
    }

    const String pathToCheck = location.getAbsPath();
    String absPath(pathToCheck);
    const String::size_type pos = pathToCheck.find( "/" );
    String mountPoint = pathToCheck.substr( 0, pos );
    String::size_type offset = pos + mountPoint.size() + 1;
    if (hasPath( mountPoint )) {
        absPath = getPath( mountPoint );
        if ( absPath[ absPath.size()-1 ] != '/') {
            absPath += '/';
            ++offset;
        }
        const String rest = pathToCheck.substr(pos+1, pathToCheck.size() - pos-1);
        absPath += rest;
    }
    
    return absPath;
}

bool AssetRegistry::registerAssetPathInBinFolder(const String &mount, const String &foldername) {
    if (mount.empty()) {
        osre_error(Tag, "Cannot register asset-mount, name is empty.");
        return false;
    }

    if (foldername.empty()) {
        osre_error(Tag, "Cannot register asset-folder, name is empty.");
        return false;
    }

    bool ok = true;
    #ifdef OSRE_WINDOWS
        ok = AssetRegistry::registerAssetPath(mount, "../../" + foldername);
    #else
        ok = AssetRegistry::registerAssetPath(mount, "../" + foldername);
    #endif

    return ok;
}

bool AssetRegistry::clear() {
    if ( nullptr == sInstance ) {
        return false;
    }

    sInstance->mName2pathMap.clear();

    return true;
}

} // Namespace Assets
} // Namespace OSRE
