SET ( GMOCK_PATH ../../3dparty/gmock-1.7.0/ )
SET ( GTEST_PATH ${GMOCK_PATH}gtest/ )

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
endif()

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    find_package(Threads)
endif()

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../../3dparty/cppcore/include
    ../../3dparty/assimp/include
    ../../3dparty/glew/include
    ../../3dparty/glm/
    ../../3dparty/soil/src
    ${GMOCK_PATH}
	${GTEST_PATH}
    ${GTEST_PATH}/include
    .././
	src
)

SET ( unittest_assets_src
	src/Assets/ModelTest.cpp
	src/Assets/AssetRegistryTest.cpp
	src/Assets/AssetDataTest.cpp
    src/Assets/AssetWrapperTest.cpp
)

SET ( unittest_app_src
    src/App/TAbstractCtrlBaseTest.cpp
)

SET ( unittest_common_src
	src/Common/AbstractProcessTest.cpp
    src/Common/ArgumentParserTest.cpp
	src/Common/AbstractServiceTest.cpp
	src/Common/CommonTest.cpp
	src/Common/ObjectTest.cpp
    src/Common/EventTest.cpp
    src/Common/IdsTest.cpp
)

SET ( unittest_collision_src
    src/Collision/TAABBTest.cpp
    src/Collision/TRayTest.cpp
)

SET ( unittest_debugging_src
    src/Debugging/osre_debugging_test.cpp
)

SET( unittest_io_src 
    src/IO/UriTest.cpp
)

SET( unittest_math_src
    src/Math/BaseMathTest.cpp
)

SET( unittest_platform_src
	src/Platform/AbstractDynamicLoaderTest.cpp
    src/Platform/AbstractThreadTest.cpp
)

SET ( unittest_rb_src
	src/RenderBackend/CullStateTest.cpp
    src/RenderBackend/BlendStateTeste.cpp
    #src/RenderBackend/PolygonStateTest.cpp
	src/RenderBackend/RenderCommonTest.cpp
	src/RenderBackend/PipelineTest.cpp
)

SET( unittest_rb_oglrenderer_src 
	src/RenderBackend/OGLRenderer/GLEnumTest.cpp
)

SET( unittest_ui_src
    src/UI/WidgetTest.cpp
    src/UI/ButtonBaseTest.cpp
    src/UI/StyleProviderTest.cpp
)

SET ( unittest_profiling_src
	src/Profiling/PerformanceCountersTest.cpp
)

SET ( unittest_scene_src
    src/Scene/ComponentTest.cpp
	src/Scene/DbgRendererTest.cpp
	src/Scene/GeometryBuilderTest.cpp
    src/Scene/NodeTest.cpp
    src/Scene/WorldTest.cpp
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

SOURCE_GROUP( src\\App                        FILES ${unittest_app_src} )
SOURCE_GROUP( src\\Common                     FILES ${unittest_common_src} )
SOURCE_GROUP( src\\Collision                  FILES ${unittest_collision_src})
SOURCE_GROUP( src\\Debugging                  FILES ${unittest_debugging_src})
SOURCE_GROUP( src\\Assets                     FILES ${unittest_assets_src} )
SOURCE_GROUP( src\\IO                         FILES ${unittest_io_src} )
SOURCE_GROUP( src\\Math                       FILES ${unittest_math_src} )
SOURCE_GROUP( src\\Platform                   FILES ${unittest_platform_src})
SOURCE_GROUP( src\\Profiling                  FILES ${unittest_profiling_src})
SOURCE_GROUP( src\\RenderBackend              FILES ${unittest_rb_src} )
SOURCE_GROUP( src\\RenderBackend\\OGLRenderer FILES ${unittest_rb_oglrenderer_src} )
SOURCE_GROUP( src\\UI                         FILES ${unittest_ui_src} )
SOURCE_GROUP( src\\Scene                      FILES ${unittest_scene_src} )
SOURCE_GROUP( src\\GTest                      FILES ${gtest_src} )

ADD_EXECUTABLE( osre_unittest
    src/osre_testcommon.h
    ${unittest_app_src}
	${unittest_assets_src}    
    ${unittest_common_src}
    ${unittest_collision_src}
    ${unittest_debugging_src}
	${unittest_io_src}
    ${unittest_math_src}
	${unittest_platform_src}    
	${unittest_profiling_src}
	${unittest_rb_src}
	${unittest_rb_oglrenderer_src}
    ${unittest_ui_src}
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
