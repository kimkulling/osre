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
///	@class		::ZFXCE2::RenderBackend::OGLRenderEventHandler
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
    
private:
    OGLRenderBackend *m_oglBackend;
    RenderCmdBuffer *m_renderCmdBuffer;
    Platform::AbstractRenderContext *m_pRenderCtx;
    OGLVertexArray *m_pVertexArray;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2
