/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <src/Engine/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/UI/Screen.h>
#include <osre/UI/UiItemFactory.h>
#include <osre/UI/TextBase.h>
#include <osre/UI/UiRenderer.h>

#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::UI;

static const c8 *Tag = "UiRenderTest";

class UiRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;
    Screen *m_screen;
    UiRenderer *m_uiRenderer;


public:
    UiRenderTest()
    : AbstractRenderTest( "rendertest/UiRenderTest" )
    , m_screen( nullptr )
    , m_uiRenderer( nullptr ) {
        // empty
    }

    virtual ~UiRenderTest() {
        // empty
    }

    bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );
        UiItemFactory::createInstance(getWindow());
        m_uiRenderer = new UiRenderer;
        m_screen = (Screen*)UiItemFactory::getInstance()->create(WidgetType::Screen, getTestName(), nullptr);
        m_screen->setSurface(getWindow());

        TextBase *text = (TextBase*) UiItemFactory::getInstance()->create(WidgetType::Text, getTestName() + ".test", m_screen);
        text->setLabel("Huhu");

        return true;
    }

    bool onRender( RenderBackendService *rbSrv ) override {
        m_uiRenderer->render(m_screen, rbSrv);

        return true;
    }

    bool onDestroy(RenderBackendService* ) override {
        delete m_uiRenderer;

        return true;
    }
};

ATTACH_RENDERTEST( UiRenderTest )

}
}
