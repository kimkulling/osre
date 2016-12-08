#include <osre/UI/ButtonBase.h>

namespace OSRE {
namespace UI {

ButtonBase::ButtonBase( const String &name )
: Object( name )
, m_rect( 0,0,1,1 )
, m_z( 0 ) {
    // empty
}

ButtonBase::~ButtonBase() {
    // empty

}

}
}
