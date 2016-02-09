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
#include <osre/Scene/Component.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace Scene {
    
using namespace OSRE::RenderBackend;

Component::Component() {

}

Component::~Component() {

}

RenderComponent::RenderComponent()
: Component()
, m_newGeo() {

}

RenderComponent::~RenderComponent() {

}

void RenderComponent::update( RenderBackendService *renderBackendSrv ) {
    if ( !m_newGeo.isEmpty() ) {
        renderBackendSrv->sendEvent( &OnAttachViewEvent, nullptr );
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;
        attachGeoEvData->m_numGeo = m_newGeo.size();
        attachGeoEvData->m_geo = m_newGeo[ 0 ];
        renderBackendSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );
        m_newGeo.resize( 0 );
    }
}

void RenderComponent::addStaticGeometry( RenderBackend::StaticGeometry *geo ) {
    if ( nullptr == geo ) {
        return;
    }

    m_newGeo.add( geo );
}

} // Namespace Scene
} // namespace OSRE
