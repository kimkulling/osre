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
#include <osre/App/World.h>
#include <osre/App/Entity.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/StringUtils.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace App {

using namespace ::CPPCore;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

static const c8 *Tag = "World";

template<class T>
void lookupMapDeleterFunc( TArray<T> &ctr ) {
    for ( ui32 i = 0; i < ctr.size(); ++i ) {
        if ( nullptr != ctr[ i ] ) {
            ctr[ i ]->release();
            ctr[ i ] = nullptr;
        }
    }
    ctr.clear();
}

World::World( const String &worldName, RenderMode renderMode )
: Object( worldName )
, m_stages()
, m_lookupStates()
, m_views()
, m_lookupViews()
, m_entities()
, m_activeStage( nullptr )
, m_activeView( nullptr )
, m_ids()
, m_renderMode( renderMode ) {
    // empty
}

World::~World() {
    ContainerClear<TArray<View*> >( m_views, lookupMapDeleterFunc );
    m_lookupViews.clear();
    m_activeView = nullptr;

    ContainerClear<TArray<Stage*> >( m_stages, lookupMapDeleterFunc );
    m_lookupStates.clear();
    m_activeStage = nullptr;
}

void World::addStage( Stage *stage ) {
    if ( nullptr == stage ) {
        return;
    }

    const ui32 hash( StringUtils::hashName( stage->getName() ) );
    if ( m_lookupStates.hasKey( hash ) ) {
        return;
    }

    m_stages.add( stage );
    m_lookupStates.insert( hash, stage );
    stage->setIdContainer( m_ids );
}

Stage *World::setActiveStage( Stage *activeStage ) {
    if ( m_activeStage == activeStage ) {
        return nullptr;
    }

    m_activeStage = activeStage;
    if (nullptr == m_stages.find(activeStage)) {
        m_stages.add(activeStage);
    }

    return m_activeStage;
}

Stage *World::setActiveStage( const String &stageName ) {
    const ui32 hash( StringUtils::hashName( stageName.c_str() ) );
    if ( !m_lookupStates.hasKey( hash ) ) {
        return nullptr;
    }
    
    Stage *activeStage( nullptr );
    if ( m_lookupStates.getValue( hash, activeStage ) ) {
        return setActiveStage(activeStage);
    }

    return nullptr;
}

size_t World::getNumStages() const {
    return m_stages.size();
}

Stage *World::getStageAt(ui32 i) const {
    if (i >= m_stages.size()) {
        return nullptr;
    }
    return m_stages[i];
}

Stage *World::getActiveStage() const {
    return m_activeStage;
}

void World::addView( View *view ) {
    if ( nullptr == view ) {
        return;
    }

    m_activeView = view;
    const ui32 hash(StringUtils::hashName( view->getName() ) );
    m_lookupViews.insert( hash, view );
}

size_t World::getNumViews() const {
    return m_views.size();
}

View *World::getViewAt(ui32 i) const {
    if (i >= m_views.size()) {
        return nullptr;
    }

    return m_views[ i ];
}

View *World::setActiveView( View *activeView ) {
    if ( m_activeView == activeView ) {
        return nullptr;
    }
    
    m_activeView = activeView;
    if (nullptr == m_views.find(activeView)) {
        m_views.add(activeView);
    }

    return activeView;
}

View *World::setActiveView( const String &viewName ) {
    const ui32 hash(StringUtils::hashName( viewName ) );
    if ( !m_lookupViews.hasKey( hash ) ) {
        return nullptr;
    }

    View *activeView( nullptr );
    if ( m_lookupViews.getValue( hash, activeView ) ) {
        setActiveView(activeView);
        return activeView;
    }

    return nullptr;
}

View *World::getActiveView() const {
    return m_activeView;
}

void World::addEntity( Entity *entity ) {
    if (nullptr == entity) {
        osre_debug( Tag, "Pointer to entity are nullptr" );
        return;
    }

    m_entities.add( entity );
}

Entity *World::getEntityByName( const String &name ) const {
    if (name.empty()) {
        return nullptr;
    }

    for (size_t i = 0; i < m_entities.size(); ++i) {
        if (nullptr != m_entities[ i ]) {
            if (m_entities[ i ]->getName() == name) {
                return m_entities[ i ];
            }
        }
    }

    return nullptr;
}

void World::update( Time dt ) {
    if (nullptr != m_activeStage) {
        m_activeStage->update( dt );
    }

    if (nullptr != m_activeView) {
        m_activeView->update( dt );
    }

    for (ui32 i = 0; i < m_entities.size(); ++i) {
        Entity *entity = m_entities[ i ];
        if (nullptr == entity) {
            continue;
        }

        entity->update( dt );
    }
}

void World::draw( RenderBackendService *rbSrv ) {
    OSRE_ASSERT(nullptr != rbSrv);

    rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    if ( nullptr != m_activeStage ) {
        m_activeStage->draw( rbSrv );
    }

    if ( nullptr != m_activeView ) {
        m_activeView->draw( rbSrv );
    }

    for (ui32 i = 0; i < m_entities.size(); ++i) {
        Entity *entity = m_entities[ i ];
        if (nullptr == entity) {
            continue;
        }

        entity->render( rbSrv );
    }

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

RenderMode World::getRenderMode() const {
    return m_renderMode;
}

} // Namespace App
} // namespace OSRE
