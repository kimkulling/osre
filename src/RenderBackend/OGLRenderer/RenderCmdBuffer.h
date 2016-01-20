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

#include <cppcore/Container/TArray.h>

namespace OSRE {
    
namespace Common {
    struct EventData;
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
struct DrawTextCmdData;
struct OGLParameter;
struct PrimitiveGroup;
struct Material;

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::RenderBackend::RenderCmdBuffer
///	@ingroup	Engine
///
///	@brief  This class is used to manage a render command buffer. Render command buffers are used 
/// to store the list of render ops for rendering one single render frame.
//-------------------------------------------------------------------------------------------------
class RenderCmdBuffer {
public:
    /// @brief  Describes the requested enqueue type.
    enum EnqueueType {
        RCE_Back    ///< Enqueue render command at the end.
    };

public:
    /// The class constructor.
    RenderCmdBuffer( OGLRenderBackend *renderBackend, Platform::AbstractRenderContext *ctx );
    /// The class destructor.
    virtual ~RenderCmdBuffer();
    /// Will set the active vertex array object.
    void setVertexArray( OGLVertexArray *vertexArray );
    /// Will set the active shader.
    void setActiveShader( OGLShader *oglShader );
    /// Will return the active shader.
    OGLShader *getActiveShader() const;
    /// Will enqueue a new render command.
    void enqueueRenderCmd( OGLRenderCmd *renderCmd, EnqueueType type = RCE_Back );
    /// The callback before rendering.
    bool onPreRenderFrame();
    /// The render callback.
    bool onRenderFrame( const Common::EventData *eventData );
    /// The callback after rendering.
    bool onPostRenderFrame();
    /// The buffer and all attached commands will be cleared.
    void clear();

protected:
    /// The update parameter callback.
    virtual bool onUpdateParameter( const Common::EventData *pEventData );
    /// The update parameter callback.
    virtual bool onSetParametersCmd( SetParameterCmdData *pData );
    /// The draw primitive callback.
    virtual bool onDrawPrimitivesCmd( DrawPrimitivesCmdData *pData );
    /// The draw primitive instances callback.
    virtual bool onDrawPrimitivesInstancesCmd( DrawInstancePrimitivesCmdData *data );
    /// The set texture callback.
    virtual bool onSetTextureStageCmd( SetTextureStageCmdData *pData );
    /// The set shader stage callback.
    virtual bool onSetShaderStageCmd( SetShaderStageCmdData *pData );
    /// The set render target callback.
    virtual bool onSetRenderTargetCmd( SetRenderTargetCmdData *pData );

private:
    OGLRenderBackend *m_renderbackend;
    Platform::AbstractRenderContext *m_renderCtx;
    CPPCore::TArray<OGLRenderCmd*> m_cmdbuffer;
    OGLShader *m_activeShader;
    OGLVertexArray *m_vertexarray;
    CPPCore::TArray<PrimitiveGroup*> m_primitives;
    CPPCore::TArray<Material*> m_materials;
    OGLParameter *m_param;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
