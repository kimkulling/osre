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
#include <osre/Scene/2D/Stage2D.h>
#include <osre/Scene/2D/Node2D.h>

namespace OSRE {
namespace Scene {

Stage2D::Stage2D(  const String &name, RenderBackend::RenderBackendService *rbService  )
: Stage( name, rbService ) {
    // empty
}

Stage2D::~Stage2D() {
    // empty
}

Node *Stage2D::addNode( const String &name, Node *parent, const String &type ) {
    if ( "default" == type ) {
        Common::Ids *ids = Stage::getIdContainer();
        if ( nullptr== ids ) {
            return nullptr;
        }
        return new Node2D( name, *ids, Node::RenderCompRequest::RenderCompRequested, Node::TransformCompRequest::TransformCompRequested, parent );
    } else {
        return Stage::addNode( name, parent, type );
    }
}


} // Namespace Scene
} // Namespace OSRE
