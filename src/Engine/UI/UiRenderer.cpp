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

    CPPCore::TArray<Geometry*> geoCache;
    for ( ui32 i = 0; i < cache.size(); ++i ) {
        UiRenderCmd *currentCmd( cache[ i ] );
        Geometry *geo = UIRenderUtils::createGeoFromCache( currentCmd->m_vc, currentCmd->m_ic, currentCmd->m_mat );
        geoCache.add( geo );

        rbService->attachGeo( geoCache, 0 );
    }
}

}
}
