SET( LIBRARY_VERSION "0.1.0" )

SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/osre )
add_definitions( -DOSRE_BUILD_EXPORT )

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
	${HEADER_PATH}
    ${PROJECT_SOURCE_DIR}/3dparty/cppcore/include
    ${PROJECT_SOURCE_DIR}/3dparty/glew/include
    ${PROJECT_SOURCE_DIR}/3dparty/glm/
    ${PROJECT_SOURCE_DIR}/3dparty/unzip/
    ${PROJECT_SOURCE_DIR}/3dparty/zlib/
    ${PROJECT_SOURCE_DIR}/3dparty/assimp/include
)

# For Vulkan support
if( NOT USE_PLATFORM MATCHES "VK_USE_PLATFORM_.*" )
    if( WIN32 )
        set( USE_PLATFORM "VK_USE_PLATFORM_WIN32_KHR" )
    elseif( UNIX )
        set( USE_PLATFORM "VK_USE_PLATFORM_XCB_KHR" )
    endif()
endif()

IF( WIN32 )
    SET( platform_libs comctl32.lib Winmm.lib opengl32.lib glu32.lib Shcore.lib SDL2 )
ELSE( WIN32 )
    SET( platform_libs SDL2 )
ENDIF( WIN32 )

#==============================================================================
# App
#==============================================================================
SET( app_inc
    ${HEADER_PATH}/App/AppCommon.h
    ${HEADER_PATH}/App/AbstractBehaviour.h
    ${HEADER_PATH}/App/AppBase.h
    ${HEADER_PATH}/App/Component.h
    ${HEADER_PATH}/App/Entity.h
    ${HEADER_PATH}/App/ServiceProvider.h
    ${HEADER_PATH}/App/Project.h
    ${HEADER_PATH}/App/TAbstractCtrlBase.h
    ${HEADER_PATH}/App/ResourceCacheService.h
    ${HEADER_PATH}/App/AssetsCommon.h
    ${HEADER_PATH}/App/AssetRegistry.h
    ${HEADER_PATH}/App/AssetDataArchive.h
    ${HEADER_PATH}/App/AssimpWrapper.h
    ${HEADER_PATH}/App/World.h
)

SET( app_src
    App/AppBase.cpp
    App/Component.cpp
    App/Entity.cpp
    App/ServiceProvider.cpp
    App/Project.cpp
    App/AssetRegistry.cpp
    App/AssetDataArchive.cpp
    App/AssimpWrapper.cpp
    App/World.cpp
    App/MouseEventListener.cpp
    App/MouseEventListener.h
)

#==============================================================================
# Common
#==============================================================================
SET( common_inc
    ${HEADER_PATH}/Common/Common.h
    ${HEADER_PATH}/Common/Environment.h
    ${HEADER_PATH}/Common/TCommand.h
    ${HEADER_PATH}/Common/AbstractEventHandler.h
    ${HEADER_PATH}/Common/AbstractProcessor.h
    ${HEADER_PATH}/Common/ArgumentParser.h
    ${HEADER_PATH}/Common/AbstractService.h
    ${HEADER_PATH}/Common/ColorRGBA.h
    ${HEADER_PATH}/Common/DateTime.h
    ${HEADER_PATH}/Common/Event.h
    ${HEADER_PATH}/Common/EventTriggerer.h
    ${HEADER_PATH}/Common/Ids.h
    ${HEADER_PATH}/Common/Logger.h
    ${HEADER_PATH}/Common/Object.h
    ${HEADER_PATH}/Common/StringUtils.h
    ${HEADER_PATH}/Common/TFunctor.h
    ${HEADER_PATH}/Common/TResource.h
    ${HEADER_PATH}/Common/TResourceCache.h
    ${HEADER_PATH}/Common/TObjPtr.h
    ${HEADER_PATH}/Common/Tokenizer.h
    ${HEADER_PATH}/Common/osre_common.h
)
SET( common_src
    Common/ArgumentParser.cpp
    Common/Common.cpp
    Common/DateTime.cpp
    Common/Event.cpp
    Common/EventTriggerer.cpp
    Common/Environment.cpp
    Common/Ids.cpp
    Common/Logger.cpp
    Common/Object.cpp
    Common/Tokenizer.cpp
)

#==============================================================================
# Debugging
#==============================================================================
SET( debugging_inc
    ${HEADER_PATH}/Debugging/AssertHandler.h
    ${HEADER_PATH}/Debugging/osre_debugging.h
    ${HEADER_PATH}/Debugging/MeshDiagnostic.h
)
SET( debugging_src
    Debugging/Debug.cpp
    Debugging/AssertHandler.cpp
    Debugging/MeshDiagnostic.cpp
)

#==============================================================================
# Documentation
#==============================================================================
SET( doc_inc
    ${CMAKE_HOME_DIRECTORY}/doc/Doxygen/osre_doc.h
)

#==============================================================================
# IO
#==============================================================================
SET( io_src
    IO/Directory.cpp
    IO/FileStream.cpp
    IO/FileStream.h
    IO/IOService.cpp
    IO/LocaleFileSystem.cpp
    IO/LocaleFileSystem.h
    IO/MemoryStream.h
    IO/Stream.cpp
    IO/Uri.cpp
    IO/ZipFileSystem.cpp
    IO/ZipFileSystem.h
    IO/ZipFileStream.cpp
    IO/ZipFileStream.h
    IO/IOSystemInfo.cpp
)
SET( io_inc
    ${HEADER_PATH}/IO/IOCommon.h
    ${HEADER_PATH}/IO/Directory.h
    ${HEADER_PATH}/IO/Stream.h
    ${HEADER_PATH}/IO/AbstractFileSystem.h
    ${HEADER_PATH}/IO/IOService.h
    ${HEADER_PATH}/IO/IOSystemInfo.h
    ${HEADER_PATH}/IO/Uri.h
)

#==============================================================================
# Math
#==============================================================================
SET( math_src
    Math/BaseMath.cpp
)
SET( math_inc
    ${HEADER_PATH}/Math/BaseMath.h
    ${HEADER_PATH}/Math/TRay.h
)

#==============================================================================
# Platform
#==============================================================================
SET( platform_inc
    ${HEADER_PATH}/Platform/PlatformCommon.h
    ${HEADER_PATH}/Platform/AbstractOSService.h
    ${HEADER_PATH}/Platform/Threading.h
    ${HEADER_PATH}/Platform/AbstractPlatformEventQueue.h
    ${HEADER_PATH}/Platform/AbstractOGLRenderContext.h
    ${HEADER_PATH}/Platform/AbstractWindow.h
    ${HEADER_PATH}/Platform/AbstractTimer.h
    ${HEADER_PATH}/Platform/AbstractSystemInfo.h
    ${HEADER_PATH}/Platform/CPUInfo.h
    ${HEADER_PATH}/Platform/SystemInfo.h
    ${HEADER_PATH}/Platform/SystemUtils.h
    ${HEADER_PATH}/Platform/KeyTypes.h
    ${HEADER_PATH}/Platform/PluginType.h
    ${HEADER_PATH}/Platform/PlatformInterface.h
    ${HEADER_PATH}/Platform/PlatformOperations.h
    ${HEADER_PATH}/Platform/AbstractDisplayInterface.h
)
SET( platform_win_inc
    ${HEADER_PATH}/Platform/Windows/MinWindows.h
)
SET( platform_src
    Platform/CPUInfo.cpp
    Platform/SystemInfo.cpp
    Platform/SystemUtils.cpp
    Platform/AbstractWindow.cpp
    Platform/PlatformInterface.cpp
    Platform/PlatformPluginFactory.cpp
    Platform/PlatformPluginFactory.h
    Platform/AbstractDynamicLoader.cpp
    Platform/PlatformOperations.cpp
)
SET( platform_win32_src
    Platform/win32/Win32DisplayInterface.h
    Platform/win32/Win32DisplayInterface.cpp
    Platform/win32/Win32SystemInfo.h
    Platform/win32/Win32DbgLogStream.h
    Platform/win32/Win32Window.cpp
    Platform/win32/Win32Window.h
    Platform/win32/Win32EventQueue.cpp
    Platform/win32/Win32EventQueue.h
    Platform/win32/Win32OGLRenderContext.cpp
    Platform/win32/Win32OGLRenderContext.h
    Platform/win32/Win32Timer.cpp
    Platform/win32/Win32Timer.h
    Platform/win32/Win32Timer.cpp
    Platform/win32/Win32Timer.h
    Platform/win32/Win32DynamicLoader.cpp
    Platform/win32/Win32DynamicLoader.h
    Platform/win32/Win32OSService.cpp
    Platform/win32/Win32OSService.h
    Platform/win32/Threading_win32.cpp
)
SET( platform_sdl2_src
    Platform/sdl2/Threading_sdl2.cpp
    Platform/sdl2/SDL2DisplayInterface.h
    Platform/sdl2/SDL2DisplayInterface.cpp
    Platform/sdl2/SDL2EventQueue.cpp
    Platform/sdl2/SDL2EventQueue.h
    Platform/sdl2/SDL2Initializer.cpp
    Platform/sdl2/SDL2Initializer.h
    Platform/sdl2/SDL2Window.cpp
    Platform/sdl2/SDL2Window.h
    Platform/sdl2/SDL2Timer.cpp
    Platform/sdl2/SDL2Timer.h
    Platform/sdl2/SDL2OGLRenderContext.cpp
    Platform/sdl2/SDL2OGLRenderContext.h
    Platform/sdl2/SDL2ThreadLocalStorage.h
    Platform/sdl2/SDL2DynamicLoader.cpp
    Platform/sdl2/SDL2DynamicLoader.h
    Platform/sdl2/SDL2SystemInfo.h
    Platform/sdl2/SDL2OSService.h
    Platform/sdl2/SDL2OSService.cpp
)

IF( WIN32 )
    SET( platform_impl_src ${platform_win32_src} )
ELSE()
    SET( platform_impl_src ${platform_sdl2_src} )
ENDIF()

#==============================================================================
# Profiling
#==============================================================================
SET( profiling_inc
    ${HEADER_PATH}/Profiling/ProfilingCommon.h
    ${HEADER_PATH}/Profiling/FPSCounter.h
    ${HEADER_PATH}/Profiling/PerformanceCounterRegistry.h
)
SET( profiling_src
    Profiling/FPSCounter.cpp
    Profiling/PerformanceCounterRegistry.cpp
)

#==============================================================================
# Properties
#==============================================================================
SET ( properties_inc
    ${HEADER_PATH}/Properties/PropertiesCommon.h
    ${HEADER_PATH}/Properties/BasePropertyMap.h
    ${HEADER_PATH}/Properties/AbstractPropertyMap.h
    ${HEADER_PATH}/Properties/Property.h
    ${HEADER_PATH}/Properties/Settings.h
)
SET ( properties_src
    Properties/BasePropertyMap.cpp
    Properties/Settings.cpp
    Properties/Property.cpp
)

#==============================================================================
# Scene
#==============================================================================
SET( scene_inc
    ${HEADER_PATH}/Scene/SceneCommon.h
    ${HEADER_PATH}/Scene/MeshProcessor.h
    ${HEADER_PATH}/Scene/MeshBuilder.h
    ${HEADER_PATH}/Scene/AnimatorBase.h
    ${HEADER_PATH}/Scene/DbgRenderer.h
    ${HEADER_PATH}/Scene/MaterialBuilder.h
    ${HEADER_PATH}/Scene/Node.h
    ${HEADER_PATH}/Scene/TrackBall.h
    ${HEADER_PATH}/Scene/Camera.h
    ${HEADER_PATH}/Scene/LineBuilder.h
    ${HEADER_PATH}/Scene/TAABB.h
    ${HEADER_PATH}/Scene/TQuadTree.h
    ${HEADER_PATH}/Scene/ParticleEmitter.h
)
SET( scene_src
    Scene/DbgRenderer.cpp
    Scene/MeshProcessor.cpp
    Scene/MeshBuilder.cpp
    Scene/LineBuilder.cpp
    Scene/MaterialBuilder.cpp
    Scene/Node.cpp
    Scene/TrackBall.cpp
    Scene/Camera.cpp
    Scene/ParticleEmitter.cpp
)

#==============================================================================
# RenderBackend
#==============================================================================
SET( renderbackend_inc
    ${HEADER_PATH}/RenderBackend/RenderCommon.h
    ${HEADER_PATH}/RenderBackend/Mesh.h
    ${HEADER_PATH}/RenderBackend/THWBufferManager.h
    ${HEADER_PATH}/RenderBackend/Pipeline.h
    ${HEADER_PATH}/RenderBackend/RenderBackendService.h
    ${HEADER_PATH}/RenderBackend/RenderStates.h
    ${HEADER_PATH}/RenderBackend/Shader.h
)
SET( renderbackend_src
    RenderBackend/Mesh.cpp
    RenderBackend/RenderBackendService.cpp
    RenderBackend/RenderCommon.cpp
    RenderBackend/Pipeline.cpp
    RenderBackend/THWBufferManager.cpp
    RenderBackend/Shader.cpp
)
SET( renderbackend_oglrenderer_src
    RenderBackend/OGLRenderer/OGLCommon.h
    RenderBackend/OGLRenderer/OGLCommon.cpp
    RenderBackend/OGLRenderer/OGLRenderCommands.h
    RenderBackend/OGLRenderer/OGLRenderCommands.cpp
    RenderBackend/OGLRenderer/OGLEnum.cpp
    RenderBackend/OGLRenderer/OGLEnum.h
    RenderBackend/OGLRenderer/OGLRenderBackend.cpp
    RenderBackend/OGLRenderer/OGLRenderBackend.h
    RenderBackend/OGLRenderer/RenderCmdBuffer.cpp
    RenderBackend/OGLRenderer/RenderCmdBuffer.h
    RenderBackend/OGLRenderer/OGLRenderEventHandler.cpp
    RenderBackend/OGLRenderer/OGLRenderEventHandler.h
    RenderBackend/OGLRenderer/OGLShader.cpp
    RenderBackend/OGLRenderer/OGLShader.h
)
SET( renderbackend_vulkanrenderer_src
    RenderBackend/VulkanRenderer/VlkFunctions.h
    RenderBackend/VulkanRenderer/VlkFunctions.cpp
    RenderBackend/VulkanRenderer/VlkExportedFunctions.inl
    RenderBackend/VulkanRenderer/VlkCommon.h
    RenderBackend/VulkanRenderer/VlkRenderBackend.cpp
    RenderBackend/VulkanRenderer/VlkRenderBackend.h
    RenderBackend/VulkanRenderer/VlkRenderEventHandler.h
    RenderBackend/VulkanRenderer/VlkRenderEventHandler.cpp
)

#==============================================================================
# Scripting with Mono
#==============================================================================
SET( scripting_mono_src
)
SET( scripting_mono_inc
	${HEADER_PATH}/Scripting/osre_scripting.h
)

#==============================================================================
# UI
#==============================================================================
SET( ui_common_src
    UI/Common/Widget.cpp
)

SET( ui_src
    UI/UICommon.cpp
    UI/Image.cpp
    UI/FocusControl.cpp
    UI/UiRenderer.cpp
    UI/UIRenderUtils.cpp
    UI/UIRenderUtils.h
    UI/FontBase.cpp
    UI/FontRenderer.cpp
    UI/FontRenderer.h
    UI/ButtonBase.cpp
    UI/Panel.cpp
    UI/Canvas.cpp
    UI/TextBase.cpp
    UI/UiItemFactory.cpp
    UI/StyleProvider.cpp
)
SET( ui_inc
    ${HEADER_PATH}/UI/UICommon.h
    ${HEADER_PATH}/UI/StyleProvider.h
    ${HEADER_PATH}/UI/FocusControl.h
    ${HEADER_PATH}/UI/FontBase.h
    ${HEADER_PATH}/UI/Layouter.h
    ${HEADER_PATH}/UI/Image.h
    ${HEADER_PATH}/UI/Widget.h
    ${HEADER_PATH}/UI/ButtonBase.h
    ${HEADER_PATH}/UI/Panel.h
    ${HEADER_PATH}/UI/Canvas.h
    ${HEADER_PATH}/UI/TextBase.h
    ${HEADER_PATH}/UI/UiRenderer.h
    ${HEADER_PATH}/UI/UiItemFactory.h
)
SET( ui_winmanager_src
    UI/WinManager/WMButton.h
    UI/WinManager/WMDialog.h
    UI/WinManager/WMDialog.cpp
)

#==============================================================================
# Threading
#==============================================================================
SET( threading_inc
    ${HEADER_PATH}/Threading/ThreadingCommon.h
    ${HEADER_PATH}/Threading/AbstractTask.h
    ${HEADER_PATH}/Threading/SystemTask.h
    ${HEADER_PATH}/Threading/TaskJob.h
    ${HEADER_PATH}/Threading/TAsyncQueue.h
)
SET( threading_src
    Threading/AbstractTask.cpp
    Threading/SystemTask.cpp
)

if( NOT USE_PLATFORM MATCHES "VK_USE_PLATFORM_.*" )
	if( WIN32 )
		set( USE_PLATFORM "VK_USE_PLATFORM_WIN32_KHR" )
	elseif( UNIX )
		set( USE_PLATFORM "VK_USE_PLATFORM_XCB_KHR" )
	endif()
endif()

#==============================================================================
# Defined Source groups
#==============================================================================
SOURCE_GROUP( App                 FILES ${app_src} )
SOURCE_GROUP( Assets              FILES ${assets_src} )
SOURCE_GROUP( Common              FILES ${common_src} )
SOURCE_GROUP( Components          FILES ${components_src} )
SOURCE_GROUP( Debugging           FILES ${debugging_src} )
SOURCE_GROUP( Debugging\\Win32    FILES ${debugging_windows_src} )
SOURCE_GROUP( IO                  FILES ${io_src} )
SOURCE_GROUP( Math                FILES ${math_src} )
SOURCE_GROUP( Memory              FILES ${memory_src} )
SOURCE_GROUP( Platform            FILES ${platform_src} )
SOURCE_GROUP( Platform\\Win32     FILES ${platform_impl_src} )
SOURCE_GROUP( Platform\\sdl2      FILES ${platform_sdl2_src} )
SOURCE_GROUP( Profiling           FILES ${profiling_src} )
SOURCE_GROUP( Properties          FILES ${properties_src} )
SOURCE_GROUP( RenderBackend       FILES ${renderbackend_src} )
SOURCE_GROUP( RenderBackend\\OGLRenderer    FILES ${renderbackend_oglrenderer_src} )
SOURCE_GROUP( RenderBackend\\VulkanRenderer FILES ${renderbackend_vulkanrenderer_src} )
SOURCE_GROUP( Resources           FILES ${resources_src} )
SOURCE_GROUP( Scene               FILES ${scene_src} )
SOURCE_GROUP( Threading           FILES ${threading_src} )
SOURCE_GROUP( UI                  FILES ${ui_src} )
SOURCE_GROUP( UI\\Common          FILES ${ui_common_src} )

SOURCE_GROUP( Include\\osre\\App        FILES ${app_inc} )
SOURCE_GROUP( Include\\osre\\Assets     FILES ${assets_inc} )
SOURCE_GROUP( Include\\osre\\Debugging  FILES ${debugging_inc} )
SOURCE_GROUP( Include\\osre\\Common     FILES ${common_inc} )
SOURCE_GROUP( Include\\osre\\Debugging  FILES ${debugging_inc} )
SOURCE_GROUP( Include\\osre\\System     FILES ${system_inc} )
SOURCE_GROUP( Include\\osre\\Utils      FILES ${utils_inc} )
SOURCE_GROUP( Include\\osre\\IO         FILES ${io_inc} )
SOURCE_GROUP( Include\\osre\\Math              FILES ${math_inc} )
SOURCE_GROUP( Include\\osre\\Memory            FILES ${memory_inc} )
SOURCE_GROUP( Include\\osre\\Properties        FILES ${properties_inc} )
SOURCE_GROUP( Include\\osre\\Platform          FILES ${platform_inc} )
SOURCE_GROUP( Include\\osre\\Platform\\Windows FILES ${platform_win_inc} )
SOURCE_GROUP( Include\\osre\\Profiling         FILES ${profiling_inc} )
SOURCE_GROUP( Include\\osre\\Resources         FILES ${resources_inc} )
SOURCE_GROUP( Include\\osre\\RenderBackend     FILES ${renderbackend_inc} )
SOURCE_GROUP( Include\\osre\\Scene             FILES ${scene_inc} )
SOURCE_GROUP( Include\\osre\\Scripting         FILES ${scripting_mono_inc} )
SOURCE_GROUP( Include\\osre\\Threading         FILES ${threading_inc} )
SOURCE_GROUP( Include\\osre\\UI                FILES ${ui_inc} )
SOURCE_GROUP( Include\\osre\\UI\\Ingame        FILES ${ui_ingame_src} )
SOURCE_GROUP( Include\\osre\\UI\\WinManager    FILES ${ui_winmanager_src} )
SOURCE_GROUP( Documentation                    FILES ${doc_inc} )

add_definitions( -D${USE_PLATFORM} )

#==============================================================================
# Define the Infrastructure library
#==============================================================================
ADD_LIBRARY( osre SHARED
    ../../README.md
    ${app_inc}
    ${debugging_inc}
    ${doc_inc}
    ${common_inc}
    ${common_src}
    ${collision_inc}
    ${collision_src}
    ${components_inc}
    ${debugging_inc}
    ${system_inc}
    ${io_inc}
    ${math_inc}
    ${memory_inc}
    ${utils_inc}
    ${properties_inc}
    ${platform_inc}
	${platform_win_inc}
    ${profiling_inc}
    ${resources_inc}
    ${scene_inc}
	${scripting_mono_inc}
    ${threading_inc}
    ${renderbackend_inc}
    ${ui_inc}
    ${math_inc}

    ${app_src}
    ${Common_src}
    ${components_src}
    ${debugging_src}
        ${debugging_windows_src}
    ${io_src}
    ${math_src}
    ${memory_src}
    ${system_src}
        ${system_impl_src}
    ${properties_src}
    ${profiling_src}
    ${platform_src}
        ${platform_impl_src}
    ${utils_src}
    ${resources_src}
    ${renderbackend_src}
        ${renderbackend_oglrenderer_src}
        ${renderbackend_vulkanrenderer_src}
    ${scene_src}
    ${threading_src}
        ${threading_impl_src}
    ${ui_src}
    ${ui_common_src}
    ${ui_winmanager_src}
)

target_link_libraries ( osre cppcore zlib Unzip glew soil jsoncpp_lib_static ${platform_libs} assimp )
set_target_properties(  osre PROPERTIES FOLDER Engine )
