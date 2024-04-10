/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <cppcore/Container/THashMap.h>
#include <osre/Common/AbstractService.h>
#include <osre/Common/Event.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Threading/SystemTask.h>
#include <osre/Common/glm_common.h>

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
struct GeoInstanceData;
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
            mFilename(filename), mWidth(w), mHeight(h) {}
    String mFilename;
    ui32 mWidth, mHeight;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  Will create an event, which shall trigger the generation of a render instance.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT CreateRendererEventData : public Common::EventData {
    CreateRendererEventData(Platform::AbstractWindow *pSurface) :
            EventData(OnCreateRendererEvent, nullptr), m_activeSurface(pSurface), m_defaultFont(""), m_pipeline(nullptr) {
        // empty
    }

    Platform::AbstractWindow *m_activeSurface;
    String m_defaultFont;
    Pipeline *m_pipeline;
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
            m_frame(nullptr) {
        // empty
    }

    Frame *m_frame;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT CommitFrameEventData : Common::EventData {
    CommitFrameEventData() :
            EventData(OnCommitFrameEvent, nullptr), 
            m_frame(nullptr) {
        // empty
    }

    ~CommitFrameEventData() = default;

    static CommitFrameEventData *create() {
        CommitFrameEventData *data = new CommitFrameEventData;
        return data;
    }

    Frame *m_frame;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT ResizeEventData : Common::EventData {
    ResizeEventData(ui32 x, ui32 y, ui32 w, ui32 h) :
            EventData(OnResizeEvent, nullptr), 
            m_x(x), 
            m_y(y), 
            m_w(w), 
            m_h(h) {
        // empty
    }
    ui32 m_x, m_y, m_w, m_h;
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
    virtual ~RenderBackendService();

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
    Pipeline *createDefaultPipeline();

    void setActivePipeline(Pipeline *pipeline);

    PassData *getPassById(const c8 *id) const;

    PassData *beginPass(const c8 *id);

    RenderBatchData *beginRenderBatch(const c8 *id);

    void setRenderTarget(FrameBuffer *fb);

    void setMatrix(MatrixType type, const glm::mat4 &m);

    void setMatrix(const String &name, const glm::mat4 &matrix);

    void setUniform(UniformVar *var);

    void setMatrixArray(const String &name, ui32 numMat, const glm::mat4 *matrixArray);

    void addMesh(Mesh *geo, ui32 numInstances);

    void addMesh(const cppcore::TArray<Mesh *> &geoArray, ui32 numInstances);

    void updateMesh(Mesh *mesh);

    bool endRenderBatch();

    bool endPass();

    void clearPasses();

    void attachView();

    void resize(ui32 w, ui32 h);

    void enableAutoResizing(bool enabled);

    void focusLost();

    void syncRenderThread();

    void setViewport(ui32 x, ui32 y, ui32 w, ui32 h);

    const Viewport &getViewport() const;

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
    Viewport mViewport;
    bool mOwnsSettingsConfig;
    bool mFrameCreated;
    Frame mFrames[2];
    Frame *mRenderFrame;
    Frame *mSubmitFrame;
    bool mDirty;
    cppcore::TArray<PassData*> mPasses;
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
