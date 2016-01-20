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
#include <osre/Platform/AtomicInt.h>
#include <osre/Platform/AbstractThreadFactory.h>

#include <cassert>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
AtomicInt::AtomicInt( i32 val )
: m_pImpl( AbstractThreadFactory::getInstance()->createAtomic( val ) ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
AtomicInt::~AtomicInt() {
    delete m_pImpl;
    m_pImpl = nullptr;
}

//-------------------------------------------------------------------------------------------------
void AtomicInt::incValue( i32 value ) {
    assert( nullptr != m_pImpl );
    m_pImpl->incValue( value );
}

//-------------------------------------------------------------------------------------------------
void AtomicInt::decValue( i32 value ) {
    assert( nullptr != m_pImpl );   
    m_pImpl->decValue( value );
}

//-------------------------------------------------------------------------------------------------
i32 AtomicInt::getValue() const {
    assert( nullptr != m_pImpl );
    return m_pImpl->getValue( );
}

//-------------------------------------------------------------------------------------------------
i32 AtomicInt::operator ++ ( ) {
    assert( nullptr != m_pImpl );
    return m_pImpl->inc( );
}

//-------------------------------------------------------------------------------------------------
i32 AtomicInt::operator -- ( ) {
    assert( nullptr != m_pImpl );
    return m_pImpl->dec( );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
