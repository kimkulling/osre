#include <osre/Common/osre_common.h>
#include <osre/App/AppBase.h>

using namespace ::OSRE;

class EditorApplication : public App::AppBase {
public:
    EditorApplication(int argc, char *argv[])
    : AppBase(argc, argv) {
        // empty
    }

    virtual ~EditorApplication() {
        // empty
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

        return true;
    }
};

static EditorApplication *s_EditorApplication = nullptr;

int CreateEditorApp() {
    if (nullptr != s_EditorApplication) {
        s_EditorApplication = new EditorApplication(1, "CreateEditorApp");
    }

    return 0;
}

int EditorUpdate() {
    s_EditorApplication->update();
    
    return 0;
}

int EditorRequestNextFrame() {
    s_EditorApplication->requestNextFrame();

    return 0;
}

int DestroyEditorApp() {
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}
