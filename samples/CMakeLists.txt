INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ./
    ../
)

SET (00_helloworld_src
    00_HelloWorld/HelloWorld.cpp
    00_HelloWorld/README.md
)

SET (01_modelloading_src
    01_ModelLoading/ModelLoading.cpp
    01_ModelLoading/README.md
)

SET (02_demo2d_src
    02_Demo2D/Demo2D.cpp
    02_Demo2D/README.md
)

SET (03_instancing_src
    03_Instancing/Instancing.cpp
    03_Instancing/README.md
)

SET (04_terrain_src
    04_terrain/TerrainRendering.cpp
    04_terrain/README.md
)

SET (05_animation_src
    05_Animation/Animation.cpp
    05_Animation/README.md
)

SET (06_widgets_src
    06_Widgets/Widgets.cpp
    06_Widgets/README.md
)

ADD_EXECUTABLE(helloworld
    ${00_helloworld_src}
)

ADD_EXECUTABLE(modelloading
    ${01_modelloading_src}
)

ADD_EXECUTABLE(demo2d
    ${02_demo2d_src}
)

ADD_EXECUTABLE(instancing
    ${03_instancing_src}
)

ADD_EXECUTABLE(terrain
    ${04_terrain_src}
)

ADD_EXECUTABLE(animation
    ${05_animation_src}
)

ADD_EXECUTABLE(widgets
    ${06_widgets_src}
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Debug
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Release
)

target_link_libraries(helloworld   osre)
target_link_libraries(modelloading osre)
target_link_libraries(demo2d       osre)
target_link_libraries(instancing   osre)
target_link_libraries(terrain      osre)
target_link_libraries(animation    osre)
target_link_libraries(widgets      osre)

set_target_properties(helloworld   PROPERTIES FOLDER Samples)
set_target_properties(modelloading PROPERTIES FOLDER Samples)
set_target_properties(demo2d       PROPERTIES FOLDER Samples)
set_target_properties(instancing   PROPERTIES FOLDER Samples)
set_target_properties(terrain      PROPERTIES FOLDER Samples)
set_target_properties(widgets      PROPERTIES FOLDER Samples)
set_target_properties(animation    PROPERTIES FOLDER Samples)

