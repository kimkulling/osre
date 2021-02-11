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
#include <osre/App/Component.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/MeshBuilder.h>
#include <osre/Scene/Camera.h>
#include <osre/Platform/AbstractWindow.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::RenderBackend;

// To identify local log entries we will define this tag.
static const c8 *Tag = "HelloWorldApp";

/// The example application, will create the render environment and render a simple triangle onto it
class HelloWorldApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock m_transformMatrix;
    /// The entity to render
    Entity *mEntity;

public:
    /// The class constructor with the incoming arguments from the command line.
    HelloWorldApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            m_transformMatrix(),
            mEntity(nullptr) {
        // empty
    }

    /// The class destructor.
    ~HelloWorldApp() override {
        // empty
    }

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Hello-World sample! Rotate with wasd, scroll with qe");
        World *world = getActiveWorld();
        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);
        Scene::Camera *camera = world->addCamera("camera_1");
        ui32 w, h;
        AppBase::getResolution(w, h);        
        camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);

        Scene::MeshBuilder meshBuilder;
        RenderBackend::Mesh *mesh = meshBuilder.allocTriangles(VertexType::ColorVertex, BufferAccessType::ReadOnly).getMesh();
        if (nullptr != mesh) {
            mEntity->addStaticMesh(mesh);
            world->addEntity(mEntity);            
            camera->observeBoundingBox(mEntity->getAABB());
        }

        return true;
    }

    void onUpdate() override {
        glm::mat4 rot(1.0);
        if (AppBase::isKeyPressed(Platform::KEY_A)) {
            m_transformMatrix.m_model *= glm::rotate(rot, 0.01f, glm::vec3(1, 0, 0));
        }
        if (AppBase::isKeyPressed(Platform::KEY_D)) {
            m_transformMatrix.m_model *= glm::rotate(rot, -0.01f, glm::vec3(1, 0, 0));
        }

        if (AppBase::isKeyPressed(Platform::KEY_W)) {
            m_transformMatrix.m_model *= glm::rotate(rot, 0.01f, glm::vec3(0, 1, 0));
        }

        if (AppBase::isKeyPressed(Platform::KEY_S)) {
            m_transformMatrix.m_model *= glm::rotate(rot, -0.01f, glm::vec3(0, 1, 0));
        }

        if (AppBase::isKeyPressed(Platform::KEY_Q)) {
            m_transformMatrix.m_model *= glm::scale(rot, glm::vec3(1.01f, 1.01, 1.01f));
        }

        if (AppBase::isKeyPressed(Platform::KEY_E)) {
            m_transformMatrix.m_model *= glm::scale(rot, glm::vec3(0.99f, 0.99f, 0.99f));
        }

        RenderBackendService *rbSrv = getRenderBackendService();

        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

/// Will generate the main function.
OSRE_MAIN(HelloWorldApp)
