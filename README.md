# OSRE - Just another Open Source Render Engine

Just another open-source render-engine made by Kim Kulling. This is a playground project for myself to work on my own side-projects.

# The Core-Features:
- Multithreaded rendering 
- Platform abstraction-layer
- OS-independent task-support to make multi-threaded rendering more easy
- Model-import provided by Asset-Importer-Lib (see https://github.com/assimp/assimp)
- Bitmap-Font-Rendering
- Simple scenegraph-module
- Virtual file-system for reading zip-archives
- CMake base build environment

Discussions: [![Join the chat at https://gitter.im/kimkulling/osre](https://badges.gitter.im/kimkulling/osre.svg)](https://gitter.im/kimkulling/osre?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Documentation
- Please check [OSRE-Docs](https://osre-doc.readthedocs.io/en/latest/).

# Build Status
[![Build status](https://github.com/kimkulling/osre/actions/workflows/cmake.yml/badge.svg)](https://github.com/kimkulling/osre/actions/workflows/cmake.yml)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/13242/badge.svg)](https://scan.coverity.com/projects/kimkulling-osre)
[![Documentation Status](https://readthedocs.org/projects/osre-doc/badge/?version=latest)](https://osre-doc.readthedocs.io/en/latest/?badge=latest)

# Supported compilers
- Windows:
  - Visual Studio 2017
  - Visual Studio 2019
- Linux:
  - GCC
  - Clang

# Quick Start

```cpp
#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/MeshBuilder.h>
#include <osre/Scene/Camera.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/RenderBackend/glm_common.h>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::RenderBackend;

class QuickStarterApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock m_transformMatrix;
    /// The entity to render
    Entity *mEntity;

public:
    /// The class constructor with the incoming arguments from the command line.
    QuickStarterApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            m_transformMatrix(),
            mEntity(nullptr) {
        // empty
    }

    /// The class destructor.
    ~QuickStarterApp() override {
        // empty
    }

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Quickstart! Rotate with wasd, scroll with qe");
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

        // Set the model-matrix in the renderpass
        RenderBackendService *rbSrv = getRenderBackendService();

        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

OSRE_MAIN(QuickStarterApp)
```

# OSRE-Ed
![ESRE-Ed](assets/Images/sponza.png)

The engine provdes an 3D-Editor called OSRE-Ed. It is still experimental:

# Get involved
If you want to contribute just use the github project page or reach us via Gitter:
[![Join the chat at https://gitter.im/kimkulling/osre](https://badges.gitter.im/kimkulling/osre.svg)](https://gitter.im/kimkulling/osre?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# What is cooking?
The project roadmap https://github.com/kimkulling/osre/projects/1
