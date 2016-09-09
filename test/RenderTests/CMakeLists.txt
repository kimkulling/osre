INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../../3dparty/cppcore/include
    ../../3dparty/glew/include
    ../../3dparty/glm/
    ../../3dparty/soil/src
    .././
)

SET ( rendertest_src
    src/AbstractTestFixure.h
    src/AbstractRenderTest.cpp
    src/AbstractRenderTest.h
    src/RenderTestSuite.cpp
    src/PrimTypeRenderTest.cpp
    src/BaseTriangleRenderTest.cpp
    src/BaseTextureRenderTest.cpp
    src/DbgFontRenderTest.cpp
    src/GeoInstanceRenderTest.cpp
	src/RenderBufferAccessTest.cpp
    src/RenderTestSuite.h
    src/RenderTestUtils.h
    src/main.cpp
)

SOURCE_GROUP( src          FILES ${rendertest_src} )

ADD_EXECUTABLE( osre_rendertest
    ${rendertest_src}
)

link_directories( 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Debug
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Release
)

target_link_libraries ( osre_rendertest osre ) 
set_target_properties(  osre_rendertest PROPERTIES FOLDER Tests )
