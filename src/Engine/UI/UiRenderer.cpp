/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/UI/UiRenderer.h>
#include <osre/UI/Screen.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include "UIRenderUtils.h"

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;

UiRenderer::UiRenderer() {

}

UiRenderer::~UiRenderer() {

}

void UiRenderer::render( UI::Screen *screen, RenderBackendService *rbService ) {
    UiRenderCmdCache cache;
    screen->render( cache, rbService );

    CPPCore::TArray<Mesh*> geoCache;
    for ( ui32 i = 0; i < cache.size(); ++i ) {
        UiRenderCmd *currentCmd( cache[ i ] );
        Mesh *geo = UIRenderUtils::createGeoFromCache( currentCmd->m_vc, currentCmd->m_ic, currentCmd->m_mat );
        geoCache.add( geo );

        rbService->addMesh( geoCache, 0 );
    }
}

}
}
