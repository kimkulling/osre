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
///	@class		::OSRE::RenderBackend::RenderCmdBuffer
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
