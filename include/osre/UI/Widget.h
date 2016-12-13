#pragma once

#include <osre/Common/Object.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
}

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

class Widget : public Common::Object {
public:
    virtual ~Widget();
    virtual void setParent( Widget *parent );
    virtual Widget *getParent() const;
    virtual bool addChildWidget( Widget *child );
    virtual bool removeChildWidget( Widget *child );
    virtual void setRect( ui32 x, ui32 y, ui32 w, ui32 h );
    virtual void requestRedraw();
    virtual bool redrawRequested() const;
    virtual void render( RenderBackend::RenderBackendService *rbSrv ) = 0;

protected:
    Widget( const String &name, Widget *parent );

private:
    Widget *m_parent;
    CPPCore::TArray<Widget*> m_children;
    RectUI m_rect;
    ui32 m_z;
    bool m_redrawRequest;
};

}
}
