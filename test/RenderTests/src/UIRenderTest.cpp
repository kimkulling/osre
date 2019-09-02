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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <src/Engine/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/UiItemFactory.h>
#include <osre/UI/TextBase.h>
#include <osre/UI/Panel.h>
#include <osre/UI/ButtonBase.h>
#include <osre/UI/UiRenderer.h>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;
using namespace ::OSRE::UI;

static const c8 *Tag = "UiRenderTest";

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief
//-------------------------------------------------------------------------------------------------
class UiRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;
    Canvas *m_canvas;
    Stage *m_stage;
    UiRenderer *m_uiRenderer;

public:
    UiRenderTest()
    : AbstractRenderTest( "rendertest/UiRenderTest" )
    , m_canvas( nullptr )
    , m_stage( nullptr )
    , m_uiRenderer( nullptr ) {
        // empty
    }

    virtual ~UiRenderTest() {
        // empty
    }

    bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );

        m_stage = new Stage("ui_stage", rbSrv);

        UiItemFactory::createInstance(getWindow());
        m_uiRenderer = new UiRenderer;
        m_canvas = (Canvas*) UiItemFactory::getInstance()->create(WidgetType::Canvas, getTestName(), nullptr);
        m_canvas->setSurface(getWindow());

        Panel *panel1 = (Panel*) UiItemFactory::getInstance()->create(WidgetType::Panel, getTestName() + ".panel1", m_canvas);
        panel1->setHeadline("Button-tests");
        panel1->setRect(10, 10, 200, 200);
        ButtonBase *btn1 = (ButtonBase*)UiItemFactory::getInstance()->create(WidgetType::Button, getTestName() + ".btn1", panel1);
        btn1->setRect(15, 15, 190, 20);
        ButtonBase *btn2 = (ButtonBase*)UiItemFactory::getInstance()->create(WidgetType::Button, getTestName() + ".btn2", panel1);
        btn2->setRect(15, 40, 190, 20);

        Panel *panel2 = (Panel*)UiItemFactory::getInstance()->create(WidgetType::Panel, getTestName() + ".panel2", m_canvas);
        panel2->setRect(230, 10, 200, 200);

        panel2->setHeadline("Text-tests");
        TextBase *text = (TextBase*) UiItemFactory::getInstance()->create(WidgetType::Text, getTestName() + ".test", panel2);
        text->setLabel( "Huhu" );

        Panel *panel3 = (Panel*)UiItemFactory::getInstance()->create(WidgetType::Panel, getTestName() + ".panel2", m_canvas);
        panel3->setRect(450, 10, 200, 200);

        return true;
    }

    bool onRender( RenderBackendService *rbSrv ) override {
        m_uiRenderer->render( m_canvas, rbSrv );

        return true;
    }

    bool onDestroy(RenderBackendService* ) override {
        delete m_uiRenderer;

        return true;
    }
};

ATTACH_RENDERTEST( UiRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
