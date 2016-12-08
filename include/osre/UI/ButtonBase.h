#pragma once

#include <osre/Common/Object.h>

namespace OSRE {
namespace UI {

class ButtonBase : public Common::Object {
public:
    ButtonBase( const String &name );
    virtual ~ButtonBase();
    void setRect( ui32 x, ui32 y, ui32 w, ui32 h );

private:
    Rect2D m_rect;
    ui32 m_z;
};

}
}

