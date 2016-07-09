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
} // Namespace OSRE
