/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/AppBase.h"
#include "App/CameraComponent.h"
#include "App/Entity.h"
#include "App/Scene.h"
#include "App/ServiceProvider.h"
#include "App/TransformController.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/TransformMatrixBlock.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include <vector>
#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries we will define this tag.
static constexpr c8 Tag[] = "TerrainRenderingApp";

//-------------------------------------------------------------------------------------------------
///	@ingroup    Samples
///
/// @brief
//-------------------------------------------------------------------------------------------------
class TerrainRenderingApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl;
    /// The number of pixels used for one sample.
    ui32 mPixelPerSample;

public:
    TerrainRenderingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mEntity(nullptr),
            mKeyboardTransCtrl(nullptr),
            mPixelPerSample(1) {
        // empty
    }

    ~TerrainRenderingApp() override {
        delete mEntity;
    }

    void setPixelPerSample(ui32 pixelPerSample) {
        mPixelPerSample = pixelPerSample;
    }

    ui32 getPixelPerSample() const {
        return mPixelPerSample;
    }

protected:
    Mesh *loadHeightMap(const String &filename) {
        if (filename.empty()) {
            return nullptr;
        }

        int width = 0, height = 0, nChannels = 0;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nChannels, 0);
        if (data == nullptr) {
            return nullptr;
        }

        // vertex generation
        const int numVertices = width * height;
        size_t index = 0;
        RenderVert *v = new RenderVert[numVertices];
        std::vector<float> vertices;
        float yScale = 128.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
        for (int i = 0; i<height; i=i+mPixelPerSample) {
            for (int j = 0; j<width; j=j+mPixelPerSample) {
                // retrieve texel for (i,j) tex coord
                unsigned char *texel = data + (j + width * i) * nChannels;
                // raw height at coordinate
                unsigned char y = texel[0];

                // vertex position
                v[index].position.x = -height/2.0f + i;
                v[index].position.y = (int) y * yScale - yShift;
                v[index].position.z = -width/2.0f + j;
                ++index;
            }
        }
        std::cout << "Number of vertices = " << numVertices << "\n";
        stbi_image_free(data);

        // Index generation
        std::vector<unsigned int> indices;
        for (ui32 i = 0; i < (height/mPixelPerSample)-1; i++) {      // for each row a.k.a. each strip
            for (ui32 j = 0; j < (width/mPixelPerSample); j++) {     // for each column
                for (ui32 k = 0; k < 2; k++)  {    // for each side of the strip
                    indices.push_back(j + width * (i + k));
                }
            }
        }
        std::cout << "Number of indices = " << indices.size() << "\n";

        MeshBuilder meshBuilder;
        Mesh *mesh = meshBuilder.allocEmptyMesh("terrain", VertexType::RenderVertex).getMesh();
        if (mesh == nullptr) {
            return nullptr;
        }

        mesh->createVertexBuffer(v, (size_t) numVertices * sizeof(RenderVert), BufferAccessType::ReadWrite);
        mesh->createIndexBuffer(&indices[0], indices.size(), IndexType::UnsignedInt, BufferAccessType::ReadWrite);

        // setup primitives
        mesh->addPrimitiveGroup(indices.size(), PrimitiveType::TriangelStrip, 0);
        mesh->setMaterial(MaterialBuilder::createBuildinMaterial(VertexType::RenderVertex));

        return mesh;
    }

    CameraComponent *setupCamera(Scene *world) {
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

        AppBase::setWindowsTitle("Terrain sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
        Scene *world = new Scene("hello_world", *getIdContainer());
        addScene(world, true);
        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);
        CameraComponent *camera = setupCamera(world);

        String filename = "world_heightmap.png";
        RenderBackend::Mesh *mesh = loadHeightMap(filename);
        if (mesh != nullptr) {
            RenderComponent *rc = (RenderComponent*) mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            // Initial update
            world->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }
        mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);

        osre_info(Tag, "Creation finished.");

        return true;
    }

    void onUpdate() override {
        Platform::Key key = AppBase::getKeyboardEventListener()->getLastKey();
        if (key != Platform::KEY_UNKNOWN) {
            mKeyboardTransCtrl->update(mKeyboardTransCtrl->getKeyBinding(key));
        }

        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    TerrainRenderingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "Instancing-Sample", WindowMode::Windowed, WindowType::Root, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
