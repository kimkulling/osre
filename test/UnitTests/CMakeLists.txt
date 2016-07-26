SET ( GTEST_PATH ../../3dparty/gmock-1.7.0/gtest/ )

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    find_package(Threads)
endif()

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../../3dparty/cppcore/include
    ../../3dparty/glew/include
    ../../3dparty/glm/
    ../../3dparty/soil/src
    ${GTEST_PATH}
    ${GTEST_PATH}/include
    .././
)

SET ( unittest_common_src
    src/Common/ArgumentParserTest.cpp
	src/Common/AbstractServiceTest.cpp
	src/Common/ObjectTest.cpp
    src/Common/EventTest.cpp
    src/Common/IdsTest.cpp
)

SET( unittest_platform_src
	src/Platform/AbstractDynamicLoaderTest.cpp
)

SET ( unittest_rb_src
    src/RenderBackend/RenderCommonTest.cpp
)

SET ( unittest_scene_src
    src/Scene/ComponentTest.cpp
	src/Scene/GeometryBuilderTest.cpp
    src/Scene/NodeTest.cpp
)

SET ( gtest_src
    ${GTEST_PATH}/src/gtest-death-test.cc
    ${GTEST_PATH}/src/gtest-filepath.cc
    ${GTEST_PATH}/src/gtest-internal-inl.h
    ${GTEST_PATH}/src/gtest-port.cc
    ${GTEST_PATH}/src/gtest-printers.cc
    ${GTEST_PATH}/src/gtest-test-part.cc
    ${GTEST_PATH}/src/gtest-typed-test.cc
    ${GTEST_PATH}/src/gtest.cc
    ${GTEST_PATH}/src/gtest_main.cc
)

SOURCE_GROUP( src\\Common         FILES ${unittest_common_src} )
SOURCE_GROUP( src\\Platform       FILES ${unittest_platform_src})
SOURCE_GROUP( src\\RenderBackend  FILES ${unittest_rb_src} )
SOURCE_GROUP( src\\Scene          FILES ${unittest_scene_src} )
SOURCE_GROUP( src\\GTest          FILES ${gtest_src} )

ADD_EXECUTABLE( osre_unittest
    ${unittest_common_src}
	${unittest_platform_src}    
	${unittest_rb_src}
    ${unittest_scene_src}
    ${gtest_src}
)

link_directories( 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Debug
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Release
)

IF( WIN32 )
    SET( platform_libs )
ELSE( WIN32 )
    SET( platform_libs pthread )
ENDIF( WIN32 )

target_link_libraries ( osre_unittest osre ${platform_libs} ) 
set_target_properties(  osre_unittest PROPERTIES FOLDER Tests )
