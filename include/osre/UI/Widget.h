/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
    virtual ui32 getNumChildren() const;
    virtual Widget *getChildWidgetAt( ui32 idx ) const;
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
