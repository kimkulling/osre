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
#include "Animation/AnimatorBase.h"

#include "App/AssetRegistry.h"
#include "App/ResourceCacheService.h"
#include "App/ServiceProvider.h"
#include "App/Scene.h"
#include "App/TransformController.h"
#include "Common/Environment.h"
#include "Debugging/osre_debugging.h"
#include "IO/IOService.h"
#include "Platform/AbstractPlatformEventQueue.h"
#include "Platform/AbstractTimer.h"
#include "Platform/AbstractWindow.h"
#include "Platform/PlatformInterface.h"
#include "Properties/Settings.h"
#include "RenderBackend/Pipeline.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/2D/CanvasRenderer.h"
#include "RenderBackend/MaterialBuilder.h"

#include "App/MouseEventListener.h"
#include "Platform/PlatformPluginFactory.h"

namespace OSRE::App {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Animation;
using namespace ::OSRE::Properties;
using namespace ::OSRE::IO;

DECL_OSRE_LOG_MODULE(AppBase)

static void attachMouseEventPtrs(EventPtrArray &eventArray) {
    eventArray.add(&MouseButtonDownEvent);
    eventArray.add(&MouseButtonUpEvent);
    eventArray.add(&MouseMoveEvent);
}

static void attachKeyboardEventPtrs(EventPtrArray &eventArray) {
    eventArray.add(&KeyboardButtonDownEvent);
    eventArray.add(&KeyboardButtonUpEvent);
}

AppBase::AppBase(i32 argc, const c8 *argv[], const String &supportedArgs, const String &desc) :
        mAppState(State::Uninited),
        mLastTime(0l),
        mArgParser(argc, argv, supportedArgs, desc),
        mEnvironment(nullptr),
        mSettings(new Settings),
        mPlatformInterface(nullptr),
        mTimer(nullptr),
        mRbService(nullptr),
        mActiveScene(nullptr),
        mMouseEvListener(nullptr),
        mKeyboardEvListener(nullptr),
        mIds(nullptr) {
    mSettings->setString(Settings::RenderAPI, "opengl");
    mSettings->setBool(Settings::PollingMode, true);
}

AppBase::~AppBase() {
    delete mSettings;
}

bool AppBase::initWindow(ui32 x, ui32 y, ui32 width, ui32 height, const String &title, WindowMode mode, WindowType type,
        RenderBackendType renderer) {
    osre_assert(nullptr != mSettings);

    mSettings->setInt(Settings::WinX, x);
    mSettings->setInt(Settings::WinY, y);
    mSettings->setInt(Settings::WinWidth, width);
    mSettings->setInt(Settings::WinHeight, height);
    mSettings->setBool(Settings::FullScreen, mode == WindowMode::Fullscreen);
    mSettings->setBool(Settings::WindowsMaximized, mode == WindowMode::WindowedMaximize);
    mSettings->setBool(Settings::ChildWindow, type == WindowType::Child);
    mSettings->setString(Settings::WindowsTitle, title);
    if (renderer == RenderBackendType::OpenGLRenderBackend) {
        mSettings->setString(Settings::RenderAPI, "opengl");
    } else if (renderer == RenderBackendType::VulkanRenderBackend) {
        mSettings->setString(Settings::RenderAPI, "vulkan");
    }

    return onCreate();
}

bool AppBase::create(Settings *config) {
    if (config != nullptr && config != mSettings) {
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
    if (mPlatformInterface != nullptr) {
        osre_debug(Tag, "Invalid platform interface.");
        return;
    }

    AbstractWindow *rootWindow = mPlatformInterface->getRootWindow();
    if (rootWindow != nullptr) {
        osre_debug(Tag, "Root window is nullptr.");
        return;
    }

    rootWindow->resize(x, y, w, h);

    // We need to update the render framebuffer as well
    if (mRbService != nullptr) {
        mRbService->resize(rootWindow->getId(), x, y, w, h);
    }
}

void AppBase::requestNextFrame() {
    osre_assert(mRbService != nullptr);
    if (mActiveScene == nullptr) {
        osre_debug(Tag, "Invalid active world.");
        return;
    }

    mActiveScene->render(mRbService);
    mRbService->update();
}

bool AppBase::handleEvents() {
    if (mPlatformInterface == nullptr) {
        osre_debug(Tag, "AppBase::PlatforInterface not in proper state: not nullptr.");
        return false;
    }

    const bool result = mPlatformInterface->update();
    if (shutdownRequested()) {
        return false;
    }

    return result;
}

Settings *AppBase::getSettings() const {
    return mSettings;
}

void AppBase::requestShutdown() {
    mShutdownRequested = true;
}

bool AppBase::shutdownRequested() const {
    return mShutdownRequested;
}

AbstractTimer *AppBase::getActiveTimer() const {
    return mTimer;
}

AnimationControllerBase *AppBase::getTransformController(TransformMatrixBlock &tmb) {
    return new TransformController(tmb);
}

AbstractWindow *AppBase::getRootWindow() const {
    if (mPlatformInterface == nullptr) {
        osre_debug(Tag, "Platform interface instance is nullptr.");
        return nullptr;
    }

    return mPlatformInterface->getRootWindow();
}

void AppBase::setWindowsTitle(const String &title) {
    if (mPlatformInterface == nullptr) {
        osre_debug(Tag, "Platform interface instance is nullptr.");
        return;
    }

    AbstractWindow *rs = mPlatformInterface->getRootWindow();
    if (rs != nullptr) {
        rs->setWindowsTitle(title);
    }
}

CanvasRenderer* AppBase::getCanvasRenderer() const {
    return static_cast<CanvasRenderer*>(mCanvasRenderer);
}

bool AppBase::onCreate() {
    if (mAppState != State::Uninited) {
        osre_debug(Tag, "AppBase::State not in expected state: Uninited.");
        return false;
    }

    ServiceProvider::create();
    mIds = new Ids;
    mEnvironment = new Environment;

    // create the asset registry
    if (AssetRegistry::create() == nullptr) {
        osre_debug(Tag, "Cannot create asset registry.");
        return false;
    }

    // Create the platform interface instance
    mPlatformInterface = PlatformInterface::create(mSettings);
    if (mPlatformInterface == nullptr) {
        osre_error(Tag, "Pointer to platform interface is nullptr.");
        return false;
    }

    if (!mPlatformInterface->open()) {
        osre_error(Tag, "Error while opening platform interface.");
        return false;
    }

    // Register any available platform-specific log streams
    AbstractLogStream *stream = PlatformPluginFactory::createPlatformLogStream();
    if (stream != nullptr) {
        Logger::getInstance()->registerLogStream(stream);
    }

    // Create the render back-end
    mRbService = new RenderBackendService();
    ServiceProvider::setService(ServiceType::RenderService, mRbService);
    mRbService->setSettings(mSettings, false);
    if (!mRbService->open()) {
        mRbService->release();
        mRbService = nullptr;
        return false;
    }

    // Create our world
    mActiveScene = new Scene("world");
    mScenes.add(mActiveScene);

    const String &api = mRbService->getSettings()->getString(Properties::Settings::RenderAPI);
    mEnvironment->addStrVar("api", api.c_str());

    mPlatformInterface->getPlatformEventHandler()->setRenderBackendService(mRbService);

    // Enable render-back-end
    AbstractWindow *rootWindow = mPlatformInterface->getRootWindow();
    CreateRendererEventData *data = new CreateRendererEventData(rootWindow);
    data->RequestedPipeline = mRbService->createDefault3DPipeline(rootWindow->getId());
    mRbService->sendEvent(&OnCreateRendererEvent, data);

    mTimer = PlatformInterface::getInstance()->getTimer();

    MaterialBuilder::create(GLSLVersion::GLSL_400);
    ResourceCacheService *rcSrv = new ResourceCacheService;
    ServiceProvider::setService(ServiceType::ResourceService, rcSrv);

    // Setup onMouse event-listener
    AbstractPlatformEventQueue *evHandler = mPlatformInterface->getPlatformEventHandler();
    if (nullptr != evHandler) {
        EventPtrArray eventArray;
        attachMouseEventPtrs(eventArray);
        mMouseEvListener = new MouseEventListener;
        evHandler->registerEventListener(eventArray, mMouseEvListener);

        eventArray.resize(0);
        attachKeyboardEventPtrs(eventArray);
        mKeyboardEvListener = new KeyboardEventListener;
        evHandler->registerEventListener(eventArray, mKeyboardEvListener);
    }

    AbstractService *ioSrv = IOService::create();
    ServiceProvider::setService(ServiceType::IOService, ioSrv);

    AssetRegistry::registerAssetPathInBinFolder("assets", "assets");

    Rect2ui rect;
    mPlatformInterface->getRootWindow()->getWindowsRect(rect);
    mCanvasRenderer = new CanvasRenderer(2, static_cast<i32>(rect.getX1()),
        static_cast<i32>(rect.getY1()),
        static_cast<i32>(rect.getWidth()),
        static_cast<i32>(rect.getHeight()));
    if (!mCanvasRenderer->create()) {
        osre_error(Tag, "Error while creating the canvas renderer.");
        return false;
    }

    mAppState = State::Created;
    osre_debug(Tag, "Set application state to Created.");

    return true;
}

bool AppBase::onDestroy() {
    if (mAppState != State::Running && mAppState != State::Created) {
        osre_debug(Tag, "AppBase::State not in proper state: Running.");
        return false;
    }

    AbstractPlatformEventQueue *evHandler = mPlatformInterface->getPlatformEventHandler();
    if (nullptr != evHandler) {
        EventPtrArray eventArray;
        attachMouseEventPtrs(eventArray);
        attachKeyboardEventPtrs(eventArray);
        evHandler->unregisterAllEventHandler(eventArray);
    }
    AssetRegistry::destroy();
    ResourceCacheService *service = ServiceProvider::getService<ResourceCacheService>(ServiceType::ResourceService);
    delete service;

    ServiceProvider::destroy();

    if (mPlatformInterface != nullptr) {
        PlatformInterface::destroy();
        mPlatformInterface = nullptr;
    }

    TextureLoader::releaseDefaultTexture();

    MaterialBuilder::destroy();

    delete mIds;
    mIds = nullptr;

    delete mCanvasRenderer;
    mCanvasRenderer = nullptr;

    delete mMouseEvListener;
    mMouseEvListener = nullptr;

    delete mKeyboardEvListener;
    mKeyboardEvListener = nullptr;

    osre_debug(Tag, "Set application state to destroyed.");
    mAppState = State::Destroyed;

#ifdef _DEBUG
    MemoryStatistics::showStatistics();
#endif
    Logger::kill();

    return true;
}

void AppBase::onUpdate() {
    const Time dt = mTimer->getTimeDiff();
    if (mActiveScene != nullptr) {
        mActiveScene->update(dt);
    }

    mKeyboardEvListener->clearKeyMap();
}

void AppBase::onRender() {
    if (mActiveScene != nullptr) {
        mActiveScene->render(mRbService);
    }
}

bool AppBase::isKeyPressed(Key key) const {
    if (nullptr == mKeyboardEvListener) {
        return false;
    }

    return mKeyboardEvListener->isKeyPressed(key);
}

void AppBase::getResolution(ui32 &width, ui32 &height) {
    width = height = 0;
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (rootWindow == nullptr) {
        return;
    }

    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);
    width = windowsRect.getWidth();
    height = windowsRect.getHeight();
}

} // namespace OSRE::App
