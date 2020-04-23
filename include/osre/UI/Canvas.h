/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/UI/Widget.h>
#include <osre/Common/TObjPtr.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace RenderBackend {
    class RenderBackendService;
}

namespace Platform {
    class AbstractWindow;
}

namespace UI {

class FocusControl;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Canvas : public Widget {
public:
    Canvas( const String &name, ui32 x, ui32 y, ui32 width, ui32 height );
    ~Canvas() override;
    virtual void setSurface( Platform::AbstractWindow *surface );
    const RenderBackend::TransformMatrixBlock &getTransform() const;
    FocusControl *getFocusControl() const;

protected:
    void onLayout() override;
    void onRender( UiRenderCmdCache &renderCmdCache, RenderBackend::RenderBackendService *rbSrv ) override;
    void onResize( ui32 x, ui32 y, ui32 w, ui32 h ) override;

private:
    Platform::AbstractWindow *m_surface;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    FocusControl *m_focusControl;
};

} // Namespace UI
} // Namespace OSRE
