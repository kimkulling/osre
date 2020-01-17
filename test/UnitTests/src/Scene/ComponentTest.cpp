/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Ids.h>
#include <osre/Scene/Node.h>
#include <osre/App/Component.h>
#include <osre/App/Entity.h>

namespace OSRE {
namespace UnitTest {
        
using namespace ::OSRE::App;
using namespace ::OSRE::Scene;

class ComponentTest : public ::testing::Test {
protected:
    Common::Ids *m_ids;

    void SetUp() override {
        m_ids = new Common::Ids;
    }

    void TearDown() override {
        delete m_ids;
    }
};

class MockComponent : public Component {
public:
    MockComponent( Entity *owner, ui32 id ) 
    : Component( owner, id ) {
        // empty
    }

    ~MockComponent() {
        // empty
    }

    void update( Time ) {
        // empty
    }

    void render( RenderBackend::RenderBackendService* ) override {
        // empty
    }

protected:
    bool onPreprocess() override {
        return true;
    }

    bool onUpdate( Time dt ) override {
        return true;
    }

    bool onRender( RenderBackend::RenderBackendService *renderBackendSrv ) override {
        EXPECT_NE(nullptr, renderBackendSrv);

        return true;
    }

    bool onPostprocess() override {
        return true;
    }
};

TEST_F( ComponentTest, createTest ) {
    bool ok( true );
    try {
        MockComponent myComp( nullptr, 0 );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F(ComponentTest, accessNodeTest) {
    String name = "test";

    Entity *entity = new Entity(name, *m_ids);
    MockComponent myComp( entity, 0);

    EXPECT_EQ(entity, myComp.getOwner());
}

TEST_F( ComponentTest, accessIdTest ) {
    String name = "test";

    Entity *entity = new Entity(name, *m_ids);

    Component *tc = entity->getComponent(ComponentType::TransformComponentType);
    EXPECT_NE(nullptr, tc);
    const ui32 id1 = tc->getId();

    Component *rc = entity->getComponent(ComponentType::RenderComponentType);
    EXPECT_NE(nullptr, rc);
    const ui32 id2 = rc->getId();

    EXPECT_NE(id1, id2);
}

} // Namespace UnitTest
} // Namespace OSRE
