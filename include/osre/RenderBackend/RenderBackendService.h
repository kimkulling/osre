#pragma once

#include <osre/Common/AbstractService.h>
#include <osre/Common/Event.h>
#include <osre/Common/TObjPtr.h>

namespace OSRE {

namespace Platform {
    class AbstractSurface;
}

namespace Properties {
    class ConfigurationMap;
}

namespace Threading {
    class SystemTask;
}

namespace RenderBackend {

struct BufferData;
struct Geometry;
struct GeoInstanceData;
struct Parameter;

DECL_EVENT( OnAttachEventHandlerEvent );
DECL_EVENT( OnDetatachEventHandlerEvent );
DECL_EVENT( OnCreateRendererEvent );
DECL_EVENT( OnDestroyRendererEvent );
DECL_EVENT( OnAttachViewEvent );
DECL_EVENT( OnDetachViewEvent );
DECL_EVENT( OnAttachSceneEvent );
DECL_EVENT( OnClearSceneEvent );
DECL_EVENT( OnDetachSceneEvent );
DECL_EVENT( OnRenderFrameEvent );
DECL_EVENT( OnUpdateParameterEvent );

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::CreateRendererEventData
///	@ingroup	RenderSystem
///
///	@brief
//-------------------------------------------------------------------------------------------------
class CreateRendererEventData : public Common::EventData {
public:
    CreateRendererEventData( Platform::AbstractSurface *pSurface )
    : EventData( OnCreateRendererEvent, nullptr )
    , m_pActiveSurface( pSurface ) {
        // empty
    }

    Platform::AbstractSurface *m_pActiveSurface;
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::AttachViewEventData
///	@ingroup	RenderSystem
///
///	@brief
//-------------------------------------------------------------------------------------------------
class AttachViewEventData : public Common::EventData {
public:
    AttachViewEventData()
    : EventData( OnAttachViewEvent, nullptr ) {
        // empty
    }
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::AttachSceneEventData
///	@ingroup	RenderSystem
///
///	@brief
//-------------------------------------------------------------------------------------------------
class AttachGeoEventData : public Common::EventData {
public:
    AttachGeoEventData()
    : EventData( OnAttachSceneEvent, nullptr )
    , m_numGeo( 0 )
    , m_pGeometry( nullptr )
    , m_numInstances( 0 )
    , m_geoInstanceData( nullptr ) {
        // empty
    }

    ui32 m_numGeo;
    Geometry *m_pGeometry;
    ui32 m_numInstances;
    GeoInstanceData *m_geoInstanceData;
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::UpdateParameterEventData
///	@ingroup	RenderSystem
///
///	@brief
//-------------------------------------------------------------------------------------------------
class UpdateParameterEventData : public Common::EventData {
public:
    UpdateParameterEventData()
    : EventData( OnUpdateParameterEvent, nullptr )
    , m_numParam( 0 )
    , m_param( nullptr ) {
        // empty
    }

    ui32 m_numParam;
    Parameter *m_param;
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::RenderBackendServer
///	@ingroup	RenderSystem
///
///	@brief  This class implements the render back-end service.
///
/// A render service implements the low-level API of the rendering. The API-calls will be performed 
/// in its separate render system task. The render back-end handling is implemented by a 
/// API-specific render event handler.
//-------------------------------------------------------------------------------------------------
class RenderBackendService : public Common::AbstractService {
public:
    /// @brief  The class constructor.
    RenderBackendService();

    /// @brief  The class destructor, virtual.
    virtual ~RenderBackendService();

    /// @brief  Set the configuration for the render service.
    /// @param  pConfiguration  [in] The render configuration.
    void setConfig( const Properties::ConfigurationMap *pConfiguration );
    
    /// @brief  Returns the render configuration.
    /// @return The render configuration.
    const Properties::ConfigurationMap *getConfig() const;
    
    /// @brief  Will send a new event to the render system task.
    /// @param  pEvent      [in] The event identifier.
    /// @param  pEventData  [in] The event data.
    void sendEvent( const Common::Event *pEvent, const Common::EventData *pEventData );

protected:
    /// @brief  The open callback.
    virtual bool onOpen();

    /// @brief  The close callback.
    virtual bool onClose();
    
    /// @brief  The update callback.
    virtual bool onUpdate( d32 timediff );

private:
    Common::TObjPtr<Threading::SystemTask> m_RenderTaskPtr;
    const Properties::ConfigurationMap *m_pConfigMap;
    bool m_ownConfig;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
