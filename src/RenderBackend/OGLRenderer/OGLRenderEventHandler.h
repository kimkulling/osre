/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <GL/glew.h>
#include <GL/gl.h>

namespace OSRE {

namespace Interface {
    class IWindow;
}

namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class OGLRenderContext;
class OGLRenderBackend;
class OGLShader;
class RenderCmdBuffer;

struct Vertex;
struct OGLVertexArray;
struct PrimitiveGroup;
struct Material;
struct OGLRenderCmd;
struct DrawPrimitivesCmdData;
struct SetParameterCmdData;
struct SetTextureStageCmdData;
struct SetShaderStageCmdData;
struct SetRenderTargetCmdData;
struct OGLParameter;

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::RenderBackend::OGLRenderEventHandler
///	@ingroup	RenderSystem
///
///	@brief  This class is used to handle all incoming events for the render back-end.
//-------------------------------------------------------------------------------------------------
class OGLRenderEventHandler : public Common::AbstractEventHandler {
public:
    /// The class constructor.
    OGLRenderEventHandler();
    /// The class destructor, virtual.
    virtual ~OGLRenderEventHandler();
    /// The OnEvent-callback.
    virtual bool onEvent( const Common::Event &ev, const Common::EventData *pEventData );
    /// Will set the active shader.
    void setActiveShader( OGLShader *pOGLShader );
    /// Will enqueue a new render command.
    void enqueueRenderCmd( OGLRenderCmd *pOGLRenderCmd );

protected:
    virtual bool onAttached( const Common::EventData *pEventData );
    virtual bool onDetached( const Common::EventData *pEventData );
    virtual bool onCreateRenderer( const Common::EventData *pEventData );
    virtual bool onDestroyRenderer( const Common::EventData *pEventData );
    virtual bool onAttachView( const Common::EventData *pEventData );
    virtual bool onAttachGeo( const Common::EventData *pEventData );
    virtual bool onClearGeo( const Common::EventData *pEventData );
    virtual bool onRenderFrame( const Common::EventData *pEventData );
    virtual bool onUpdateParameter( const Common::EventData *pEventData );
	virtual bool onRenderText(const Common::EventData *pEventData);

private:
    OGLRenderBackend *m_oglBackend;
    RenderCmdBuffer *m_renderCmdBuffer;
    Platform::AbstractRenderContext *m_pRenderCtx;
    OGLVertexArray *m_pVertexArray;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
