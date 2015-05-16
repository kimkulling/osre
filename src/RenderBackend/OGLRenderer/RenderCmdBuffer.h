#pragma once

#include <cppcore/Container/TArray.h>

namespace OSRE {
    
namespace Common {
    class EventData;
}

namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class OGLRenderBackend;
class OGLShader;

struct OGLVertexArray;
struct OGLRenderCmd;
struct DrawPrimitivesCmdData;
struct DrawInstancePrimitivesCmdData;
struct SetParameterCmdData;
struct SetTextureStageCmdData;
struct SetShaderStageCmdData;
struct SetRenderTargetCmdData;
struct OGLParameter;
struct PrimitiveGroup;
struct Material;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::RenderCmdBuffer
///	@ingroup	RenderSystem
///
///	@brief  This class is used to manage a render command buffer.
//-------------------------------------------------------------------------------------------------
class RenderCmdBuffer {
public:
    /// @brief  Describes the requested enqueue type.
    enum EnqueueType {
        RCE_Back    ///< Enqueue render command at the end 
    };

public:
    /// The class constructor.
    RenderCmdBuffer( OGLRenderBackend *pRenderBackend, Platform::AbstractRenderContext *ctx );
    /// The class destructor.
    virtual ~RenderCmdBuffer();
    /// Will set the active vertex array object.
    void setVertexArray( OGLVertexArray *pVertexArray );
    /// Will set the active shader.
    void setActiveShader( OGLShader *pOGLShader );
    ///
    OGLShader *getActiveShader() const;
    /// Will enqueue a new render command.
    void enqueueRenderCmd( OGLRenderCmd *pOGLRenderCmd, EnqueueType type = RCE_Back );
    /// The callback before rendering.
    bool onPreRenderFrame();
    /// The render callback.
    bool onRenderFrame( const Common::EventData *pEventData );
    /// The callback after rendering.
    bool onPostRenderFrame();
    /// The buffer and all attached commands will be cleared.
    void clear();

protected:
    virtual bool onUpdateParameter( const Common::EventData *pEventData );
    virtual bool onSetParametersCmd( SetParameterCmdData *pData );
    virtual bool onDrawPrimitivesCmd( DrawPrimitivesCmdData *pData );
    virtual bool onDrawPrimitivesInstancesCmd( DrawInstancePrimitivesCmdData *data );
    virtual bool onSetTextureStageCmd( SetTextureStageCmdData *pData );
    virtual bool onSetShaderStageCmd( SetShaderStageCmdData *pData );
    virtual bool onSetRenderTargetCmd( SetRenderTargetCmdData *pData );

private:
    OGLRenderBackend *m_pRenderBackend;
    Platform::AbstractRenderContext *m_pRenderCtx;
    CPPCore::TArray<OGLRenderCmd*> m_cmdbuffer;
    OGLShader *m_pActiveShader;
    OGLVertexArray *m_pVertexArray;
    CPPCore::TArray<PrimitiveGroup*> m_primitives;
    CPPCore::TArray<Material*> m_materials;
    OGLParameter *m_param;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
