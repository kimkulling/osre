INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../../3dparty/cppcore/include
    ../../3dparty/glew/include
    ../../3dparty/glm/
    ../../3dparty/soil/src
    .././
)

SET ( unittest_common_src
    src/Common/ObjectTest.cpp
)

SET ( GTEST_PATH ../../3dparty/gmock-1.7.0/gtest/ )

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

SOURCE_GROUP( src\\Common          FILES ${unittest_common_src} )
SOURCE_GROUP( src\\GTest          FILES ${gtest_src} )

ADD_EXECUTABLE( unittest
    ${unittest_common_src}
    ${gtest_src}
)

link_directories( 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Debug
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ThirdParty/glew/Release
)

target_link_libraries ( unittest osre ) 
set_target_properties(  unittest PROPERTIES FOLDER Tests )
