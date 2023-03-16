/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/BaseMath.h>
#include <osre/RenderBackend/RenderStates.h>

#include <map>

namespace OSRE {

// Forward declarations
namespace Common {
struct EventData;
}

namespace Platform {
class AbstractOGLRenderContext;
}

namespace RenderBackend {

class OGLRenderBackend;
class OGLShader;
class Pipeline;
class Material;

struct OGLVertexArray;
struct OGLRenderCmd;
struct DrawPrimitivesCmdData;
struct DrawInstancePrimitivesCmdData;
struct DrawPanelsCmdData;
struct SetMaterialStageCmdData;
struct SetRenderTargetCmdData;
struct DrawTextCmdData;
struct PrimitiveGroup;
struct OGLParameter;

//-------------------------------------------------------------------------------------------------
/// @ingroup	Engine
///
/// @brief  This class is used to manage a render command buffer. Render command buffers are used
/// to store the list of render ops for rendering one single render frame.
//-------------------------------------------------------------------------------------------------
class RenderCmdBuffer {
public:
    /// @brief The class constructor.
    /// @param renderBackend    Pointer showing to the render backend.
    /// @param ctx              The render context.
    RenderCmdBuffer(OGLRenderBackend *renderBackend, Platform::AbstractOGLRenderContext *ctx);

    /// @brief  The class destructor.
    ~RenderCmdBuffer();
    
    /// @brief  Will set the active shader.
    /// @param  activeShader    The active shader.
    void setActiveShader(OGLShader *activeShader);
    
    /// @brief  Will return the active shader.
    /// @return The active shader, equal nullptr if none.
    OGLShader *getActiveShader() const;
    
    /// @brief Will enqueue a new render command.
    /// @param renderCmd    The render command to enqueue.
    void enqueueRenderCmd(OGLRenderCmd *renderCmd);

    /// @brief Will enqueue a new render command group.
    /// @param groupName    The group name
    /// @param cmdGroup     The command group.
    void enqueueRenderCmdGroup(const String &groupName, cppcore::TArray<OGLRenderCmd *> &cmdGroup);
    
    /// @brief  The callback before rendering.
    /// @param  pipeline         The pipeline to use.
    void onPreRenderFrame(Pipeline *pipeline);

    /// @brief  The render callback.
    void onRenderFrame();
    
    /// @brief  The callback after rendering.
    void onPostRenderFrame();

    /// @brief  The buffer and all attached commands will be cleared.
    void clear();

    /// @brief Will add one parameter to the setup of the pipeline
    void setParameter(OGLParameter *param);
    
    /// @brief Will add an array of parameters to the setup of the pipeline
    /// @param paramArray   The array with the assigned parameters.
    void setParameter(const ::cppcore::TArray<OGLParameter *> &paramArray);
    
    /// @brief Commits all assigned parameters to the active shader.
    void commitParameters();

    /// @brief  Will assign the default matrices.
    /// @param model    The model matrix.
    /// @param view     The view matrix
    /// @param proj     The projection matrix.
    void setMatrixes(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
    
    ///	@brief  Will assign a matrix buffer.
    /// @param  id      The matrix buffer id
    /// @param  buffer  The matrix buffer itself.
    void setMatrixBuffer(const c8 *id, MatrixBuffer *buffer);

protected:
    /// The render primitive callback.
    virtual bool onDrawPrimitivesCmd(DrawPrimitivesCmdData *data);
    /// The render primitive instances callback.
    virtual bool onDrawPrimitivesInstancesCmd(DrawInstancePrimitivesCmdData *data);
    /// The set render target callback.
    virtual bool onSetRenderTargetCmd(SetRenderTargetCmdData *data);
    /// The set material callback.
    virtual bool onSetMaterialStageCmd(SetMaterialStageCmdData *data);

private:
    OGLRenderBackend *mRBService;
    ClearState mClearState;
    Platform::AbstractOGLRenderContext *mRenderCtx;
    ::cppcore::TArray<OGLRenderCmd *> mCommandQueue;
    OGLShader *mActiveShader;
    ::cppcore::TArray<PrimitiveGroup *> mPrimitives;
    ::cppcore::TArray<Material *> mMaterials;
    ::cppcore::TArray<OGLParameter *> mParamArray;
    std::map<const char *, MatrixBuffer *> mMatrixBuffer;
    glm::mat4 mModel;
    glm::mat4 mView;
    glm::mat4 mProj;
    Pipeline *mPipeline;
};

} // Namespace RenderBackend
} // Namespace OSRE
