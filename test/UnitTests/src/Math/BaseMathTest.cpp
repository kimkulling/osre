#include <gtest/gtest.h>
#include <osre/Math/BaseMath.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Math;

class BaseMathTest : public testing::Test {
    // empty
};

TEST_F( BaseMathTest, lerpTest ) {
    f32 res = lerp<f32>( 0.0f, 1.0f, 0.5f );
    EXPECT_FLOAT_EQ( 0.5f, res );
}

}
}
