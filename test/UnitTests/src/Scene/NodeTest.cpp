/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "osre_testcommon.h"

#include "RenderBackend/RenderCommon.h"
#include "App/TransformComponent.h"
#include "App/Component.h"
#include "App/Entity.h"
#include "Common/Ids.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

class TransformComponentTest : public ::testing::Test {
protected:
    App::Entity *mEntity;
    Common::Ids *mIds;
    std::vector<SceneNode*> mNodes;

    virtual void SetUp() {
        mIds = new Common::Ids( 0 );
        mEntity = new App::Entity("test", *mIds, nullptr);
    }

    virtual void TearDown() {
        delete mIds;
        mIds = nullptr;

        delete mEntity;
        mEntity = nullptr;

        for ( auto &current : mNodes ) {
            current->release();
        }
        mNodes.resize(0);
    }

    SceneNode *createNode(const String &name, Entity *owner, Common::Ids &ids, SceneNode *parent) {
        SceneNode *n = new SceneNode(name, owner, ids, parent);
        addNodeForRelease( n );
        
        return n;
    }

    void addNodeForRelease(SceneNode *node) {
        mNodes.push_back(node);
    }
};

TEST_F( TransformComponentTest, createTest ) {
    bool ok = true;
    try {
        SceneNode *myNode_transform_render = createNode("testnode1", mEntity, *mIds, nullptr);
        EXPECT_NE( nullptr, myNode_transform_render );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( TransformComponentTest, accessChildren ) {
    SceneNode *parent = createNode("parent", mEntity, *mIds, nullptr);
    SceneNode *myNode1 = createNode("testnode1", mEntity, *mIds, parent);
    SceneNode *myNode2 = createNode("testnode2", mEntity, *mIds, parent);

    EXPECT_EQ( 2u, parent->getNumChildren() );
    EXPECT_TRUE( nullptr != myNode1->getParent() );
    EXPECT_TRUE( nullptr != myNode2->getParent() );
    EXPECT_TRUE( nullptr == parent->getParent() );

    SceneNode *found = parent->findChild("testnode1");
    EXPECT_TRUE(nullptr != found);

    found = parent->findChild( "testnode3" );
    EXPECT_TRUE(nullptr == found);

    bool ok = parent->removeChild("testnode1");
    EXPECT_TRUE( ok );

    ok = parent->removeChild("testnode1");
    EXPECT_FALSE(ok);
}

TEST_F(TransformComponentTest, activeTest) {
    SceneNode *myNode = createNode("parent", mEntity, *mIds, nullptr);
    EXPECT_TRUE( myNode->isActive() );

    myNode->setActive( false );
    EXPECT_FALSE(myNode->isActive());
}

} // Namespace UnitTest
} // Namespace OSRE
