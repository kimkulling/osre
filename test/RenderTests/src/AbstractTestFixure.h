/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
///	@ingroup	Test
///
///	@brief  This class is the abstract base class for all render test fixture.
///
/// To implement a new render test fixture you have to implement the setup and the teardown method. 
/// Setup will be called before performing the fixture, teardown will be called to cleanup after the
/// render test was executed.
/// You can use a fixture to group tests.
//-------------------------------------------------------------------------------------------------
class AbstractTestFixture {
public:
    /// @brief  The setup method, will be called before the test execution of the fixture.
    /// @param  API     [in] The requested API.
    /// @return true if successful, false in case of an error.
    virtual bool setup( const String &API ) = 0;

    /// @brief  The teardown method, will be called after the test execution of the fixture.
    /// @return true if successful, false in case of an error.
    virtual bool teardown() = 0;

    /// @brief  Returns the test fixture name.
    /// @return The name of the test fixture.
    const String &getName() const;

protected:
    /// @brief  The class constructor.
    /// @param  name        [in] The name of the test.
    AbstractTestFixture( const String &name );

    /// @brief  The class destructor.
    virtual ~AbstractTestFixture();

    // Avoid copying
    AbstractTestFixture() = delete;
    AbstractTestFixture( const AbstractTestFixture & ) = delete;
    AbstractTestFixture &operator = ( const AbstractTestFixture & ) = delete;

private:
    String m_name;
};

inline
AbstractTestFixture::AbstractTestFixture( const String &name )
: m_name( name ) {
    // empty
}

inline
AbstractTestFixture::~AbstractTestFixture() {
    // empty
}

inline
const String &AbstractTestFixture::getName() const {
    return m_name;
}

} // Namespace RenderTest
} // Namespace OSRE
