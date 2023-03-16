/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Animation/AnimatorBase.h>

#include <osre/App/AssetRegistry.h>
#include <osre/App/ResourceCacheService.h>
#include <osre/App/ServiceProvider.h>
#include <osre/App/World.h>
#include <osre/App/Stage.h>
#include <osre/App/TransformController.h>
#include <osre/Common/Environment.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/IOService.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/App/CameraComponent.h>
#include <osre/RenderBackend/MaterialBuilder.h>

#include "src/Engine/App/MouseEventListener.h"
#include "src/Engine/Platform/PlatformPluginFactory.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Animation;
using namespace ::OSRE::IO;

static constexpr c8 Tag[] = "AppBase";

AppBase::AppBase(i32 argc, const c8 *argv[], const String &supportedArgs, const String &desc) :
        mAppState(State::Uninited),
        mArgParser(argc, argv, supportedArgs, desc),
        mEnvironment(nullptr),
        mSettings(new Properties::Settings),
        mPlatformInterface(nullptr),
        mTimer(nullptr),
        mRbService(nullptr),
        mWorlds(),
        mStage(nullptr),
        mMouseEvListener(nullptr),
        mKeyboardEvListener(nullptr),
        mIds(nullptr),
        mShutdownRequested(false) {
    mSettings->setString(Properties::Settings::RenderAPI, "opengl");
    mSettings->setBool(Properties::Settings::PollingMode, true);
}

AppBase::~AppBase() {
    delete mSettings;
    mSettings = nullptr;
}

bool AppBase::initWindow(ui32 x, ui32 y, ui32 width, ui32 height, const String &title, bool fullscreen,
        RenderBackendType renderer) {
    osre_assert(nullptr != mSettings);

    mSettings->setInt(Properties::Settings::WinX, x);
    mSettings->setInt(Properties::Settings::WinY, y);
    mSettings->setInt(Properties::Settings::WinWidth, width);
    mSettings->setInt(Properties::Settings::WinHeight, height);
    mSettings->setBool(Properties::Settings::FullScreen, fullscreen);
    mSettings->setString(Properties::Settings::WindowsTitle, title);
    if (renderer == RenderBackendType::OpenGLRenderBackend) {
        mSettings->setString(Properties::Settings::RenderAPI, "opengl");
    } else if (renderer == RenderBackendType::VulkanRenderBackend) {
        mSettings->setString(Properties::Settings::RenderAPI, "vulkan");
    }

    return onCreate();
}

bool AppBase::create(Properties::Settings *config) {
    if (nullptr != config && config != mSettings) {
        delete mSettings;
        mSettings = config;
    }

    return onCreate();
}

bool AppBase::destroy() {
    if (mAppState == State::Created || mAppState == State::Running) {
        return onDestroy();
    }
    return false;
}

void AppBase::update() {
    if (mAppState == State::Created) {
        mAppState = State::Running;
    }
    
    onUpdate();
}

void AppBase::resize(i32 x, i32 y, i32 w, i32 h) {
    if (nullptr == mPlatformInterface) {
        return;
    }

    AbstractWindow *rootWindow = mPlatformInterface->getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }

    rootWindow->resize(x, y, w, h);
}

void AppBase::requestNextFrame() {
    osre_assert(nullptr != mRbService);

    if (mStage == nullptr) {
        return;
    }

    mStage->render(mRbService);
    mRbService->update();
}

bool AppBase::handleEvents() {
    if (nullptr == mPlatformInterface) {
        osre_debug(Tag, "AppBase::PlatforInterface not in proper state: not nullptr.");
        return false;
    }

    const bool result = mPlatformInterface->update();
    if (shutdownRequested()) {
        return false;
    }

    return result;
}

Properties::Settings *AppBase::getSettings() const {
    return mSettings;
}

Camera *AppBase::setActiveCamera(Camera *camera) {
    if (nullptr == mStage) {
        osre_debug(Tag, "No world to activate state to.");
        return nullptr;
    }

    World *activeWorld = mStage->getActiveWorld();
    if (activeWorld == nullptr) {
        return nullptr;
    }
    
    if (!activeWorld->setActiveCamera(camera)) {
        return nullptr;
    }
    
    return camera;
}

void AppBase::requestShutdown() {
    mShutdownRequested = true;
}

bool AppBase::shutdownRequested() const {
    return mShutdownRequested;
}

Platform::AbstractTimer *AppBase::getActiveTimer() const {
    return mTimer;
}

AnimationControllerBase *AppBase::getTransformController(TransformMatrixBlock &tmb) {
    return new TransformController(tmb);
}

Platform::AbstractWindow *AppBase::getRootWindow() const {
    if (nullptr == mPlatformInterface) {
        return nullptr;
    }

    return mPlatformInterface->getRootWindow();
}

void AppBase::setWindowsTitle(const String &title) {
    if (nullptr == mPlatformInterface) {
        return;
    }

    AbstractWindow *rs = mPlatformInterface->getRootWindow();
    if (nullptr != rs) {
        rs->setWindowsTitle(title);
    }
}

bool AppBase::onCreate() {
    if (mAppState != State::Uninited) {
        osre_debug(Tag, "AppBase::State not in proper state: Uninited.");
        return false;
    }

    ServiceProvider::create();
    mIds = new Common::Ids;
    mEnvironment = new Common::Environment;

    // create the asset registry
    AssetRegistry *registry = AssetRegistry::create();
    if (nullptr == registry) {
        osre_debug(Tag, "Cannot create asset registry.");
    }

    // create the platform interface instance
    mPlatformInterface = Platform::PlatformInterface::create(mSettings);
    if (nullptr == mPlatformInterface) {
        osre_error(Tag, "Pointer to platform interface is nullptr.");
        return false;
    }

    if (!mPlatformInterface->open()) {
        osre_error(Tag, "Error while opening platform interface.");
        return false;
    }

    // register any available platform-specific log streams
    Common::AbstractLogStream *stream = Platform::PlatformPluginFactory::createPlatformLogStream();
    if (nullptr != stream) {
        Logger::getInstance()->registerLogStream(stream);
    }

    // create the render back-end
    mRbService = new RenderBackendService();
    ServiceProvider::setService(ServiceType::RenderService, mRbService);
    mRbService->setSettings(mSettings, false);
    if (!mRbService->open()) {
        mRbService->release();
        mRbService = nullptr;
        return false;
    }

    // Create our world
    mStage = new Stage("stage");
    mStage->createWorld("world");

    const String &api = mRbService->getSettings()->getString(Properties::Settings::RenderAPI);
    mEnvironment->addStrVar("api", api.c_str());

    mPlatformInterface->getPlatformEventHandler()->setRenderBackendService(mRbService);

    // enable render-back-end
    RenderBackend::CreateRendererEventData *data = new CreateRendererEventData(mPlatformInterface->getRootWindow());
    data->m_pipeline = mRbService->createDefaultPipeline();
    mRbService->sendEvent(&RenderBackend::OnCreateRendererEvent, data);

    mTimer = Platform::PlatformInterface::getInstance()->getTimer();

    MaterialBuilder::create();
    ResourceCacheService *rcSrv = new ResourceCacheService;
    ServiceProvider::setService(ServiceType::ResourceService, rcSrv);
    // Setup onMouse event-listener
    AbstractPlatformEventQueue *evHandler = mPlatformInterface->getPlatformEventHandler();
    if (nullptr != evHandler) {
        TArray<const Common::Event *> eventArray;
        eventArray.add(&MouseButtonDownEvent);
        eventArray.add(&MouseButtonUpEvent);
        eventArray.add(&MouseMoveEvent);
        mMouseEvListener = new MouseEventListener;
        evHandler->registerEventListener(eventArray, mMouseEvListener);

        eventArray.resize(0);
        eventArray.add(&KeyboardButtonDownEvent);
        eventArray.add(&KeyboardButtonUpEvent);
        mKeyboardEvListener = new KeyboardEventListener;
        evHandler->registerEventListener(eventArray, mKeyboardEvListener);
    }

    Common::AbstractService *ioSrv = IO::IOService::create();
    ServiceProvider::setService(ServiceType::IOService, ioSrv);

    App::AssetRegistry::registerAssetPathInBinFolder("assets", "assets");

    mAppState = State::Created;
    osre_debug(Tag, "Set application state to Created.");

    return true;
}

bool AppBase::onDestroy() {
    if (mAppState != State::Running) {
        osre_debug(Tag, "AppBase::State not in proper state: Running.");
        return false;
    }

    AbstractPlatformEventQueue *evHandler = mPlatformInterface->getPlatformEventHandler();
    if (nullptr != evHandler) {
        Common::EventPtrArray eventArray;
        eventArray.add(&MouseButtonDownEvent);
        eventArray.add(&MouseButtonUpEvent);
        eventArray.add(&KeyboardButtonDownEvent);
        eventArray.add(&KeyboardButtonUpEvent);
        evHandler->unregisterAllEventHandler(eventArray);
    }
    AssetRegistry::destroy();
    ResourceCacheService *service = ServiceProvider::getService<ResourceCacheService>(ServiceType::ResourceService);
    delete service;

    ServiceProvider::destroy();

    if (mPlatformInterface) {
        Platform::PlatformInterface::destroy();
        mPlatformInterface = nullptr;
    }

    MaterialBuilder::destroy();

    delete mStage;
    mStage = nullptr;

    delete mIds;
    mIds = nullptr;

    delete mMouseEvListener;
    mMouseEvListener = nullptr;

    delete mKeyboardEvListener;
    mKeyboardEvListener = nullptr;

    
    osre_debug(Tag, "Set application state to destroyed.");
    mAppState = State::Destroyed;
    Logger::kill();

    return true;
}

static constexpr i64 Conversion2Micro = 1000;

void AppBase::onUpdate() {
    i64 microsecs = mTimer->getMilliCurrentSeconds() * Conversion2Micro;
    Time dt(microsecs);    

    if (nullptr != mStage) {
        mStage->update(dt);
    }

    mKeyboardEvListener->clearKeyMap();
}

void AppBase::onRender() {
    if (nullptr != mStage) {
        mStage->render(mRbService);
    }
}

const ArgumentParser &AppBase::getArgumentParser() const {
    return mArgParser;
}

Ids *AppBase::getIdContainer() const {
    return mIds;
}

bool AppBase::isKeyPressed(Key key) const {
    if (nullptr == mKeyboardEvListener) {
        return false;
    }

    return mKeyboardEvListener->isKeyPressed(key);
}

void AppBase::getResolution(ui32 &width, ui32 &height) {
    width = height = 0;
    Rect2ui windowsRect;
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }

    rootWindow->getWindowsRect(windowsRect);
    width = windowsRect.getWidth();
    height = windowsRect.getHeight();
}

} // Namespace App
} // Namespace OSRE
