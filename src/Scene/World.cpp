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
#include <osre/Common/StringUtils.h>
#include <osre/Debugging/osre_debugging.h>
#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Scene {

using namespace ::CPPCore;
using namespace ::OSRE::Common;

struct World::Impl {
    TArray<Stage*> m_stages;
    THashMap<ui32, Stage*> m_lookupStates;
    Stage *m_activeStage;

    Impl()
        : m_stages()
        , m_activeStage( nullptr ) {

    }

    ~Impl() {
        m_lookupStates.clear();
        m_activeStage = nullptr;
        for ( ui32 i = 0; i < m_stages.size(); ++i ) {
            if ( nullptr != m_stages[ i ] ) {
                m_stages[ i ]->release();
            }
        }
        m_stages.clear();
    }
};

World::World( const String &worldName )
: Object( worldName )
, m_impl( nullptr ) {
    m_impl = new Impl;
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

    const ui32 hash( StringUtils::hashName( stage->getName().c_str() ) );
    if ( m_impl->m_lookupStates.hasKey( hash ) ) {
        return;
    }

    m_impl->m_stages.add( stage );
    m_impl->m_lookupStates.insert( hash, stage );
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
    if ( m_impl->m_lookupStates.hasKey( hash ) ) {
        return false;
    }
    
    Stage *activeStage( nullptr );
    if ( m_impl->m_lookupStates.getValue( hash, activeStage ) ) {
        m_impl->m_activeStage = activeStage;
        return true;
    }

    return false;
}

} // Namespace Scene
} // namespace OSRE
