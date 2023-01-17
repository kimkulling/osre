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

class NodeTest : public ::testing::Test {
protected:
    App::Entity *mEntity;
    Common::Ids *m_ids;
    std::vector<TransformComponent*> m_nodes;

    virtual void SetUp() {
        m_ids = new Common::Ids( 0 );
        mEntity = new App::Entity("test", *m_ids, nullptr);
    }

    virtual void TearDown() {
        delete m_ids;
        m_ids = nullptr;

        delete mEntity;
        mEntity = nullptr;

        for ( auto &current : m_nodes ) {
            current->release();
        }
        m_nodes.resize( 0 );
    }

    TransformComponent *createNode(const String &name, Entity *owner, Common::Ids &ids, TransformComponent *parent) {
        TransformComponent *n(new TransformComponent(name, owner, ids, parent));
        addNodeForRelease( n );
        
        return n;
    }

    void addNodeForRelease(TransformComponent *node) {
        m_nodes.push_back(node);
    }
};

TEST_F( NodeTest, createTest ) {
    bool ok = true;
    try {
        TransformComponent *myNode_transform_render = createNode("testnode1", mEntity, *m_ids, nullptr);
        EXPECT_NE( nullptr, myNode_transform_render );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( NodeTest, accessChildren ) {
    TransformComponent *parent = createNode("parent", mEntity, *m_ids, nullptr);
    TransformComponent *myNode1 = createNode("testnode1", mEntity, *m_ids, parent);
    TransformComponent *myNode2 = createNode("testnode2", mEntity, *m_ids, parent);

    EXPECT_EQ( 2u, parent->getNumChildren() );
    EXPECT_TRUE( nullptr != myNode1->getParent() );
    EXPECT_TRUE( nullptr != myNode2->getParent() );
    EXPECT_TRUE( nullptr == parent->getParent() );

    TransformComponent *found = parent->findChild("testnode1");
    EXPECT_TRUE( nullptr != found );

    found = parent->findChild( "testnode3" );
    EXPECT_TRUE( nullptr == found );

    bool ok = parent->removeChild("testnode1", TransformComponent::TraverseMode::FlatMode);
    EXPECT_TRUE( ok );

    ok = parent->removeChild("testnode1", TransformComponent::TraverseMode::FlatMode);
    EXPECT_FALSE( ok );
}

TEST_F( NodeTest, activeTest ) {
    TransformComponent *myNode = createNode("parent", mEntity, *m_ids, nullptr);
    EXPECT_TRUE( myNode->isActive() );

    myNode->setActive( false );
    EXPECT_FALSE( myNode->isActive() );
}

TEST_F( NodeTest, onUpdateTest ) {
    TransformComponent *myNode = createNode("parent", mEntity, *m_ids, nullptr);
    EXPECT_NE(nullptr, myNode);
}

} // Namespace UnitTest
} // Namespace OSRE
