find_package(Python3 COMPONENTS Interpreter Development)

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui
)

INCLUDE_DIRECTORIES(
    ./
    ./src
    ${Python3_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}
)

link_directories(
    ${Python3_LIBRARIES}
)

set(platform_backend 
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/backends/imgui_impl_sdl2.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/backends/imgui_impl_sdl2.h
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/backends/imgui_impl_opengl3.h
)

set(imgui_src
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imconfig.h
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui.h
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui_demo.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui_draw.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui_internal.h
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui_tables.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imgui_widgets.cpp
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imstb_rectpack.h
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imstb_textedit.h
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui/imstb_truetype.h
    ${platform_backend}
)

SET( inspector_module_src 
    src/Modules/InspectorModule/InspectorModule.cpp
    src/Modules/InspectorModule/InspectorModule.h
)

SET( log_module_src
    src/Modules/LogModule/LogModule.h
    src/Modules/LogModule/LogModule.cpp
)

SET( editor_module_src
    src/Modules/EditorModule/EditorModule.h
    src/Modules/EditorModule/EditorModule.cpp
)

SET( modules_src 
    src/Modules/ModuleRegistry.h
    src/Modules/ModuleRegistry.cpp
)

SET( scripting_src 
    src/Scripting/PythonInterface.cpp
    src/Scripting/PythonInterface.h
)

set(renderview_src
    src/RenderView/MainRenderView.cpp
    src/RenderView/MainRenderView.h    
)

set(actions_src
    src/Actions/ActionBase.h
    src/Actions/ImportAction.cpp
    src/Actions/ImportAction.h
)

set( gui_src
    src/Gui/UIElementsWin32.h
    src/Gui/UIElementsWin32.cpp
    src/Gui/UIElements.cpp
    src/Gui/UIElements.h
)

set(main_src
    src/OsreEdApp.cpp
    src/OsreEdApp.h
    src/SceneData.h
    src/ProgressReporter.cpp
    src/ProgressReporter.h
    src/main.cpp

)
SOURCE_GROUP(contrib\\Imgui           FILES ${imgui_src})
SOURCE_GROUP(RenderView               FILES ${renderview_src})
SOURCE_GROUP(Actions                  FILES ${actions_src})
source_group(Gui                      FILES ${gui_src})
SOURCE_GROUP(Modules\\InspectorModule FILES ${inspector_module_src} )
SOURCE_GROUP(Modules\\LogModule       FILES ${log_module_src} )
SOURCE_GROUP(Modules\\EditorModule    FILES ${editor_module_src} )
SOURCE_GROUP(Modules\\                FILES ${modules_src} )
SOURCE_GROUP(Scripting\\              FILES ${scripting_src} )

SOURCE_GROUP(Main\\                   FILES ${main_src})

ADD_EXECUTABLE( osre_ed_imgui
    ${imgui_src}    
    ${actions_src}
    ${gui_src}
    ${inspector_module_src}
    ${log_module_src}
    ${editor_module_src}
    ${modules_src}
    ${scripting_src}
    ${renderview_src}
    ${main_src}
)

IF(WIN32)
  target_link_libraries( osre_ed_imgui osre ${Python3_LIBRARIES} Ws2_32 SDL2::SDL2 )
ELSE()
  target_link_libraries( osre_ed_imgui osre ${Python3_LIBRARIES} SDL2::SDL2 )
ENDIF(WIN32)
