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

#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/THWBufferManager.h>

#include <GL/glew.h>
#include <GL/gl.h>

namespace OSRE {

// Forward declarations
namespace Interface {
    class IWindow;
}

namespace Platform {
    class AbstractOGLRenderContext;
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
struct SetTextureStageCmdData;
struct SetShaderStageCmdData;
struct SetRenderTargetCmdData;
struct OGLParameter;
struct OGLBuffer;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
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
    virtual bool onEvent( const Common::Event &ev, const Common::EventData *pEventData ) override;
    /// Will set the active shader.
    void setActiveShader( OGLShader *pOGLShader );
    /// Will enqueue a new render command.
    void enqueueRenderCmd( OGLRenderCmd *pOGLRenderCmd );

    void setParameter( const ::CPPCore::TArray<OGLParameter*> &paramArray );

    RenderCmdBuffer *getRenderCmdBuffer() const { return m_renderCmdBuffer; }

protected:
	///	@brief	Callback for attaching the event handler.
    virtual bool onAttached( const Common::EventData *eventData ) override;
	///	@brief	Callback for detaching the event handler.
	virtual bool onDetached( const Common::EventData *eventData ) override;
	///	@brief	Callback for render backend creation.
	virtual bool onCreateRenderer( const Common::EventData *eventData );
	///	@brief	Callback for render backend destroying.
	virtual bool onDestroyRenderer( const Common::EventData *eventData );
	///	@brief	Callback for attaching a new view onto a stage.
	virtual bool onAttachView( const Common::EventData *eventData );
    ///	@brief	Callback for detaching a new view onto a stage.
    virtual bool onDetachView( const Common::EventData *eventData );
    ///	@brief	Callback for clearing all geometry from a stage.
	virtual bool onClearGeo( const Common::EventData *eventData );
    ///	@brief	Callback for the render frame.
	virtual bool onRenderFrame( const Common::EventData *eventData );
    /// @brief  Callback to init the passes.
    virtual bool onInitRenderPasses(const Common::EventData *eventData);
    /// @brief  Callback to commit the next frame.
    virtual bool onCommitNexFrame( const Common::EventData *eventData );
    /// @brief  Callback for dealing with a shutdown request.
    virtual bool onShutdownRequest( const Common::EventData *eventData );
    /// @brief  Callback for dealing with resize events.
    virtual bool onResizeRenderTarget( const Common::EventData *eventData );

private:
    bool m_isRunning;
    OGLRenderBackend *m_oglBackend;
    RenderCmdBuffer *m_renderCmdBuffer;
    Platform::AbstractOGLRenderContext *m_renderCtx;
    OGLVertexArray *m_vertexArray;
    HWBufferManager<OGLBuffer> *mHwBufferManager;
};

} // Namespace RenderBackend
} // Namespace OSRE
