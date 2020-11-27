/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Math/BaseMath.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Scene/TrackBall.h>
#include <osre/Scene/Node.h>

namespace OSRE {
namespace Scene {

using namespace OSRE::Common;
using namespace OSRE::Platform;

TrackBall::TrackBall(const String &trackBallObjName, ui32 w, ui32 h, Ids &ids) :
        Camera(trackBallObjName, ids),
        mStartVector(0, 0, 0),
        mEndVector(0, 0, 0),
        m_Dimension(w, h),
        m_rotation(),
        m_bLeftMButtonClicked(false),
        m_bMiddleClicked(false),
        m_bRightMButtonClicked(false),
        m_bStartPosInited(false),
        m_adjWidth(0.0f),
        m_adjHeight(0.0f),
        m_screenY(0),
        m_screenYOld(0),
        mRadius (1.0f) {
    // adjust the width for to sphere mapping
    const f32 width = static_cast<f32>(m_Dimension.getWidth());
    if (width) {
        m_adjWidth = 1.0f / (width - 1.0f);
    }

    // adjust the height for to sphere mapping
    const f32 height = static_cast<f32>(m_Dimension.getHeight());
    if (height) {
        m_adjHeight = 1.0f / (height - 1.0f);
    }
}

TrackBall::~TrackBall() {
    // empty
}

void TrackBall::rotate( const Vec2f &from, Vec2f &to ) {
    mapToSphere(&from, &mStartVector);
    mapToSphere(&to, &mEndVector);
    computeRotation();
}

void TrackBall::pan( f32 x, f32 y ) {
    glm::vec3 lookAt = normalize(getEye() - getCenter());
    glm::vec3 up = getUp();
    glm::vec3 right = glm::cross(lookAt, up);
    right *= x;
    up *= y;
}

void TrackBall::onOSEvent(const Common::Event &osEvent, const Common::EventData *data) {
    if (osEvent == Platform::MouseButtonDownEvent) {
        Platform::MouseButtonEventData *pMBData = (Platform::MouseButtonEventData *)data;
        if ( 0 == pMBData->m_Button ) {
            Vec2f pos( static_cast<f32>( pMBData->m_AbsX ), static_cast<f32>( pMBData->m_AbsY ) );
            mapToSphere( &pos, &mEndVector );
            m_bLeftMButtonClicked = true;
        } else if ( 1 == pMBData->m_Button ) {
            m_bMiddleClicked = true;
        } else {
            m_bRightMButtonClicked = true;
        }
    } else if ( osEvent == Platform::MouseMoveEvent ) {
        const MouseMoveEventData *pMMData = (Platform::MouseMoveEventData*) (data);
        if ( m_bLeftMButtonClicked ) {
            Vec2f pos(static_cast<f32>(pMMData->m_absX), static_cast<f32>(pMMData->m_absY));
            mStartVector = mEndVector;
            mapToSphere( &pos, &mEndVector );
            computeRotation();
        } else if ( m_bMiddleClicked ) {
            zoom( pMMData->m_absY );
        }
    } else if (osEvent == Platform::MouseButtonUpEvent) {
        m_screenYOld = 0;
        const MouseButtonEventData *pMBData = reinterpret_cast<const MouseButtonEventData*>( data );
        if ( 0 == pMBData->m_Button ) {
            m_bLeftMButtonClicked = false;
        } else if ( 1 == pMBData->m_Button ) {
            m_bMiddleClicked = false;
        } else {
            m_bRightMButtonClicked = false;
        }
    }
}

void TrackBall::mapToSphere(const Vec2f *pNewPt, Vec3f *newVector) {
    // copy parameter into temp point
    Vec2f tempPt(*pNewPt);

    // adjust point coordinates and scale down to range of [-1 ... 1]
    f32 x = (tempPt.getX() * m_adjWidth) - 1.0f;
    f32 y = tempPt.getY() * m_adjHeight;
    tempPt.set(x, y);

    // compute the square of the length of the vector to the point from the center
    f32 length = (tempPt.getX() * tempPt.getX()) + (tempPt.getY() * tempPt.getY());

    // if the point is mapped outside of the sphere... (length > radius squared)
    if (length > mRadius) {
        // compute a normalizing factor (radius / sqrt(length))
        f32 norm = mRadius / sqrt(length);

        // return the "normalized" vector, a point on the sphere
        newVector->set(tempPt.getX() * norm, tempPt.getY() * norm, 0.0f);
    } else { // else it's on the inside
        // return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        newVector->set(tempPt.getX(), tempPt.getY(), sqrt(mRadius - length));
    }
}

void TrackBall::computeRotation() {
    Vec3f perp = mStartVector.crossProduct(mEndVector);
    if (perp.getLength() > Math::BaseMath::getSPEPS()) {
        m_rotation.x = perp.getX();
        m_rotation.y = perp.getX();
        m_rotation.z = perp.getZ();
        m_rotation.w = mStartVector.dotProduct(mEndVector);
    } else {
        m_rotation.x = 0;
        m_rotation.y = 0;
        m_rotation.z = 0;
        m_rotation.w = 1;
    }
}

void TrackBall::zoom(ui32 y) {
    m_screenYOld = m_screenY;
    m_screenY = y;
    const f32 offset = 0.00005f;
    if (m_screenYOld) {
        const i32 diff = m_screenY - m_screenYOld;
        mRadius += offset * static_cast<f32>(diff);
    }
}

void TrackBall::reset() {
    m_screenYOld = 0;
}


} // namespace Scene
} // namespace OSRE
