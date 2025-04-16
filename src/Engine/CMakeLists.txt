SET( LIBRARY_VERSION "0.1.1" )

add_definitions( -DOSRE_BUILD_EXPORT )

INCLUDE_DIRECTORIES(
    engine
    contrib/cppcore/include
    contrib/glew/include
    contrib/glm/
    contrib/unzip/
    contrib/zlib/
    contrib/zlib/
    contrib/assimp/include
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
    SET(platform_libs comctl32.lib Winmm.lib opengl32.lib glu32.lib Shcore.lib cppcore)
ELSE( WIN32 )
    SET(platform_libs 
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        cppcore)
ENDIF( WIN32 )

#==============================================================================
# Animation
#==============================================================================
SET(animation_src
    Animation/AnimatorBase.h
    Animation/AnimatorComponent.h
    Animation/AnimatorComponent.cpp
)

#==============================================================================
# App
#==============================================================================
SET( app_src
    App/App.h
    App/AppCommon.h
    App/AssetBundle.h
    App/AssetRegistry.h
    App/TransformComponent.h
    App/TransformComponent.cpp
    App/CameraComponent.h
    App/CameraComponent.cpp
    App/ParticleEmitter.h
    App/ParticleEmitter.cpp
    App/AppBase.h
    App/AppBase.cpp
    App/Component.h
    App/Component.cpp
    App/Entity.h
    App/Entity.cpp
    App/KeyboardEventListener.cpp
    App/KeyboardEventListener.h
    App/ServiceProvider.h
    App/ServiceProvider.cpp
    App/Project.h
    App/Project.cpp
    App/AssetRegistry.cpp
    App/AssimpWrapper.h
    App/AssimpWrapper.cpp
    App/Scene.h
    App/Scene.cpp
    App/MouseEventListener.cpp
    App/MouseEventListener.h
    App/TAbstractCtrlBase.h
    App/TransformController.h
    App/TransformController.cpp
    App/ResourceCacheService.h
    App/OrbitalMouseControl.h
    App/OrbitalMouseControl.cpp
)

#==============================================================================
# Modules
#==============================================================================
SET(modules_src
    Modules/IModuleView.h
    Modules/ModuleBase.h
    Modules/ModuleBase.cpp
)

#==============================================================================
# Common
#==============================================================================
SET( common_src
    Common/Environment.h
    Common/AbstractEventHandler.h
    Common/AbstractProcessor.h
    Common/ArgumentParser.h
    Common/AbstractService.h
    Common/ColorRGBA.h
    Common/DateTime.h
    Common/Event.h
    Common/EventBus.h
    Common/EventTriggerer.h
    Common/Frustum.h
    Common/Ids.h
    Common/Logger.h
    Common/Object.h
    Common/StringUtils.h
    Common/TAABB.h
    Common/TFunctor.h
    Common/TResource.h
    Common/TResourceCache.h
    Common/Tokenizer.h
    Common/osre_common.h
    Common/glm_common.h
    Common/BaseMath.h
    Common/TRay.h
    Common/ArgumentParser.cpp
    Common/BaseMath.cpp
    Common/Common.cpp
    Common/DateTime.cpp
    Common/Event.cpp
    Common/EventBus.cpp
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
SET( debugging_src
    Debugging/AssertHandler.h
    Debugging/osre_debugging.h
    Debugging/MeshDiagnostic.h
    Debugging/Debug.cpp
    Debugging/AssertHandler.cpp
    Debugging/MeshDiagnostic.cpp
)

#==============================================================================
# IO
#==============================================================================
SET( io_src
    IO/IOCommon.h
    IO/Directory.h
    IO/File.h
    IO/Stream.h
    IO/AbstractFileSystem.h
    IO/IOService.h
    IO/IOSystemInfo.h
    IO/Uri.h
    IO/Directory.cpp
    IO/File.cpp
    IO/FileStream.cpp
    IO/FileStream.h
    IO/IOService.cpp
    IO/LocaleFileSystem.cpp
    IO/LocaleFileSystem.h
    IO/Stream.cpp
    IO/Uri.cpp
    IO/IOSystemInfo.cpp
)

#==============================================================================
# Platform
#==============================================================================
SET( platform_src
    Platform/Windows/MinWindows.h
    Platform/PlatformCommon.h
    Platform/AbstractOSService.h
    Platform/Threading.h
    Platform/AbstractPlatformEventQueue.h
    Platform/AbstractOGLRenderContext.h
    Platform/AbstractWindow.h
    Platform/AbstractTimer.h
    Platform/AbstractSystemInfo.h
    Platform/SystemInfo.h
    Platform/SystemUtils.h
    Platform/KeyTypes.h
    Platform/PluginType.h
    Platform/PlatformInterface.h
    Platform/PlatformOperations.h
    Platform/AbstractDisplayInterface.h
    Platform/AbstractPlatformEventQueue.cpp
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
SET( profiling_src
    Profiling/ProfilingCommon.h
    Profiling/FPSCounter.h
    Profiling/PerformanceCounterRegistry.h
    Profiling/FPSCounter.cpp
    Profiling/PerformanceCounterRegistry.cpp
)

#==============================================================================
# Properties
#==============================================================================
SET ( properties_src
    Properties/PropertiesCommon.h
    Properties/BasePropertyMap.h
    Properties/AbstractPropertyMap.h
    Properties/Property.h
    Properties/Settings.h
    Properties/BasePropertyMap.cpp
    Properties/Settings.cpp
    Properties/Property.cpp
)

#==============================================================================
# RenderBackend
#==============================================================================
SET( renderbackend_src
    RenderBackend/RenderCommon.h
    RenderBackend/DbgRenderer.h
    RenderBackend/FontService.h
    RenderBackend/Material.h
    RenderBackend/Mesh.h
    RenderBackend/LineBuilder.h
    RenderBackend/MeshProcessor.h
    RenderBackend/MeshBuilder.h
    RenderBackend/MaterialBuilder.h
    RenderBackend/TransformMatrixBlock.h
    RenderBackend/Pipeline.h
    RenderBackend/RenderPass.h
    RenderBackend/RenderBackendService.h
    RenderBackend/RenderStates.h
    RenderBackend/Shader.h
    RenderBackend/DbgRenderer.cpp
    RenderBackend/Material.cpp
    RenderBackend/Mesh.cpp
    RenderBackend/MeshProcessor.cpp
    RenderBackend/MeshBuilder.cpp
    RenderBackend/LineBuilder.cpp
    RenderBackend/MaterialBuilder.cpp
    RenderBackend/RenderBackendService.cpp
    RenderBackend/RenderCommon.cpp
    RenderBackend/Pipeline.cpp
    RenderBackend/RenderPass.cpp
    RenderBackend/TransformMatrixBlock.cpp
    RenderBackend/Shader.cpp
)

SET( renderbackend_mesh_src
    RenderBackend/Mesh/MeshUtilities.h
)
SET( renderbackend_2d_src
    RenderBackend/2D/RenderPass2D.h
    RenderBackend/2D/CanvasRenderer.h
    RenderBackend/2D/CanvasRenderer.cpp
)

SET( renderbackend_vulkanrenderer_src
    RenderBackend/VulkanRenderer/VulkanRenderEventHandler.cpp
    RenderBackend/VulkanRenderer/VulkanRenderEventHandler.h
    RenderBackend/VulkanRenderer/VulkanRenderBackend.cpp
    RenderBackend/VulkanRenderer/VulkanRenderBackend.h
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

set(renderbackend_shader_src
    RenderBackend/Shader/DefaultShader.h
    RenderBackend/Shader/DefaultShader.cpp
)

#==============================================================================
# Scripting with Mono
#==============================================================================
SET( scripting_mono_src
    ${HEADER_PATH}/Scripting/osre_scripting.h
)

#==============================================================================
# Threading
#==============================================================================
SET( threading_src
    Threading/ThreadingCommon.h
    Threading/AbstractTask.h
    Threading/SystemTask.h
    Threading/TaskJob.h
    Threading/TAsyncQueue.h
    Threading/AbstractTask.cpp
    Threading/SystemTask.cpp
)

#==============================================================================
# Defined Source groups
#==============================================================================
SOURCE_GROUP( App                         FILES ${app_src} )
SOURCE_GROUP( Animation                   FILES ${animation_src} )
SOURCE_GROUP( Assets                      FILES ${assets_src} )
SOURCE_GROUP( Common                      FILES ${common_src} )
SOURCE_GROUP( Components                  FILES ${components_src} )
SOURCE_GROUP( Debugging                   FILES ${debugging_src} )
SOURCE_GROUP( Debugging\\Win32            FILES ${debugging_windows_src} )
SOURCE_GROUP( IO                          FILES ${io_src} )
SOURCE_GROUP( Math                        FILES ${math_src} )
SOURCE_GROUP( Memory                      FILES ${memory_src} )
SOURCE_GROUP( Modules                     FILES ${modules_src} )
SOURCE_GROUP( Platform                    FILES ${platform_src} )
SOURCE_GROUP( Platform\\Win32             FILES ${platform_impl_src} )
SOURCE_GROUP( Platform\\sdl2              FILES ${platform_sdl2_src} )
SOURCE_GROUP( Profiling                   FILES ${profiling_src} )
SOURCE_GROUP( Properties                  FILES ${properties_src} )
SOURCE_GROUP( RenderBackend               FILES ${renderbackend_src} )
SOURCE_GROUP( RenderBackend\\2D           FILES ${renderbackend_2d_src})
SOURCE_GROUP( RenderBackend\\Mesh         FILES ${renderbackend_mesh_src} )
SOURCE_GROUP( RenderBackend\\OGLRenderer  FILES ${renderbackend_oglrenderer_src} )
SOURCE_GROUP( RenderBackend\\Vulkan       FILES ${renderbackend_vulkanrenderer_src} )
SOURCE_GROUP( RenderBackend\\Shader       FILES ${renderbackend_shader_src})
SOURCE_GROUP( Resources                   FILES ${resources_src} )
SOURCE_GROUP( Scene                       FILES ${scene_src} )
SOURCE_GROUP( Threading                   FILES ${threading_src} )
SOURCE_GROUP( UI\\Common                  FILES ${ui_common_src} )

SOURCE_GROUP( Documentation                    FILES ${doc_inc} )

add_definitions( -D${USE_PLATFORM} )

#==============================================================================
# Define the Infrastructure library
#==============================================================================
ADD_LIBRARY( osre SHARED
    ../../README.md
    ${common_src}
    ${collision_src}
    ${modules_src}
    ${app_src}
    ${animation_src}
    ${Common_src}
    ${components_src}
    ${debugging_src}
        ${debugging_windows_src}
    ${io_src}
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
        ${renderbackend_2d_src}
        ${renderbackend_mesh_src}
        ${renderbackend_shader_src}
    ${scene_src}
        ${scene_shader_src}
    ${threading_src}
        ${threading_impl_src}
)

target_link_libraries ( osre cppcore zlibstatic GLEW::GLEW ${platform_libs} assimp )
target_compile_definitions(osre PRIVATE
        $<$<CONFIG:Debug>:
        _DEBUG=1 >
)

set_target_properties(  osre PROPERTIES FOLDER Engine )
