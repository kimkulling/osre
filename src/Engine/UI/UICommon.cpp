#include <osre/UI/UICommon.h>
#include <osre/Common/Logger.h>

namespace OSRE {
namespace UI {


static const c8 *Tag = "StyleProvider";

StyleProvider *StyleProvider::s_instance = nullptr;

Style &StyleProvider::getCurrentStyle() {
    if (nullptr == s_instance) {
        s_instance = new StyleProvider;
    }

    return s_instance->m_activeStyle;
}

void StyleProvider::setStyle( const Style &newStyle ) {
    if (nullptr == s_instance) {
        static_cast< void >( StyleProvider::getCurrentStyle() );
    }

    if (nullptr == s_instance) {
        osre_debug( Tag, "Singleton instance is nullptr." );
        return;
    }

    if (newStyle != s_instance->m_activeStyle) {
        s_instance->m_activeStyle = newStyle;
    }
}

StyleProvider::StyleProvider()
    : m_activeStyle() {
    // empty
}

StyleProvider::~StyleProvider() {
    // empty
}

} // Namespace UI
} // Namespace OSRE
