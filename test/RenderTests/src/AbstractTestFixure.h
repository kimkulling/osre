#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace RenderTest {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderTest::AbstractTestFixure
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
