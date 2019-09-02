/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/UI/FontManager.h>
#include <osre/Common/StringUtils.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

FontManager *FontManager::s_instance = nullptr;

static ui32 calcHash( const String &name ) {
    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    return hash;
}

FontManager *FontManager::create() {
    if ( nullptr == s_instance ) {
        s_instance = new FontManager;
    }
    return s_instance;
}

void FontManager::destroy() {
    if ( nullptr != s_instance ) {
        delete s_instance;
        s_instance = nullptr;
    }
}

FontManager *FontManager::getInstance() {
    return s_instance;
}

RenderBackend::FontBase *FontManager::getFontByName( const String &fontName ) const {
    if ( fontName.empty() ) {
        return nullptr;
    }
    
    FontBase *font( nullptr );
    const ui32 hash( calcHash( fontName ) );
    if ( m_fontHashMap.hasKey( hash ) ) {
        m_fontHashMap.getValue( hash, font );
    }
    
    return font;
}

bool FontManager::loadFont( const String &name ) {
    return false;
}

FontManager::FontManager()
: m_fontHashMap() {

}

FontManager::~FontManager() {
    // empty
}

} // Namespace UI
} // Namespace OSRE
