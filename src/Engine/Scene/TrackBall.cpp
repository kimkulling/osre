/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/TrackBall.h>
#include <osre/Common/Logger.h>
#include <osre/Math/BaseMath.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace Scene {

using namespace OSRE::Common;
using namespace OSRE::Platform;

TrackBall::TrackBall( const String &trackBallObjName, ui32 w, ui32 h ) 
: OSEventListener( trackBallObjName )
, m_StartVector( 0,0,0 )
, m_EndVector( 0,0,0 )
, m_Dimension( w, h )
, m_rotation()
, m_bLeftMButtonClicked( false )
, m_bMiddleClicked( false )
, m_bRightMButtonClicked( false )
, m_bStartPosInited( false )
, m_adjWidth( 0.0f )
, m_adjHeight( 0.0f )
, m_screenY( 0 )
, m_screenYOld( 0 ) {
    // adjust the width for to sphere mapping
    const f32 width = static_cast<f32>( m_Dimension.getWidth() );
    if ( width ) {
        m_adjWidth = 1.0f / ( width - 1.0f );
    }

    // adjust the height for to sphere mapping
    const f32 height = static_cast<f32>( m_Dimension.getHeight() );
    if ( height ) {
        m_adjHeight = 1.0f / ( height - 1.0f );
    }
}

TrackBall::~TrackBall() {
    // empty
}

void TrackBall::onOSEvent( const Common::Event &rOSEvent, const Common::EventData *pData ) {
/*	if ( rOSEvent == Interface::MouseButtonDownEvent ) {
        Interface::MouseButtonEventData *pMBData = (Interface::MouseButtonEventData*) pData;
        if ( 0 == pMBData->m_Button ) {
            Math::Vector2f pos( static_cast<f32>( pMBData->m_AbsX ), static_cast<f32>( pMBData->m_AbsY ) );
            mapToSphere( &pos, &m_EndVector );
            m_bLeftMButtonClicked = true;
        } else if ( 1 == pMBData->m_Button ) {
            m_bMiddleClicked = true;
        } else {
            m_bRightMButtonClicked = true;
        }
    } else if ( rOSEvent == Interface::MouseMoveEvent ) {
        const MouseMoveEventData *pMMData = reinterpret_cast<const MouseMoveEventData*>( pData );
        if ( m_bLeftMButtonClicked ) {
            Math::Vector2f pos( static_cast<f32>( pMMData->m_AbsX ), static_cast<f32>( pMMData->m_AbsY ) );
            m_StartVector = m_EndVector;
            mapToSphere( &pos, &m_EndVector );
            computeRotation();
        } else if ( m_bMiddleClicked ) {
            computeScaling( pMMData->m_AbsY );
        }
    } else if (rOSEvent == Interface::MouseButtonUpEvent ) {
        m_nodePtr->setScale( ZFXCE2::Math::Vector3f( 1,1,1 ) );
        m_nodePtr->setRotation( ZFXCE2::Math::Quaternionf( 0,0,0,1 ) );
        m_screenYOld = 0;
        const MouseButtonEventData *pMBData = reinterpret_cast<const MouseButtonEventData*>( pData );
        if ( 0 == pMBData->m_Button ) {
            m_bLeftMButtonClicked = false;
        } else if ( 1 == pMBData->m_Button ) {
            m_bMiddleClicked = false;
        } else {
            m_bRightMButtonClicked = false;
        }
    }*/
}

void TrackBall::mapToSphere( const Vec2f *pNewPt, Vec3f *NewVec ) {
    // copy parameter into temp point
    Vec2f tempPt( *pNewPt );

    // adjust point coordinates and scale down to range of [-1 ... 1]
    f32 x = ( tempPt.getX() * m_adjWidth )  - 1.0f;
    f32 y = /*1.0f -*/ ( tempPt.getY() * m_adjHeight );
    tempPt.set( x, y );

    // compute the square of the length of the vector to the point from the center
    f32 length = ( tempPt.getX() * tempPt.getX()) + (tempPt.getY() * tempPt.getY() );

    // if the point is mapped outside of the sphere... (length > radius squared)
    if ( length > 1.0f ) {
        // compute a normalizing factor (radius / sqrt(length))
        f32 norm = 1.0f / sqrt( length );

        // return the "normalized" vector, a point on the sphere
        NewVec->set( tempPt.getX() * norm, tempPt.getY() * norm, 0.0f );
    } else  {  // else it's on the inside
        // return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        NewVec->set( tempPt.getX(), tempPt.getY(), sqrt( 1.0f - length ) );
    }
}

void TrackBall::computeRotation() {
    Vec3f perp = m_StartVector.crossProduct( m_EndVector );;
    //perp.crossProduct( m_StartVector, m_EndVector );
    
    if ( perp.getLength() > Math::BaseMath::getSPEPS() ) {
        m_rotation.set( perp.getX(), perp.getY(), perp.getZ(), m_StartVector.dotProduct( m_EndVector ) );
    } else {
        m_rotation.set( 0.0f, 0.0f, 0.0f, 1.0f );
    }

    //m_nodePtr->setRotation( m_Rotation );
}

void TrackBall::computeScaling( ui32 y ) {
    m_screenYOld = m_screenY;
    m_screenY = y;
    static const f32 offset = 0.001f;
    if ( m_screenYOld ) {
        i32 diff = m_screenY - m_screenYOld;
        const f32 scaleFactor = offset * (f32) diff;
        Vec3f scale/* = m_nodePtr->getScale()*/;
        scale += scaleFactor;				
        if ( scale.isZero() ) {
            scale.set( 0,0,0 );
        }
        /*m_nodePtr->setScale( scale )*/;
    }
}


} // Namespace SceneTools
} // namespace OSRE
