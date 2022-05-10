/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/TAABB.h>
#include <osre/Common/BaseMath.h>

#include <GL/glew.h>
#include <GL/gl.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class offers some system-specific functions.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT ParticleEmitter {
public:
    ParticleEmitter( RenderBackend::RenderBackendService *rbSrv );
    ~ParticleEmitter();
    void init( ui32 numPoints );
    void update( d32 tick );
    void setBounds(const AABB& bounds);
    RenderBackend::Mesh* getMesh() const;

private:
    RenderBackend::RenderBackendService *m_rbSrv;
    ui32 m_numPoints;
    glm::vec3 *m_col;
    glm::vec3 *m_pos;
    GLushort *m_pt_indices;
    RenderBackend::Mesh *m_ptGeo;
    bool mUseBounds;
    AABB mBounds;
};

} // Namespace RenderBackend
} // Namespace OSRE
