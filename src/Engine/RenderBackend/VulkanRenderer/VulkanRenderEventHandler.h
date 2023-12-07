#pragma once

namespace OSRE {
namespace RenderBackend {

class Pipeline;

//-------------------------------------------------------------------------------------------------
///	@brief  This class is used to handle all incoming events for the render back-end.
//-------------------------------------------------------------------------------------------------
class VulkanRenderEventHandler : public Common::AbstractEventHandler {
public:
    /// @brief The default class constructor.
    VulkanRenderEventHandler();

    ///	@brief  The class destructor.
    ~VulkanRenderEventHandler() override = default;

    /// @brief The OnEvent-callback.
    /// @param ev           The event for handling.
    /// @param pEventData   The event data.
    /// @return The result from the handler.
    bool onEvent( const Common::Event &ev, const Common::EventData *pEventData ) override;

protected:
    /// @brief  Callback for attaching the event handler.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onAttached( const Common::EventData *eventData ) override;

    /// @brief  Callback for detaching the event handler.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onDetached( const Common::EventData *eventData ) override;

    /// @brief  Callback for render backend creation.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onCreateRenderer( const Common::EventData *eventData );

    /// @brief  Callback for render backend destroying.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onDestroyRenderer( const Common::EventData *eventData );

    /// @brief  Callback for attaching a new view onto a stage.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onAttachView( const Common::EventData *eventData );

    /// @brief  Callback for detaching a new view onto a stage.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onDetachView( const Common::EventData *eventData );

    /// @brief  Callback for clearing all geometry from a stage.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onClearGeo( const Common::EventData *eventData );

    /// @brief  Callback for the render frame.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onRenderFrame( const Common::EventData *eventData );

    /// @brief  Callback to init the passes.
    /// @param eventData	The event state data
    /// @return true if successful, false if not.
    bool onInitRenderPasses(const Common::EventData *eventData);

    /// @brief  Callback to commit the next frame.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onCommitNexFrame( const Common::EventData *eventData );

    /// @brief  Callback for dealing with a shutdown request.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onShutdownRequest( const Common::EventData *eventData );

    /// @brief  Callback for dealing with resize events.
    /// @param  eventData	The event state data
    /// @return true if successful, false if not.
    bool onResizeRenderTarget(const Common::EventData *eventData );

private:
    bool mIsRunning;
    //OGLRenderBackend *m_oglBackend;
    //RenderCmdBuffer *m_renderCmdBuffer;
    //Platform::AbstractOGLRenderContext *m_renderCtx;
    Pipeline *mPipeline;
};

}
}
