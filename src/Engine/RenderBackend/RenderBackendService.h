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

#include "Common/AbstractService.h"
#include "Common/Event.h"
#include "RenderBackend/Pipeline.h"
#include "RenderBackend/RenderCommon.h"
#include "Threading/SystemTask.h"
#include "Common/glm_common.h"

#include <cppcore/Container/THashMap.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Platform {
    class AbstractWindow;
}

namespace Properties {
    class Settings;
}

namespace Threading {
    class SystemTask;
}

namespace RenderBackend {

class Mesh;

struct BufferData;
struct UniformVar;
struct TransformMatrixBlock;

// Event declarations
DECL_EVENT(OnScreenshotEvent);
DECL_EVENT(OnAttachEventHandlerEvent);
DECL_EVENT(OnDetatachEventHandlerEvent);
DECL_EVENT(OnCreateRendererEvent);
DECL_EVENT(OnDestroyRendererEvent);
DECL_EVENT(OnAttachViewEvent);
DECL_EVENT(OnDetachViewEvent);
DECL_EVENT(OnClearSceneEvent);
DECL_EVENT(OnDetachSceneEvent);
DECL_EVENT(OnSetRenderStates);
DECL_EVENT(OnRenderFrameEvent);
DECL_EVENT(OnSetParameterEvent);
DECL_EVENT(OnInitPassesEvent);
DECL_EVENT(OnCommitFrameEvent);
DECL_EVENT(OnShutdownRequestEvent);
DECL_EVENT(OnResizeEvent);

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  Will create a screenshot event.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT ScreenshotEventData : public Common::EventData {
    ScreenshotEventData(String filename, ui32 w, ui32 h) : EventData(OnScreenshotEvent, nullptr), 
            Filename(filename), Width(w), Height(h) {}
    
    String Filename;
    ui32 Width, Height;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  Will create an event, which shall trigger the generation of a render instance.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT CreateRendererEventData : public Common::EventData {
    CreateRendererEventData(Platform::AbstractWindow *pSurface) :
            EventData(OnCreateRendererEvent, nullptr), ActiveSurface(pSurface), DefaultFont(""), RequestedPipeline(nullptr) {
        // empty
    }

    Platform::AbstractWindow *ActiveSurface;
    String DefaultFont;
    Pipeline *RequestedPipeline;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This event will signal, that a new view shall be attached.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT AttachViewEventData : public Common::EventData {
    AttachViewEventData() :
            EventData(OnAttachViewEvent, nullptr) {
        // empty
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT InitPassesEventData : Common::EventData {
    InitPassesEventData() :
            EventData(OnInitPassesEvent, nullptr), 
            NextFrame(nullptr) {
        // empty
    }

    Frame *NextFrame;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT CommitFrameEventData : Common::EventData {
    CommitFrameEventData() :
            EventData(OnCommitFrameEvent, nullptr), 
            NextFrame(nullptr) {
        // empty
    }

    static CommitFrameEventData *create() {
        CommitFrameEventData *data = new CommitFrameEventData;
        return data;
    }

    Frame *NextFrame;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT ResizeEventData : Common::EventData {
    ResizeEventData(guid targetId_, ui32 x_, ui32 y_, ui32 w_, ui32 h_) :
            EventData(OnResizeEvent, nullptr),  targetId(targetId_), X(x_), Y(y_), W(w_), H(h_) {
        // empty
    }
    guid targetId;
    ui32 X, Y, W, H;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the render back-end service.
///
/// A render service implements the low-level API of the rendering. The API-calls will be performed
/// in its separate render system task. The render back-end handling is implemented by a
/// API-specific render event handler.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderBackendService : public Common::AbstractService {
public:
    /// @brief  The class constructor.
    RenderBackendService();

    /// @brief  The class destructor, virtual.
    ~RenderBackendService() override;

    /// @brief  Set the configuration for the render service.
    /// @param  config          [in] The render configuration.
    /// @param  moveOwnership   [in] true when ownership shall be moved.
    void setSettings(const Properties::Settings *config, bool moveOwnership);

    /// @brief  Returns the render configuration.
    /// @return The render configuration.
    const Properties::Settings *getSettings() const;

    /// @brief  Will send a new event to the render system task.
    /// @param  ev          [in] The event identifier.
    /// @param  eventData   [in] The event data.
    void sendEvent(const Common::Event *ev, const Common::EventData *eventData);

    /// @brief  Will create the default render pipeline.
    /// @return Pointer showing to the default render pipeline.
    Pipeline *createDefault3DPipeline(guid framebufferId);

    ///	@brief
    ///	@return 
    Pipeline *createDefault2DPipeline(guid framebufferId);

    ///	@brief
    /// @param
    void setActivePipeline(Pipeline *pipeline);

    ///	@brief
    /// @param
    ///	@return
    PassData *getPassById(const c8 *id) const;

    ///	@brief
    /// @param
    ///	@return
    PassData *beginPass(const c8 *id);

    ///	@brief
    /// @param
    ///	@return
    RenderBatchData *beginRenderBatch(const c8 *id);

    ///	@brief
    /// @param
    void setRenderTarget(FrameBuffer *fb);

    ///	@brief
    /// @param
    /// @param
    void setMatrix(MatrixType type, const glm::mat4 &m);

    ///	@brief
    /// @param
    /// @param
    void setMatrix(const String &name, const glm::mat4 &matrix);

    ///	@brief Will add a new uniform variable
    /// @param[in] uniformVar   The uniform variable.
    void addUniform(UniformVar *uniformVar);

    void setMatrixArray(const String &name, ui32 numMat, const glm::mat4 *matrixArray);

    void addMesh(Mesh *geo, ui32 numInstances);

    void addMesh(const MeshArray &meshArray, ui32 numInstances);

    void updateMesh(Mesh *mesh);

    bool endRenderBatch();

    bool endPass();

    void clearPasses();

    void attachView();

    void resize(guid targetId, ui32 x, ui32 y, ui32 w, ui32 h);

    void enableAutoResizing(bool enabled);

    void focusLost();

    void syncRenderThread();

protected:
    /// @brief  The open callback.
    bool onOpen() override;

    /// @brief  The close callback.
    bool onClose() override;

    /// @brief  The update callback.
    bool onUpdate() override;

    /// @brief  All render passes will be initialized
    void initPasses();

    /// @brief  Will apply all used parameters
    void commitNextFrame();

private:
    Threading::SystemTaskPtr mRenderTaskPtr;
    const Properties::Settings *mSettings;
    bool mOwnsSettingsConfig;
    bool mFrameCreated;
    Frame mFrames[2];
    Frame *mRenderFrame;
    Frame *mSubmitFrame;
    bool mDirty;
    TArray<PassData*> mPasses;
    Pipeline *mPipeline;
    PassData *mCurrentPass;
    RenderBatchData *mCurrentBatch;
    struct Behaviour {
        bool ResizeViewport;

        Behaviour() : ResizeViewport(true) {}
    } mBehaviour;
};

inline void RenderBackendService::enableAutoResizing(bool enabled) {
    mBehaviour.ResizeViewport = enabled;
}

} // Namespace RenderBackend
} // Namespace OSRE
