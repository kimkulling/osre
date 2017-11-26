/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace UI {

struct UiFlags {
    static const ui32 Resizable = 0x001;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Panel : public Widget {
public:
    Panel( const String &name, ui32 flags, Widget *parent );
    virtual ~Panel();
    ui32 getFlags() const;
    bool isEnabled( ui32 flag ) const;
    void setHeadline( const String &headline );
    const String &getHeadline() const;

protected:
    virtual void onRender( UiVertexCache &vertexCache, UiIndexCache &indexCache, RenderBackend::RenderBackendService *rbSrv );

private:
    f32 m_angle;
    ui32 m_flags;
    String m_headline;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
};

} // Namespace UI
} // Namespace OSRE
