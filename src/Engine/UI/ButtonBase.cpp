#include <osre/UI/ButtonBase.h>

namespace OSRE {
namespace UI {

StyleProvider *StyleProvider::s_instance = nullptr;

Style &StyleProvider::getCurrentStyle() {
    if ( nullptr == s_instance ) {
        s_instance = new StyleProvider;
    }
    return s_instance->m_activeStyle;
}

StyleProvider::StyleProvider()
: m_activeStyle() {
    // empty
}

StyleProvider::~StyleProvider() {
    // empty
}

ButtonBase::ButtonBase( const String &name )
: Object( name )
, m_rect( 0,0,1,1 )
, m_z( 0 ) {
    static_cast<void>( StyleProvider::getCurrentStyle() );
}

ButtonBase::~ButtonBase() {
    // empty

}

}
}
