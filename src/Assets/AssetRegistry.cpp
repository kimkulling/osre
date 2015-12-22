/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/StringUtils.h>

namespace OSRE {
namespace Assets {

using namespace ::OSRE::Common;

AssetRegistry *AssetRegistry::s_instance( nullptr );

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

void AssetRegistry::registerAssetPath( const String &schema, const String &path ) {
    ui32 hashId( StringUtils::hashName( schema.c_str() ) );
    m_name2pathMap.insert( hashId, path );
}

static const String Dummy("");

String AssetRegistry::getPath( const String &name ) const {
    ui32 hashId( StringUtils::hashName(name.c_str() ) );
    if ( !m_name2pathMap.hasKey( hashId ) ) {
        return Dummy;
    }

    String path;
    if ( m_name2pathMap.getValue( hashId, path ) ) {
        return path;
    }

    return Dummy;
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
