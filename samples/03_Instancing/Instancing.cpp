/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/AppBase.h>
#include <osre/App/AssetRegistry.h>
#include <osre/App/AssimpWrapper.h>
#include <osre/App/Component.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/Common/Ids.h>
#include <osre/IO/Uri.h>
#include <osre/Math/BaseMath.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/DbgRenderer.h>
#include <osre/Scene/MeshBuilder.h>
#include <osre/Scene/MeshProcessor.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/Camera.h>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

// To identify local log entries
static const c8 *Tag = "InstancingApp";

/// The example application, will create the render environment and render a simple triangle onto it
class InstancingApp : public App::AppBase {
    App::Entity *mEntity;
    Scene::Camera *mCamera;

public:
    InstancingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api:model", "The render API:The model to load"),
            mEntity(nullptr),
            mCamera(nullptr) {
        // empty
    }

    virtual ~InstancingApp() {
        // empty
    }

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }
        AppBase::setWindowsTitle("Instancing sample!");

#ifdef OSRE_WINDOWS
        AssetRegistry::registerAssetPath("assets", "../../assets");
#else
        AssetRegistry::registerAssetPath("assets", "../assets");
#endif

        RenderBackendService *rbSrv = getRenderBackendService();
        if (nullptr == rbSrv) {
            return false;
        }

        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            return false;
        }

        Rect2ui windowsRect;
        rootWindow->getWindowsRect(windowsRect);

        Camera *camera = getActiveWorld()->addCamera("cam1");
        camera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.0001f, 1000.f);
        mEntity = new App::Entity("instance", getActiveWorld()->getIds(), getActiveWorld());
        Scene::MeshBuilder meshBuilder;
        AppBase::getActiveWorld()->addEntity(mEntity);
//        RenderBackend::Mesh *mesh = meshBuilder.allocCube(VertexType::RenderVertex, 1, 2, 3, BufferAccessType::ReadWrite).getMesh();
        RenderBackend::Mesh *mesh = meshBuilder.allocTriangles(VertexType::RenderVertex, BufferAccessType::ReadWrite).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);
            Scene::MeshProcessor process;
            process.addGeo(mesh);
            process.execute();
            Scene::Node::AABB aabb = process.getAABB();
            Node *root = getActiveWorld()->getRootNode();
            Node *geoNode = root->createChild("mesh_node");
            geoNode->setAABB(aabb);
            camera->observeBoundingBox(aabb);
        }

        return true;
    }

    void onUpdate() override {
        // Rotate the model
        glm::mat4 rot(1.0);

        RenderBackendService *rbSrv(getRenderBackendService());

        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        // Scene::DbgRenderer::getInstance()->renderDbgText(0, 0, 2U, "XXX");

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    InstancingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "ModelLoader-Sample", false, App::RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
