INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../3dparty/cppcore/include
    ../3dparty/glew/include
    ../3dparty/assimp/include
    ../3dparty/glm/
    ../3dparty/soil/src
    ../
)

SET ( 00_helloworld_src
    00_HelloWorld/HelloWorld.cpp
	00_HelloWorld/README.md
)

SET ( 01_modelloading_src
    01_ModelLoading/ModelLoading.cpp
    01_ModelLoading/README.md
)

SET ( 02_uidemo_src
    02_UIDemo/UIDemo.cpp
    02_UIDemo/README.md
)

SET ( 03_instancing_src
	03_Instancing/Instancing.cpp
	03_Instancing/README.md
)

ADD_EXECUTABLE( HelloWorld
    ${00_helloworld_src}
)

ADD_EXECUTABLE( ModelLoading
    ${01_modelloading_src}
)

ADD_EXECUTABLE( UIDemo
    ${02_uidemo_src}
)

ADD_EXECUTABLE( Instancing
    ${03_instancing_src}
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Debug
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Release
)

target_link_libraries ( HelloWorld   osre )
target_link_libraries ( ModelLoading osre ) 
target_link_libraries ( UIDemo       osre )
target_link_libraries ( Instancing   osre )

set_target_properties(  HelloWorld   PROPERTIES FOLDER Samples )
set_target_properties(  ModelLoading PROPERTIES FOLDER Samples )
set_target_properties(  UIDemo       PROPERTIES FOLDER Samples )
set_target_properties(  UIDemo       PROPERTIES Instancing Samples )
