SET( LIBRARY_VERSION "0.1.0" )

SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/osre/ )

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${HEADER_PATH}/include
    ../3dparty/cppcore/include
    ../3dparty/glew/include
)

IF( WIN32 )
    SET( platform_libs comctl32.lib Winmm.lib opengl32.lib glu32.lib SDL2 )
ELSE( WIN32 )
    SET( platform_libs SDL2 )
ENDIF( WIN32 )

#==============================================================================
# Common
#==============================================================================
SET( common_inc
    ${HEADER_PATH}/Common/AbstractCodec.h
    ${HEADER_PATH}/Common/AbstractEventHandler.h
    ${HEADER_PATH}/Common/ArgumentParser.h
    ${HEADER_PATH}/Common/AbstractService.h
    ${HEADER_PATH}/Common/CodecRegistry.h
    ${HEADER_PATH}/Common/ColorRGBA.h
    ${HEADER_PATH}/Common/DateTime.h
    ${HEADER_PATH}/Common/Event.h
    ${HEADER_PATH}/Common/EventTriggerer.h
    ${HEADER_PATH}/Common/Logger.h
    ${HEADER_PATH}/Common/Object.h
    ${HEADER_PATH}/Common/StringUtils.h
    ${HEADER_PATH}/Common/TFunctor.h
    ${HEADER_PATH}/Common/TObjPtr.h
    ${HEADER_PATH}/Common/Tokenizer.h
    ${HEADER_PATH}/Common/Types.h
)
    
SET( common_src
    Common/AbstractCodec.cpp
    Common/ArgumentParser.cpp
    Common/CodecRegistry.cpp
    Common/DateTime.cpp
    Common/EventTriggerer.cpp
    Common/Logger.cpp
    Common/Object.cpp
    Common/Tokenizer.cpp
)

#==============================================================================
# Platform
#==============================================================================
SET( platform_inc 
    ${HEADER_PATH}/Platform/AbstractThread.h
    ${HEADER_PATH}/Platform/AbstractThreadEvent.h
    ${HEADER_PATH}/Platform/AbstractThreadFactory.h
    ${HEADER_PATH}/Platform/AbstractCriticalSection.h
    ${HEADER_PATH}/Platform/AtomicInt.h
    ${HEADER_PATH}/Platform/AbstractPlatformEventHandler.h
    ${HEADER_PATH}/Platform/AbstractRenderContext.h
    ${HEADER_PATH}/Platform/AbstractSurface.h
    ${HEADER_PATH}/Platform/AbstractTimer.h
    ${HEADER_PATH}/Platform/KeyTypes.h
    ${HEADER_PATH}/Platform/PluginType.h
    ${HEADER_PATH}/Platform/PlatformInterface.h
)

SET( platform_src
    Platform/AtomicInt.cpp
    Platform/AbstractSurface.cpp
    Platform/PlatformInterface.cpp
    Platform/PlatformPluginFactory.cpp
    Platform/PlatformPluginFactory.h
)

SET( platform_win32_src 
    Platform/win32/Win32Atomic.h
    Platform/win32/Win32Surface.cpp
    Platform/win32/Win32Surface.h
    Platform/win32/Win32Eventhandler.cpp
    Platform/win32/Win32Eventhandler.h
    Platform/win32/Win32RenderContext.cpp
    Platform/win32/Win32RenderContext.h
    Platform/win32/Win32Timer.cpp
    Platform/win32/Win32Timer.h
    Platform/win32/Win32CriticalSection.h
    Platform/win32/Win32Thread.cpp
    Platform/win32/Win32Thread.h
    Platform/win32/Win32ThreadEvent.cpp
    Platform/win32/Win32ThreadEvent.h
    Platform/win32/Win32ThreadFactory.cpp
    Platform/win32/Win32ThreadFactory.h
    Platform/win32/Win32Timer.cpp
    Platform/win32/Win32Timer.h
)

SET( platform_sdl2_src
    Platform/sdl2/SDL2Atomic.h
    Platform/sdl2/SDL2CriticalSection.h
    Platform/sdl2/SDL2EventHandler.cpp
    Platform/sdl2/SDL2EventHandler.h
    Platform/sdl2/SDL2Initializer.cpp
    Platform/sdl2/SDL2Initializer.h
    Platform/sdl2/SDL2Surface.cpp
    Platform/sdl2/SDL2Surface.h
    Platform/sdl2/SDL2Timer.cpp
    Platform/sdl2/SDL2Timer.h
    Platform/sdl2/SDL2RenderContext.cpp
    Platform/sdl2/SDL2RenderContext.h
    Platform/sdl2/SDL2Thread.cpp
    Platform/sdl2/SDL2Thread.h
    Platform/sdl2/SDL2ThreadEvent.cpp
    Platform/sdl2/SDL2ThreadEvent.h
    Platform/sdl2/SDL2ThreadFactory.cpp
    Platform/sdl2/SDL2ThreadFactory.h
)

IF( WIN32 )
    SET( platform_impl_src ${platform_win32_src} )
ENDIF( WIN32 )

#==============================================================================
# Properties
#==============================================================================
SET ( properties_inc
    ${HEADER_PATH}/Properties/BasePropertyMap.h
    ${HEADER_PATH}/Properties/IPropertyMap.h
    ${HEADER_PATH}/Properties/Property.h
    ${HEADER_PATH}/Properties/ConfigurationMap.h
)
SET ( properties_src
    Properties/BasePropertyMap.cpp
    Properties/ConfigurationMap.cpp
    Properties/Property.cpp
)

#==============================================================================
# Threading
#==============================================================================
SET( threading_src
    Threading/AbstractTask.cpp
    Threading/AsyncTask.cpp
    Threading/IThreadFactory.cpp
    Threading/SystemTask.cpp
    Threading/TaskGroup.cpp
    Threading/TaskScheduler.cpp
    Threading/ThreadPool.cpp
    Threading/WorkerThread.cpp
    Threading/WorkerThread.h
)

SET( threading_inc
    ${HEADER_PATH}/Threading/AbstractTask.h
    ${HEADER_PATH}/Threading/AsyncTask.h
    ${HEADER_PATH}/Threading/TaskScheduler.h
    ${HEADER_PATH}/Threading/SystemTask.h
    ${HEADER_PATH}/Threading/TaskJob.h
    ${HEADER_PATH}/Threading/TaskGroup.h
    ${HEADER_PATH}/Threading/ThreadPool.h
    ${HEADER_PATH}/Threading/TAsyncQueue.h
)

#==============================================================================
# Defined Source groups
#==============================================================================
SOURCE_GROUP( Common              FILES ${common_src} )
SOURCE_GROUP( Components          FILES ${components_src} )
SOURCE_GROUP( Debugging           FILES ${debugging_src} )
SOURCE_GROUP( Debugging\\Win32    FILES ${debugging_windows_src} )
SOURCE_GROUP( System              FILES ${system_src} )
SOURCE_GROUP( Utils               FILES ${utils_src} )
SOURCE_GROUP( IO                  FILES ${io_src} )
SOURCE_GROUP( Math                FILES ${math_src} )
SOURCE_GROUP( Memory              FILES ${memory_src} )
SOURCE_GROUP( Platform            FILES ${platform_src} )
SOURCE_GROUP( Platform\\Win32     FILES ${platform_impl_src} )
SOURCE_GROUP( Platform\\sdl2      FILES ${platform_sdl2_src} )
SOURCE_GROUP( Properties          FILES ${properties_src} )
SOURCE_GROUP( Resources           FILES ${resources_src} )
SOURCE_GROUP( Scripting           FILES ${scripting_src} )
SOURCE_GROUP( Threading           FILES ${threading_src} )
SOURCE_GROUP( Include\\Common     FILES ${common_inc} )
SOURCE_GROUP( Include\\Components FILES ${components_inc} )
SOURCE_GROUP( Include\\Debugging  FILES ${debugging_inc} )
SOURCE_GROUP( Include\\System     FILES ${system_inc} )
SOURCE_GROUP( Include\\Utils      FILES ${utils_inc} )
SOURCE_GROUP( Include\\IO         FILES ${io_inc} )
SOURCE_GROUP( Include\\Math       FILES ${math_inc} )
SOURCE_GROUP( Include\\Memory     FILES ${memory_inc} )
SOURCE_GROUP( Include\\Properties FILES ${properties_inc} )
SOURCE_GROUP( Include\\Platform   FILES ${platform_inc} )
SOURCE_GROUP( Include\\Resources  FILES ${resources_inc} )
SOURCE_GROUP( Include\\Scripting  FILES ${scripting_inc} )
SOURCE_GROUP( Include\\Thread     FILES ${threading_inc} )

#==============================================================================
# Define the Infrastructure library
#==============================================================================
ADD_LIBRARY( osre SHARED
    ${common_inc}
    ${common_src}
    ${Common_inc}
    ${components_inc}
    ${debugging_inc}
    ${system_inc}
    ${io_inc}
    ${math_inc}
    ${memory_inc}
    ${utils_inc}
    ${properties_inc}
    ${platform_inc}
    ${resources_inc}
    ${scripting_inc}
    ${threading_inc}
    ${math_inc}

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
    ${platform_src}
        ${platform_impl_src}
        ${platform_sdl2_src}
    ${utils_src}
    ${resources_src}
    ${scripting_src}
    ${threading_src}
        ${threading_impl_src}
)

target_link_libraries ( osre cppCommon zlib Unzip Lua glew ${platform_libs} )
set_target_properties(  osre PROPERTIES FOLDER Engine )

#==============================================================================
# Define the install targets.
#==============================================================================
INSTALL( TARGETS osre
    DESTINATION ${LIB_INSTALL_DIR}
    COMPONENT lib_ce2_infrastructure${ZFXCE2_VERSION_MAJOR}
)
#INSTALL( FILES ${PUBLIC_HEADERS}
#    DESTINATION ${INCLUDE_INSTALL_DIR}/zfxce2/Infrastructure
    #COMPONENT zfxce2-dev
#)
