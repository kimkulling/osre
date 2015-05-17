#pragma once

#include <osre/Common/TObjPtr.h>
#include <osre/Common/TArea.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Math/TVector2.h>
#include <osre/Math/TVector3.h>
#include <osre/Math/BaseMath.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Scene::TrackBall
///	@ingroup	Application
///
///	@brief	this class implements a simple trackball. You can use it to rotate a model.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TrackBall : public Platform::OSEventListener {
public:
    ///	@brief	The class constructor.
    ///	@param	objName		[in] The name for the instance.
    ///	@param	w			[in] The width of the view-port to navigate in
    ///	@param	h			[in] The height of the view-port to navigate in
    TrackBall( const String &objName, ui32 w, ui32 h );

    ///	@brief	The class destructor.
    ~TrackBall();

    ///	@brief	The event callback.
    ///	@param	osEvent		[in] The incoming event from the operation system.
    ///	@param	pData		[in] The event data.
    void onOSEvent( const Common::Event &osEvent, const Common::EventData *pData );
    
    ///	@brief	Maps a 2D-point to a sphere and returns the 3D-coordinate.
    ///	@param	pNewPt		[in] The 2D-point to map.
    ///	@param	NewVec		[out] The mapped 3D-point.
    void mapToSphere( const Math::Vector2f *pNewPt, Math::Vector3f *NewVec );
    
    ///	@brief	The current rotation will be calculated.
    void computeRotation();

    ///	@brief	Calculates the current scaling.
    ///	@param	y			[in] The current y value for the scaling.
    void computeScaling( ui32 y );

private:
    Math::Vector3f m_StartVector, m_EndVector;
    Common::TArea<ui32> m_Dimension;
    Math::Quatf m_rotation;
    //Math::Quaternionf m_Rotation;
    bool m_bLeftMButtonClicked;
    bool m_bMiddleClicked;
    bool m_bRightMButtonClicked;
    bool m_bStartPosInited;
    f32 m_adjWidth;
    f32 m_adjHeight;
    ui32 m_screenY;
    ui32 m_screenYOld;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Scene
} // namespace OSRE
