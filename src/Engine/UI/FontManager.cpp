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
