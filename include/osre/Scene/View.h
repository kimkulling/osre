/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Object.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace OSRE {
    
namespace RenderBackend {
    class RenderBackendService;
}

namespace Scene {

class Node;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class View : public Common::Object {
public:
    View( const String &name );
    virtual ~View();
    virtual void observeNode( Node *node );
    virtual void update( RenderBackend::RenderBackendService *renderBackendSrv );
    virtual void set( const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up );

private:
    Node *m_node;
    glm::vec3 m_pos, m_lookAt, m_up;
    glm::mat4 m_view, m_projection;
};

} // Namespace Scene
} // namespace OSRE
