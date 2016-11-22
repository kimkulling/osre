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
#include <osre/Scene/World.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/Common/StringUtils.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Common/Ids.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Scene {

using namespace ::CPPCore;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

template<class T>
void lookupMapDeleterFunc( TArray<T> &ctr ) {
    for ( ui32 i = 0; i < ctr.size(); ++i ) {
        if ( nullptr != ctr[ i ] ) {
            ctr[ i ]->release();
        }
    }
    ctr.clear();
}

struct World::Impl {
    TArray<Stage*> m_stages;
    THashMap<ui32, Stage*> m_lookupStates;
    TArray<View*> m_views;
    THashMap<ui32, View*> m_lookupViews;
    Stage *m_activeStage;
    View *m_activeView;
    Ids m_ids;
    RenderMode m_renderMode;

    Impl( RenderMode renderMode )
    : m_stages()
    , m_lookupStates()
    , m_views()
    , m_lookupViews()
    , m_activeStage( nullptr )
    , m_activeView( nullptr )
    , m_ids()
    , m_renderMode( renderMode ) {
        // empty
    }

    ~Impl() {
        ContainerClear<TArray<View*> >( m_views, lookupMapDeleterFunc );
        m_lookupViews.clear();
        m_activeView = nullptr;

        ContainerClear<TArray<Stage*> >( m_stages, lookupMapDeleterFunc );
        m_lookupStates.clear();
        m_activeStage = nullptr;
    }
};

static ui32 calcHash( const String &name ) {
    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    return hash;
}

World::World( const String &worldName, RenderMode renderMode )
: Object( worldName )
, m_impl( nullptr ) {
    m_impl = new Impl( renderMode );
}

World::~World() {
    delete m_impl;
    m_impl = nullptr;
}

void World::addStage( Stage *stage ) {
    OSRE_ASSERT( nullptr != m_impl );

    if ( nullptr == stage ) {
        return;
    }

    const ui32 hash( calcHash( stage->getName() ) );
    if ( m_impl->m_lookupStates.hasKey( hash ) ) {
        return;
    }

    m_impl->m_stages.add( stage );
    m_impl->m_lookupStates.insert( hash, stage );
    stage->setIdContainer( m_impl->m_ids );
}

bool World::setActiveStage( Stage *activeStage ) {
    OSRE_ASSERT( nullptr != m_impl );

    if ( m_impl->m_activeStage == activeStage ) {
        return false;
    }

    m_impl->m_activeStage = activeStage;

    return true;
}

bool World::setActiveStage( const String &stageName ) {
    OSRE_ASSERT( nullptr != m_impl );

    const ui32 hash( StringUtils::hashName( stageName.c_str() ) );
    if ( !m_impl->m_lookupStates.hasKey( hash ) ) {
        return false;
    }
    
    Stage *activeStage( nullptr );
    if ( m_impl->m_lookupStates.getValue( hash, activeStage ) ) {
        m_impl->m_activeStage = activeStage;
        return true;
    }

    return false;
}

void World::addView( View *view ) {
    if ( nullptr == view ) {
        return;
    }

    m_impl->m_activeView = view;
    const ui32 hash( calcHash( view->getName() ) );
    m_impl->m_lookupViews.insert( hash, view );
}

bool World::setActiveView( View *activeView ) {
    OSRE_ASSERT( nullptr != m_impl );

    if ( m_impl->m_activeView == activeView ) {
        return true;
    }
    
    m_impl->m_activeView = activeView;

    return true;
}

bool World::setActiveView( const String &viewName ) {
    OSRE_ASSERT( nullptr != m_impl );

    const ui32 hash( calcHash( viewName ) );
    if ( !m_impl->m_lookupViews.hasKey( hash ) ) {
        return false;
    }

    View *activeView( nullptr );
    if ( m_impl->m_lookupViews.getValue( hash, activeView ) ) {
        m_impl->m_activeView = activeView;
        return true;
    }

    return false;
}

void World::update( RenderBackendService *rbService ) {
    OSRE_ASSERT( nullptr != m_impl );

    if ( nullptr != m_impl->m_activeStage ) {
        m_impl->m_activeStage->update( rbService );
    }
    if ( nullptr != m_impl->m_activeView ) {
        m_impl->m_activeView->update( rbService );
    }
}

} // Namespace Scene
} // namespace OSRE
