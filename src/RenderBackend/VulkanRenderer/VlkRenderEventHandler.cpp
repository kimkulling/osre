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
#include "VlkRenderEventHandler.h"
#include "VlkRenderBackend.h"

#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

VlkRenderEventHandler::VlkRenderEventHandler()
: AbstractEventHandler()
, m_vlkBackend( nullptr ) {
    // empty
}

VlkRenderEventHandler::~VlkRenderEventHandler() {
    // empty
}

bool VlkRenderEventHandler::onEvent( const Event &ev, const EventData *data ) {
    bool result( false );
    if ( OnAttachEventHandlerEvent == ev ) {
        result = onAttached( data );
    } else if ( OnDetatachEventHandlerEvent == ev ) {
        result = onDetached( data );
    } else if ( OnCreateRendererEvent == ev ) {
        result = onCreateRenderer( data );
    } else if ( OnDestroyRendererEvent == ev ) {
        result = onDestroyRenderer( data );
    } else if ( OnAttachViewEvent == ev ) {
        result = onAttachView( data );
    } else if ( OnDetachViewEvent == ev ) {
        result = onDetachView( data );
    } else if ( OnAttachSceneEvent == ev ) {
        result = onAttachGeo( data );
    } else if ( OnDetachSceneEvent == ev ) {
        result = onDetachGeo( data );
    } else if ( OnClearSceneEvent == ev ) {
        result = onClearGeo( data );
    } else if ( OnRenderFrameEvent == ev ) {
        result = onRenderFrame( data );
    } else if ( OnUpdateParameterEvent == ev ) {
        result = onUpdateParameter( data );
    } else if ( OnRenderTextEvent == ev ) {
        result = onRenderText( data );
    }

    return result;
}
 
bool VlkRenderEventHandler::onAttached( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onDetached( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onCreateRenderer( const Common::EventData *eventData ) {
    m_vlkBackend = new VlkRenderBackend;
    AbstractSurface *surface( PlatformInterface::getInstance()->getRootSurface() );
    if ( nullptr == surface ) {
        return false;
    }
    
    if ( !m_vlkBackend->create( surface ) ) {
        return false;
    }

    return true;
}

bool VlkRenderEventHandler::onDestroyRenderer( const Common::EventData *eventData ) {
    delete m_vlkBackend;
    m_vlkBackend = nullptr;

    return true;
}

bool VlkRenderEventHandler::onAttachView( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onDetachView( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onAttachGeo( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onDetachGeo( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onClearGeo( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onRenderFrame( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onUpdateParameter( const Common::EventData *eventData ) {
    return true;
}

bool VlkRenderEventHandler::onRenderText( const Common::EventData *eventData ) {
    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
