/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/RenderStates.h"
#include "RenderBackend/RenderPass.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

// Forward declarations ---------------------------------------------------------------------------
class Shader;
class RenderBackendService;

using cppcore::TArray;

/// @brief The pipeline access
namespace DefaultPipelines {
    OSRE_EXPORT const c8 *get3DPipelineDefault();
    OSRE_EXPORT const c8 *get2DPipelineDefault();
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This class is used to describes a pipeline used for rendering. A pipeline contains 
/// passes. Each frame which will be rendered by the pipeline walks through all the passes. So
/// a pipeline could look like:
/// MyPipeline:
///   1.) ClearPass ( will clear all the buffers)
///   2.) Render all static meshes
///   3.) Render special tiles for lightning
///   4.) UI-Rendering 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Pipeline : public Common::Object {
public:
    /// @brief  The class constructor.
    /// @param  name    The name for the pipeline.
    Pipeline(const String &name);
    
    ///	@brief  The class destructor.
    ~Pipeline();

    /// @brief  Will add a new render-pass to the pipeline.
    /// @param  pass    The pass to add.
    void addPass(RenderPass *pass);

    /// @brief  Will return the number of passes.
    /// @return The number of passes in the pipeline.
    size_t getNumPasses() const;

    /// @brief Will searc for a given render pass described by its id.
    /// @param passId The pass id to look for.
    /// @return The pass or a nullptr if the id was not assigned to a pass.
    RenderPass *getPassById(guid passId) const;

    /// @brief  Will begin the frame-rendering in the current pipeline.
    /// @return The number of render-passes within the pipeline.
    /// @note   Use this to loop over all render-passes.
    size_t beginFrame();

    /// @brief  Will begin the rendering for a given render-pass.
    /// @param  passId  The render-pass id.
    /// @return Pointer showing to the render-pass. Will be a nullptr when no render-pass exists.
    RenderPass *beginPass(guid passId);

    /// @brief  Will end the current render-pass.
    /// @param  passId  The render-pass id.
    /// @return true if successful, false in case of an error.
    bool endPass(guid passId);

    /// @brief  Will end the frame-rendering for the given pipeline.
    void endFrame();

    /// @brief  Will clear the pipeline, all assigned render-passes will be removed.
    void clear();

private:
    using PipelinePassArray = TArray<RenderPass*>;
    PipelinePassArray mPasses;
    RenderBackend::RenderBackendService *mRbService;
    guid mCurrentPassId;
    bool mInFrame;
};

} // Namespace RenderBackend
} // Namespace OSRE
