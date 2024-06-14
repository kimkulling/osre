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

#include "App/Component.h"
#include "App/Entity.h"
#include "Common/Ids.h"
#include "App/TransformComponent.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

class ComponentTest : public ::testing::Test {
protected:
    Common::Ids *mIds;

    void SetUp() override {
        mIds = new Common::Ids;
    }

    void TearDown() override {
        delete mIds;
    }
};

class MockComponent : public Component {
public:
    MockComponent(Entity *owner, ComponentType type) 
	    : Component(owner, type) {}

    ~MockComponent() = default;

    void update(Time) {}

    void render(RenderBackend::RenderBackendService *) override {}

protected:
    bool onUpdate(Time dt) override {
        m_dt = dt;
        return true;
    }

    bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) override {
        EXPECT_NE(nullptr, renderBackendSrv);

        return true;
    }

private:
    Time m_dt;
};

TEST_F(ComponentTest, createTest) {
    bool ok(true);
    try {
        Common::Ids ids;
        Entity entity("test", ids, nullptr);
        MockComponent myComp(&entity, ComponentType::LightComponentType);
    } catch (...) {
        ok = false;
    }
    EXPECT_TRUE(ok);
}

TEST_F(ComponentTest, accessNodeTest) {
    String name = "test";

    Entity *entity = new Entity(name, *mIds, nullptr);
    MockComponent myComp(entity, ComponentType::LightComponentType);

    EXPECT_EQ(entity, myComp.getOwner());
}

TEST_F(ComponentTest, accessIdTest) {
    String name = "test";

    Entity *entity = new Entity(name, *mIds, nullptr);
    Component *rc = entity->getComponent(ComponentType::RenderComponentType);
    EXPECT_NE(nullptr, rc);
}

} // Namespace UnitTest
} // Namespace OSRE
