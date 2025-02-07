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

#include "Common/AbstractEventHandler.h"
#include "Common/Event.h"
#include "RenderBackend/RenderBackendService.h"

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
class Material;

struct Vertex;
struct OGLVertexArray;
struct PrimitiveGroup;
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
///	@brief  This class is used to handle all incoming events for the render back-end, which
///         implements the OpenGL renderer.
//-------------------------------------------------------------------------------------------------
class OGLRenderEventHandler : public Common::AbstractEventHandler {
public:
    /// @brief The default class constructor.
    OGLRenderEventHandler();

    ///	@brief  The class destructor.
    ~OGLRenderEventHandler() override = default;

    /// @brief The OnEvent-callback.
    /// @param[in] ev           The event for handling.
    /// @param[in] pEventData   The event data.
    /// @return The result from the handler.
    bool onEvent( const Common::Event &ev, const Common::EventData *pEventData ) override;

    /// @brief Will set the active shader.
    /// @param[in] oglShader  The acitve shader.
    void setActiveShader( OGLShader *oglShader );
    
    /// @brief Will enqueue a new render command.
    /// @param[in] oglRenderCmd     The render command will enqued
    void enqueueRenderCmd( OGLRenderCmd *oglRenderCmd );

    /// @brief Will set all parameters.
    /// @param[in] paramArray   [in]    The array with the parameters.
    void setParameter(const cppcore::TArray<OGLParameter*> &paramArray);

    /// @brief  Will return the active render command buffer.
    /// @return Pointer showing to the render command buffer.
    RenderCmdBuffer *getRenderCmdBuffer() const;

    /// @brief	Will add meshed to a given mesh entry descrbed by its id.
    /// @param[in] id                The id of the mesh entry
    /// @param[in] primGroups        The primitive groups to add
    /// @param[in] currentMeshEntry  The mesh entry
    /// @return true if successful, false if not.
    bool addMeshes(const c8 *id, cppcore::TArray<size_t> &primGroups, MeshEntry *currentMeshEntry);

protected:
    /// @brief  Callback for attaching the event handler.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onAttached( const Common::EventData *eventData ) override;

    /// @brief  Callback for detaching the event handler.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onDetached( const Common::EventData *eventData ) override;

    /// @brief  Callback for render backend creation.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onCreateRenderer( const Common::EventData *eventData );

    /// @brief  Callback for render backend destroying.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onDestroyRenderer( const Common::EventData *eventData );

    /// @brief  Callback for attaching a new view onto a stage.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onAttachView( const Common::EventData *eventData );

    /// @brief  Callback for detaching a new view onto a stage.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onDetachView( const Common::EventData *eventData );

    /// @brief  Callback for clearing all geometry from a stage.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onClearGeo( const Common::EventData *eventData );

    /// @brief  Callback for the render frame.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onRenderFrame( const Common::EventData *eventData );
	
    /// @brief  Callback to init the passes.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onInitRenderPasses(const Common::EventData *eventData);
    
    /// @brief  Callback to commit the next frame.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onCommitNexFrame( const Common::EventData *eventData );
    
    /// @brief  Callback for dealing with a shutdown request.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onShutdownRequest( const Common::EventData *eventData );
    
    /// @brief  Callback for dealing with resize events.
    /// @param[in] eventData	The event state data
    /// @return true if successful, false if not.
    bool onResizeRenderTarget(const Common::EventData *eventData );

    /// @brief Will create a screenshot.
    /// @param[in] data  The event data with the filename.
    /// @return true if successful.
    bool onScreenshot(const Common::EventData *data);

    /// @brief Will handle a singke commit command.
    /// @param[in] cmd      The submit command to handle.
    void onHandleCommit(FrameSubmitCmd *cmd);

private:
    bool m_isRunning;
    OGLRenderBackend *m_oglBackend;
    RenderCmdBuffer *m_renderCmdBuffer;
    Platform::AbstractOGLRenderContext *m_renderCtx;
    OGLVertexArray *m_vertexArray;
    Pipeline *mActivePipeline;
};

inline RenderCmdBuffer *OGLRenderEventHandler::getRenderCmdBuffer() const {
    return m_renderCmdBuffer;
}

} // Namespace RenderBackend
} // Namespace OSRE
