/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions 
  and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
  and the following disclaimer in the documentation and/or other materials provided with the 
  distribution.

* Neither the name of the ASSIMP team, nor the names of its contributors may be used to endorse or 
  promote products derived from this software without specific prior written permission of the 
  ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <osre/Threading/AbstractTask.h>
#include <osre/Platform/AtomicInt.h>

namespace OSRE {
namespace Threading {

//-------------------------------------------------------------------------------------------------
AbstractTask::AbstractTask( const String &taskName )
: Object( taskName )
, m_pRefCount( nullptr )
, m_pParent( nullptr ) {
    m_pRefCount = new Platform::AtomicInt( 0 );
}

//-------------------------------------------------------------------------------------------------
AbstractTask::~AbstractTask() {
    delete m_pRefCount;
    m_pRefCount = nullptr;
}

//-------------------------------------------------------------------------------------------------
bool AbstractTask::preExecute() {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool AbstractTask::postExecute() {
    if ( m_pParent ) {
        m_pParent->removeDependency();
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void AbstractTask::addDependency() {
    m_pRefCount->incValue( 1 );
}

//-------------------------------------------------------------------------------------------------
void AbstractTask::removeDependency()  {
    if ( m_pRefCount->getValue() > 0 ) {
        m_pRefCount->decValue( 1 );
    }
}

//-------------------------------------------------------------------------------------------------
ui32 AbstractTask::getNumRefs() const {
    if ( m_pRefCount ) {
        const ui32 numRefs = m_pRefCount->getValue();
        return numRefs;
    } else {
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
AbstractTask *AbstractTask::getParent() const {
    return m_pParent;
}

//-------------------------------------------------------------------------------------------------
void AbstractTask::setParent( AbstractTask *pParent ) {
    m_pParent = pParent;
    if ( m_pParent ) {
        m_pParent->addDependency();
    }
}

//-------------------------------------------------------------------------------------------------
void AbstractTask::enqueue( AbstractTask *pTask ) {
    if ( pTask ) {
        pTask->setParent( this );
        m_childTasks.add( pTask );
    }
}

//-------------------------------------------------------------------------------------------------
ui32 AbstractTask::getNumChildTasks() const {
    return m_childTasks.size();
}

//-------------------------------------------------------------------------------------------------
AbstractTask *AbstractTask::getChildTask( ui32 idx ) const {
    return m_childTasks[ idx ];
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
