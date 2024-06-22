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
#include "App/App.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "App/Entity.h"
#include "Platform/AbstractWindow.h"
#include "Common/glm_common.h"

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::RenderBackend;

// To identify local log entries we will define this tag.
static constexpr c8 Tag[] = "HelloWorldApp";

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief This is
//-------------------------------------------------------------------------------------------------

/// The example application, will create the render environment and render a simple triangle onto it
class HelloWorldApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl;

public:
    /// The class constructor with the incoming arguments from the command line.
    HelloWorldApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mEntity(nullptr),
            mKeyboardTransCtrl(nullptr) {
        // empty
    }

    /// The class destructor.
    ~HelloWorldApp() override = default;

protected:
    CameraComponent *setupCamera(World *world) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), world);
        world->addEntity(camEntity);
        CameraComponent *camera =(CameraComponent*) camEntity->createComponent(ComponentType::CameraComponentType);
        world->setActiveCamera(camera);
        ui32 w, h;
        AppBase::getResolution(w, h);
        camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);

        return camera;
    }

    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Hello-World sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
        World *world = getStage()->addActiveWorld("hello_world");

        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);
        MeshBuilder meshBuilder;
        Mesh *mesh = meshBuilder.createCube(VertexType::ColorVertex, .5,.5,.5,BufferAccessType::ReadOnly).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent*) mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            CameraComponent *camera = setupCamera(world);
            world->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }
        mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);

        osre_info(Tag, "Creation finished.");

        return true;
    }

    void onUpdate() override {
        Platform::Key key = AppBase::getKeyboardEventListener()->getLastKey();
        mKeyboardTransCtrl->update(TransformController::getKeyBinding(key));

        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);

            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main( int argc, char *argv[] )  {
    HelloWorldApp myApp( argc, argv );
    if ( !myApp.create() ) {
        return 1;
    }
    while ( myApp.handleEvents() ) {
        myApp.update();
        myApp.requestNextFrame();
    }
    myApp.destroy();
    return 0;
}

