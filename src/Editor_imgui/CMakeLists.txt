find_package(Python3 COMPONENTS Interpreter Development)

INCLUDE_DIRECTORIES( BEFORE
    ${CMAKE_HOME_DIRECTORY}/contrib/imgui
)

INCLUDE_DIRECTORIES(
    ./
    ./src
    ${Python3_INCLUDE_DIRS}
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

set(main_src
    ${imgui_src}
    src/main.cpp

)
SOURCE_GROUP(contrib\\Imgui           FILES ${imgui_src})
SOURCE_GROUP(Main\\                   FILES ${main_src})

ADD_EXECUTABLE( osre_ed_imgui
    ${imgui_src}    
    ${main_src}
)

message(${Python3_LIBRARIES})
target_link_libraries ( osre_ed_imgui osre ${Python3_LIBRARIES} sdl2-config SDL2main )
