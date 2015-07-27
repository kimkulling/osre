#include <gtest/gtest.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
    namespace UnitTest {

        using namespace ::OSRE::RenderBackend;

        class RenderCommonTest : public ::testing::Test {

        };

        TEST_F( RenderCommonTest, createVertexLayoutTest ) {
            bool ok( true );
            try {
                VertexLayout layout;
            } catch( ... ) {
                ok = false;
            }
            EXPECT_TRUE( ok );
        }
    }
}
