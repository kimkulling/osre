#include <gtest/gtest.h>
#include <osre/Assets/AssetData.h>

namespace OSRE {
namespace UnitTest {
        
using namespace ::OSRE::Assets;

class AssetDataTest : public ::testing::Test {
    // empty
};

TEST_F( AssetDataTest, createTest ) {
    bool ok( true );
    try {
        AssetData myData;
    }
    catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( AssetDataTest, load_save_Test ) {
    
}

}
}
