/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/UI/UICommon.h>
#include <osre/UI/Widget.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {

// Forward declarations
namespace Platform {
    class AbstractWindow;
}

namespace RenderBackend {
    class RenderBackendService;
}

namespace UI {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT ButtonBase : public Widget {
public:
    ButtonBase( const String &name, Widget *parent );
    virtual ~ButtonBase();
    virtual void setLabel( const String &label );
    virtual const String &getLabel() const;
    virtual void setImage( Image &image );
    virtual void setImage( const String &name );
    virtual const String &getImage() const;
    void registerCallback( WidgetState state, UiFunctor functor );
    static ButtonBase *createIconButton( const String &name, Image &icon, Widget *parent );
    static ButtonBase *createBaseButton( const String &name, const String &label, Widget *parent );

protected:
    void onLayout() override;
    void onRender(UiRenderCmdCache &renderCmdCache, RenderBackend::RenderBackendService *rbSrv) override;
    void onMouseDown(const Point2ui &pt, void *data ) override;
    void onMouseUp(const Point2ui &pt, void *data) override;

private:
    String m_label;
    IO::Uri m_image;
    Image *m_imageWidget;
    TextBase *m_textWidget;
    FunctorContainer *m_callback;
};

} // Namespace UI
} // Namespace OSRE

