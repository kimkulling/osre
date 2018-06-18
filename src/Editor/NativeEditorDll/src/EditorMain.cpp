#include "EditorMain.h"

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/World.h>
#include <osre/IO/IOService.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#endif // OSRE_WINDOWS

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Scene;

class EditorApplication : public App::AppBase {
    World *m_world;
    Stage *m_stage;
    Node::NodePtr m_modelNode;
    TransformMatrixBlock m_transformMatrix;

public:
    EditorApplication(int argc, char *argv[])
    : AppBase(argc, argv)
    , m_world( nullptr )
    , m_stage( nullptr )
    , m_modelNode()
    , m_transformMatrix() {
        // empty
    }

    virtual ~EditorApplication() {
        // empty
    }

    int enqueueEvent() {
        return 0;
    }

    World *getWorld() const {
        return m_world;
    }

    int importAsset( const String &filename, int flags ) {
        if (filename.empty() ) {
            return 1;
        }

        Ids ids;
        Assets::AssimpWrapper assimpWrapper(ids);
        String normalizedFilename;
#ifdef OSRE_WINDOWS
        IO::Uri::normalizePath(filename, '\\', normalizedFilename);
#else
        normalizedFilename = filename;
#endif
        IO::Uri modelLoc(normalizedFilename);
        if (assimpWrapper.importAsset(modelLoc, flags)) {
            Assets::Model *model = assimpWrapper.getModel();
            Collision::TAABB<f32> aabb = model->getAABB();
            const f32 diam = aabb.getDiameter();
            const Vec3f center = aabb.getCenter();

            CPPCore::TArray<Geometry*> geoArray = model->getGeoArray();
            m_transformMatrix.m_model = glm::rotate(m_transformMatrix.m_model, 0.0f, glm::vec3(1, 1, 0));
            m_transformMatrix.update();
            RenderBackendService *rbSrv(getRenderBackendService());
            if (nullptr != rbSrv) {
                Platform::AbstractWindow *rootWindow(getRootWindow());
                if (nullptr == rootWindow) {
                    return false;
                }
                const i32 w = rootWindow->getProperties()->m_width;
                const i32 h = rootWindow->getProperties()->m_height;
                const f32 aspect = static_cast<f32>(w) / static_cast<f32>(h);
                const f32 zNear = 0.0001f;
                const f32 zFar = 1000.f;

                glm::vec3 eye(diam, 0, 0), up(0, 0, 1);
                glm::vec3 c(center.getX(), center.getY(), center.getZ());

                m_transformMatrix.m_model = glm::mat4(1.0f);
                m_transformMatrix.m_projection = glm::perspective(glm::radians(60.0f), aspect, zNear, zFar);
                m_transformMatrix.m_view = glm::lookAt(eye, c, up);
                rbSrv->setMatrix(MatrixType::View, m_transformMatrix.m_view);
                rbSrv->setMatrix(MatrixType::Projection, m_transformMatrix.m_projection);

                renderNodes(model, rbSrv);
            }
            const String name(model->getRootNode()->getName());
            m_modelNode = m_stage->addNode(name, nullptr, "default");

            update();
            requestNextFrame();
        }

        return 0;
    }

    void renderNode(Node *currentNode, RenderBackendService *rbSrv) {
        const ui32 numGeo = currentNode->getNumGeometries();
        for (ui32 i = 0; i < numGeo; ++i) {
            rbSrv->attachGeo(currentNode->getGeometryAt(i), 0);
        }

        for (ui32 i = 0; i < currentNode->getNumChildren(); ++i) {
            Node *current = currentNode->getChildAt(i);
            renderNode(current, rbSrv);
        }
    }

    void renderNodes(Assets::Model *model, RenderBackendService *rbSrv) {
        if (nullptr == model) {
            return;
        }

        Node *root = model->getRootNode();
        if (nullptr == root) {
            return;
        }

        renderNode(root, rbSrv);
    }

protected:
    bool onCreate(Properties::Settings *settings = nullptr) override {
        Properties::Settings *baseSettings(AppBase::getSettings());
        if (nullptr == baseSettings) {
            return false;
        }

        baseSettings->setString(Properties::Settings::WindowsTitle, "Demo UI!");
        if (!AppBase::onCreate(baseSettings)) {
            return false;
        }

        m_world = new World("HelloWorld", RenderMode::Render3D);
        m_stage = AppBase::createStage( "HelloWorld" );
        m_world->addStage(m_stage);

        AppBase::activateStage( m_stage->getName() );

        return true;
    }
};

static EditorApplication *s_EditorApplication = nullptr;

extern "C" OSRE_EDITOR_EXPORT int STDCALL CreateEditorApp( int *mainWindowHandle ) {
#ifdef OSRE_WINDOWS
    if (nullptr == s_EditorApplication) {
        HWND mainWH( nullptr );
        if ( nullptr != mainWindowHandle ) {
            mainWH = (HWND) mainWindowHandle;
        }
        char *argc[] = { "CreateEditorApp" };
        s_EditorApplication = new EditorApplication(1, argc );
        s_EditorApplication->create();
        s_EditorApplication->update();
        s_EditorApplication->requestNextFrame();

    }
#endif // OSRE_WINDOWS
    return 0;
}

int STDCALL EditorUpdate() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }

    s_EditorApplication->update();
    
    return 0;
}

int STDCALL EditorRequestNextFrame() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }

    s_EditorApplication->requestNextFrame();

    return 0;
}

int STDCALL DestroyEditorApp() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}

int STDCALL LoadWorld(const char *filelocation, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int STDCALL SaveWorld(const char *filelocation, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int STDCALL ImportAsset(const char *filename, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filename) {
        return 1;
    }

    return s_EditorApplication->importAsset(filename, flags);
}

int STDCALL EnqueueEvent( CSharpEvent *ev ) {
    if (nullptr == ev || nullptr == s_EditorApplication ) {
        return 1;
    }

    return s_EditorApplication->enqueueEvent();
}
using NodeArray = CPPCore::TArray<Node*>;

void countChildren( Node *node, ui32 &numNodes ) {
    if (nullptr == node) {
        return;
    }
    numNodes += node->getNumChildren();
    for (ui32 i = 0; i < node->getNumChildren(); ++i) {
        Node *child( node->getChildAt( i ) );
        if (nullptr != child) {
            countChildren( child, numNodes );
        }
    }
}
static void collectNodes( Node *node, NodeArray &nodeArray ) {
    nodeArray.add( node );
    for (ui32 i = 0; i < node->getNumChildren(); ++i) {
        Node *child( node->getChildAt( i ) );
        if ( nullptr != child ) {
            collectNodes( child, nodeArray );
        }
    }
}

static i32 getNodeIndex( Node *node, const NodeArray &nodeArray ) {
    if (nullptr == node) {
        return -1;
    }
    for (ui32 i = 0; i < nodeArray.size(); ++i) {
        if (node == nodeArray[ i ]) {
            return i;
        }
    }

    return -1;
}

static void addChildren( Node *node, NodeArray &nodeArray ) {
    if (nullptr == node) {
        return;
    }

    for (ui32 i = 0; i < node->getNumChildren(); ++i) {
        Node *currentNode( node->getChildAt( i ) );
        nodeArray.add( currentNode );
        addChildren( currentNode, nodeArray );
    }
}

int STDCALL GetNumItems() {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    World *world = s_EditorApplication->getWorld();
    if (nullptr == world) {
        return 0;
    }

    Stage *stage = world->getActiveStage();
    if (nullptr == stage) {
        return 1;
    }

    Node *rootNode = stage->getRoot();
    if (nullptr == rootNode) {
        return 0;
    }
    ui32 numNodes( 0 );
    countChildren( rootNode, numNodes );

    return static_cast<i32>(numNodes);
}

int STDCALL GetNodeHierarchy( int numItems, NativeStreeItem *items ) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == items) {
        return 0;
    }

    World *world = s_EditorApplication->getWorld();
    if (nullptr == world) {
        return 0;
    }

    Stage *stage = world->getActiveStage();
    if (nullptr == stage) {
        return 1;
    }

    Node *rootNode = stage->getRoot();
    if (nullptr == rootNode) {
        return 0;
    }
    CPPCore::TArray<Node*> nodeArray;
    collectNodes( rootNode, nodeArray );

    CPPCore::TArray<NativeStreeItem*> itemArray;
    const ui32 numChildren( rootNode->getNumChildren() );
    for (ui32 i = 0; i < numChildren; ++i) {
        Node *currentChild( rootNode->getChildAt( i ) );
        if (nullptr == currentChild) {
            continue;
        }

        NativeStreeItem *item = new NativeStreeItem;
        item->m_name = currentChild->getName();
        item->m_numChildren = currentChild->getNumChildren();
        item->m_childrenIds = new i32[ item->m_numChildren ];
        
        for (ui32 j = 0; j < item->m_numChildren; ++j) {
            item->m_childrenIds[ i ] = getNodeIndex( currentChild->getChildAt( j ), nodeArray );
        }
    }

    return 0;
}
