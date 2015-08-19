/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderTest {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::RenderTest::AbstractTestFixure
///	@ingroup	Test
///
///	@brief  This class is the abstract base class for all render test fixture.
///
/// To implement a new render test fixture you have to implement the setup and the teardown method. 
/// Setup will be called before performing the fixture, teardown will be called to cleanup after the
/// render test was executed.
//-------------------------------------------------------------------------------------------------
class AbstractTestFixture {
public:
    virtual bool setup() = 0;
    virtual bool teardown() = 0;
    const String &getName() const;

protected:
    AbstractTestFixture( const String &name );
    virtual ~AbstractTestFixture();

private:
    AbstractTestFixture();
    AbstractTestFixture( const AbstractTestFixture & );

private:
    String m_name;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractTestFixture::AbstractTestFixture( const String &name )
: m_name( name ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
AbstractTestFixture::~AbstractTestFixture() {

}

//-------------------------------------------------------------------------------------------------
inline
const String &AbstractTestFixture::getName() const {
    return m_name;
}

//-------------------------------------------------------------------------------------------------

}
}
