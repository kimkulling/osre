#include <osre/App/AppBase.h>
#include <osre/App/CameraComponent.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/App/Stage.h>
#include <osre/App/ServiceProvider.h>
#include <osre/App/TransformController.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/MaterialBuilder.h>
#include <osre/RenderBackend/MeshBuilder.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include <vector>
#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries we will define this tag.
static constexpr c8 Tag[] = "TerrainRenderingApp";

class TerrainRenderingApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl;

public:
    TerrainRenderingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mEntity(nullptr),
            mKeyboardTransCtrl(nullptr) {
        // empty
    }
    
    ~TerrainRenderingApp() override {
        delete mEntity;
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
        float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
        for (int i = 0; i<height; ++i) {
            for (int j = 0; j<width; ++j) {
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
        for (int i = 0; i < height-1; i++) {      // for each row a.k.a. each strip
            for (int j = 0; j < width; j++) {     // for each column
                for (int k = 0; k < 2; k++)  {    // for each side of the strip
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

    Camera *setupCamera(World *world) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), world);
        world->addEntity(camEntity);
        Camera *camera =(Camera*) camEntity->createComponent(ComponentType::CameraComponentType);
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
        World *world = getStage()->addActiveWorld("hello_world");
        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);
        Camera *camera = setupCamera(world);

        //String filename = "world_heightmap.png";
        String filename = "Rolling_Hills.png";
        RenderBackend::Mesh *mesh = loadHeightMap(filename);
        if (mesh != nullptr) {
            RenderComponent *rc = (RenderComponent*) mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            // Initial update
	        Time dt =;
            world->update(dt);
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
        rbSrv->beginRenderBatch("terrain");

        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    TerrainRenderingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "Instancing-Sample", false, false, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}

