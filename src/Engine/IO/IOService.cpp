/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "IO/IOService.h"
#include "Common/Tokenizer.h"
#include "Common/Logger.h"
#include "IO/LocaleFileSystem.h"

IMPLEMENT_SINGLETON(::OSRE::IO::IOService)

namespace OSRE::IO {

using namespace OSRE::Common;

static constexpr c8 Tag[] = "IOService";

IOService::IOService() : AbstractService("io/ioserver"), mMountedMap() {
    CREATE_SINGLETON( IOService );
}

IOService::~IOService() {
    DESTROY_SINGLETON( IOService );    
}

bool IOService::onOpen() {
    // create the locale file system
    AbstractFileSystem *pFileSystem{ nullptr };
    pFileSystem = new LocaleFileSystem;
    mountFileSystem( pFileSystem->getSchema(), pFileSystem );

    return true;
}

bool IOService::onClose() {
    if (mMountedMap.empty()) {
        return true;
    }

    for (MountedMap::iterator it = mMountedMap.begin(); it != mMountedMap.end(); ++it) {
        delete it->second;
    }
    mMountedMap.clear();
    
    return true;
}

bool IOService::onUpdate() {
    return true;
}

void IOService::mountFileSystem(const String &mountPoint, AbstractFileSystem *fileSystem) {
    mMountedMap[mountPoint] = fileSystem;
}

void IOService::umountFileSystem(const String &mountPoint, AbstractFileSystem *fileSystem) {
    MountedMap::iterator it = mMountedMap.find(mountPoint);
    if ( mMountedMap.end() == it ) {
        return;
    }
    if (it->second == fileSystem) {
        mMountedMap.erase(it);
    }
}

Stream *IOService::openStream(const Uri &file, Stream::AccessMode mode) {
    if (auto *fs = getFileSystem(Uri::schemeEnumToStr(file.getScheme())); fs != nullptr) {
        return fs->open( file, mode );
    }

    return nullptr;
}

void IOService::closeStream( Stream **ppStream ) {
    if (nullptr == ppStream) {
        osre_error(Tag, "Invalid pointer to stream.");
        return;
    }

    if (nullptr == *ppStream) {
        osre_error(Tag, "Incalid stream.");
        return;
    }
    
    const String &schema = Uri::schemeEnumToStr((*ppStream)->getUri().getScheme());
    if (auto *fs = getFileSystem(schema); fs != nullptr) {
        fs->close( ppStream );
    }
}

AbstractFileSystem *IOService::getFileSystem( const String &schema ) const {
    if (mMountedMap.empty()) {
        return nullptr;
    }

    if (auto it = mMountedMap.find(schema); mMountedMap.end() != it) {
        return it->second;
    } 

    return nullptr;
}

bool IOService::fileExists( const Uri &file ) const {
    bool exists = false;
    AbstractFileSystem *fs = this->getFileSystem(Uri::schemeEnumToStr(file.getScheme()));
    if (fs != nullptr) {
        exists = fs->fileExist(file);
    }

    return exists;
}

IOService *IOService::create() {
    return new IOService;
}

} // Namespace OSRE::IO
