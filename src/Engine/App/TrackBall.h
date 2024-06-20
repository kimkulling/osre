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

#include "App/CameraComponent.h"
#include "Platform/PlatformInterface.h"

namespace OSRE {
namespace App {

// Forward declarations ---------------------------------------------------------------------------
class CameraComponent;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	this class implements a simple trackball. You can use it to rotate a model.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TrackBall {
public:
    ///	@brief	The class constructor.
    ///	@param[in]	cameraComponent The name for the instance.
    ///	@param[in]	w               The width of the view-port to navigate in
    ///	@param[in]	h               The height of the view-port to navigate in
    TrackBall(CameraComponent *cameraComponent, ui32 w, ui32 h);

    ///	@brief	The class destructor.
    ~TrackBall() = default;

    ///	@brief	The event callback.
    ///	@param[in] osEvent  The incoming event from the operation system.
    ///	@param[in] data     The event data.
    void onOSEvent( const Common::Event &osEvent, const Common::EventData *data );
    
    void rotate(const glm::vec2 &from, glm::vec2 &to);
    void pan(f32 x, f32 y);

    ///	@brief	Maps a 2D-point to a sphere and returns the 3D-coordinate.
    ///	@param[in]	  pNewPt   The 2D-point to map.
    ///	@param[inout] NewVec   The mapped 3D-point.
    void mapToSphere(const glm::vec2 *pNewPt, glm::vec3 *NewVec);
    
    ///	@brief	The current rotation will be calculated.
    void computeRotation();

    ///	@brief	Calculates the current scaling.
    ///	@param[in] y   The current y value for the scaling.
    void zoom(ui32 y);
    const glm::quat &getRotation() const;
    glm::vec3 getScale() const;
    void reset();

private:
    glm::vec3 mStartVector, mEndVector;
    CameraComponent *mCameraComponent;
    TRectangle<ui32> m_Dimension;
    glm::quat m_rotation;
    bool m_bLeftMButtonClicked;
    bool m_bMiddleClicked;
    bool m_bRightMButtonClicked;
    bool m_bStartPosInited;
    f32 m_adjWidth;
    f32 m_adjHeight;
    ui32 m_screenY;
    ui32 m_screenYOld;
    f32 mRadius;
};

inline const glm::quat &TrackBall::getRotation() const {
    return m_rotation;
}

inline glm::vec3 TrackBall::getScale() const {
    return glm::vec3(mRadius, mRadius, mRadius);
}

} // namespace App
} // namespace OSRE
