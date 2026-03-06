## Terrain Renderer
![Terrain rendering](../../assets/Images/01_box.png)
This sample shows how to render a terrain from a heightmap image.

```cpp
#include "App/App.h"
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

DECL_OSRE_LOG_MODULE(TerrainRenderingApp);

class TerrainRenderingApp : public App::AppBase {
    TransformMatrixBlock mTransformMatrix;
    Entity *mEntity = nullptr;
    Animation::AnimationControllerBase *mKeyboardTransCtrl = nullptr;
    ui32 mPixelPerSample = 1;

public:
    TerrainRenderingApp(int argc, char *argv[]) : AppBase(argc, (const char **)argv) {
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

        const int numVertices = width * height;
        size_t index = 0;
        RenderVert *v = new RenderVert[numVertices];
        std::vector<float> vertices;
        float yScale = 128.0f / 256.0f, yShift = 16.0f;
        for (int i = 0; i<height; i=i+mPixelPerSample) {
            for (int j = 0; j<width; j=j+mPixelPerSample) {
                unsigned char *texel = data + (j + width * i) * nChannels;
                unsigned char y = texel[0];

                v[index].position.x = -height/2.0f + i;
                v[index].position.y = (int) y * yScale - yShift;
                v[index].position.z = -width/2.0f + j;
                ++index;
            }
        }
        stbi_image_free(data);

        std::vector<unsigned int> indices;
        for (ui32 i = 0; i < (height/mPixelPerSample)-1; i++) {
            for (ui32 j = 0; j < (width/mPixelPerSample); j++) {
                for (ui32 k = 0; k < 2; k++)  {
                    indices.push_back(j + width * (i + k));
                }
            }
        }

        MeshBuilder meshBuilder;
        Mesh *mesh = meshBuilder.allocEmptyMesh("terrain", VertexType::RenderVertex).getMesh();
        if (mesh == nullptr) {
            return nullptr;
        }

        mesh->createVertexBuffer(v, (size_t) numVertices * sizeof(RenderVert), BufferAccessType::ReadWrite);
        mesh->createIndexBuffer(&indices[0], indices.size(), IndexType::UnsignedInt, BufferAccessType::ReadWrite);
        mesh->addPrimitiveGroup(indices.size(), PrimitiveType::TriangelStrip, 0);
        TextureResourceArray texResArray;
        mesh->setMaterial(MaterialBuilder::createBuildinMaterial("default.mat", texResArray, VertexType::RenderVertex));

        return mesh;
    }

    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Terrain sample! Rotate with keyboard: w, a, s, d, zoom with q, e");
        Scene *world = new Scene("hello_world");
        addScene(world, true);
        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);

        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            osre_error(Tag, "Root window not available.");
            return false;
        }

        Rect2ui windowsRect;
        rootWindow->getWindowsRect(windowsRect);
        CameraComponent *camera = AppBase::setupCamera("camera", world, windowsRect, *getIdContainer());

        String filename = "world_heightmap.png";
        RenderBackend::Mesh *mesh = loadHeightMap(filename);
        if (mesh != nullptr) {
            RenderComponent *rc = (RenderComponent*) mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);
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
```

## Key Concepts
The terrain renderer loads a heightmap image and generates a 3D terrain mesh from it. The height values from the image are used to set the Y-coordinate of each vertex, creating the terrain topography.
