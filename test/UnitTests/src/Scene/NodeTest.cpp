/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/App/TransformComponent.h>
#include <osre/App/Component.h>
#include <osre/App/Entity.h>
#include <osre/Common/Ids.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

class TransformComponentTest : public ::testing::Test {
protected:
    App::Entity *mEntity;
    Common::Ids *mIds;
    std::vector<TransformComponent*> mNodes;

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

    TransformComponent *createNode(const String &name, Entity *owner, Common::Ids &ids, TransformComponent *parent) {
        TransformComponent *n = new TransformComponent(name, owner, ids, parent);
        addNodeForRelease( n );
        
        return n;
    }

    void addNodeForRelease(TransformComponent *node) {
        mNodes.push_back(node);
    }
};

TEST_F( TransformComponentTest, createTest ) {
    bool ok = true;
    try {
        TransformComponent *myNode_transform_render = createNode("testnode1", mEntity, *mIds, nullptr);
        EXPECT_NE( nullptr, myNode_transform_render );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( TransformComponentTest, accessChildren ) {
    TransformComponent *parent = createNode("parent", mEntity, *mIds, nullptr);
    TransformComponent *myNode1 = createNode("testnode1", mEntity, *mIds, parent);
    TransformComponent *myNode2 = createNode("testnode2", mEntity, *mIds, parent);

    EXPECT_EQ( 2u, parent->getNumChildren() );
    EXPECT_TRUE( nullptr != myNode1->getParent() );
    EXPECT_TRUE( nullptr != myNode2->getParent() );
    EXPECT_TRUE( nullptr == parent->getParent() );

    TransformComponent *found = parent->findChild("testnode1");
    EXPECT_TRUE(nullptr != found);

    found = parent->findChild( "testnode3" );
    EXPECT_TRUE(nullptr == found);

    bool ok = parent->removeChild("testnode1", TransformComponent::TraverseMode::FlatMode);
    EXPECT_TRUE( ok );

    ok = parent->removeChild("testnode1", TransformComponent::TraverseMode::FlatMode);
    EXPECT_FALSE(ok);
}

TEST_F(TransformComponentTest, activeTest) {
    TransformComponent *myNode = createNode("parent", mEntity, *mIds, nullptr);
    EXPECT_TRUE( myNode->isActive() );

    myNode->setActive( false );
    EXPECT_FALSE(myNode->isActive());
}

TEST_F(TransformComponentTest, onUpdateTest) {
    TransformComponent *comp = createNode("parent", mEntity, *mIds, nullptr);
    EXPECT_NE(nullptr, comp);

    glm::mat4 mat1 = comp->getTransformationMatrix();
    glm::vec3 pos(1, 2, 3);
    comp->translate(pos);
    Time dt;
    comp->update(dt);

    glm::mat4 mat2 = comp->getTransformationMatrix();
    EXPECT_NE(mat1, mat2);
    EXPECT_FLOAT_EQ(mat2[3][0], 1);
    EXPECT_FLOAT_EQ(mat2[3][1], 2);
    EXPECT_FLOAT_EQ(mat2[3][2], 3);
}

TEST_F(TransformComponentTest, onUpdateWorldTransformTest) {
    TransformComponent *comp_parent = createNode("parent", mEntity, *mIds, nullptr);
    EXPECT_NE(nullptr, comp_parent);
    TransformComponent *comp_child = createNode("parent", mEntity, *mIds, comp_parent);
    EXPECT_NE(nullptr, comp_child);
    EXPECT_EQ(comp_parent, comp_child->getParent());

    const glm::vec3 pos(1, 2, 3);
    comp_child->translate(pos);
    comp_parent->translate(pos);
    Time dt;
    comp_child->update(dt);
    comp_parent->update(dt);
    glm::mat4 mat_child = comp_child->getWorlTransformMatrix();
    EXPECT_FLOAT_EQ(mat_child[3][0], 2);
    EXPECT_FLOAT_EQ(mat_child[3][1], 4);
    EXPECT_FLOAT_EQ(mat_child[3][2], 6);
    glm::mat4 mat_parent = comp_parent->getWorlTransformMatrix();
    EXPECT_FLOAT_EQ(mat_parent[3][0], 1);
    EXPECT_FLOAT_EQ(mat_parent[3][1], 2);
    EXPECT_FLOAT_EQ(mat_parent[3][2], 3);
}

} // Namespace UnitTest
} // Namespace OSRE
