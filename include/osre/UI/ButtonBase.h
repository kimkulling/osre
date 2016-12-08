#pragma once

#include <osre/Common/Object.h>

namespace OSRE {
namespace UI {

struct Style {
    enum class ColorTable {
        FGColor = 0,
        BGColor,
        TextColor
    };

    CPPCore::TArray<Color4> m_properties;

    Style()
    : m_properties() {
        m_properties.add( Color4( 1.f, 1.f, 1.f, 1.f ) );
        m_properties.add( Color4( 0.f, 0.f, 0.f, 0.f ) );
        m_properties.add( Color4( 1.f, 1.f, 1.f, 1.f ) );
    }
};

class StyleProvider {
public:
    static Style &getCurrentStyle();

private:
    void create();
    StyleProvider();
    ~StyleProvider();

private:
    static StyleProvider *s_instance;
    Style m_activeStyle;
};

class ButtonBase : public Common::Object {
public:
    ButtonBase( const String &name );
    virtual ~ButtonBase();
    void setRect( ui32 x, ui32 y, ui32 w, ui32 h );

private:
    RectUI m_rect;
    ui32 m_z;
};

}
}

