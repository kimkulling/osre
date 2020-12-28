message(STATUS "The CMake version used is ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.5...${CMAKE_VERSION})

PROJECT( osre )
set ( OSRE_VERSION_MAJOR 0 )
set ( OSRE_VERSION_MINOR 1 )
set ( OSRE_VERSION_PATCH 0 )
set ( OSRE_VERSION ${OSRE_VERSION_MAJOR}.${OSRE_VERSION_MINOR}.${OSRE_VERSION_PATCH} )
set ( PROJECT_VERSION "${OSRE_VERSION}" )
set_property( GLOBAL PROPERTY CXX_STANDARD 11 )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/3dparty/assimp/cmake-modules/")
message(${CMAKE_MODULE_PATH})

OPTION( OSRE_BUILD_SAMPLES
  "Build the samples of OSRE."
  ON
)

OPTION( OSRE_BUILD_TESTS
  "Build the test suite of OSRE."
  ON
)

OPTION( OSRE_BUILD_DOC
  "Build the doxygen-based documentationof OSRE."
  OFF
)

OPTION( OSRE_BUILD_CSHARP_EDITOR
  "Build the c# editor of OSRE."
  OFF
)

# Cache these to allow the user to override them manually.
set( LIB_INSTALL_DIR "lib" CACHE PATH
    "Path the built library files are installed to." )
set( INCLUDE_INSTALL_DIR "osre" CACHE PATH
    "Path the header files are installed to." )
set( BIN_INSTALL_DIR "bin" CACHE PATH
    "Path the tool executables are installed to." )

INCLUDE_DIRECTORIES( BEFORE
    include/
    /usr/include/SDL2/
    /usr/include/
    3dparty/cppcore/include
    3dparty/glew/include
    3dparty/assimp/include
    3dparty/glm/
    3dparty/soil/src
    ${CMAKE_HOME_DIRECTORY}/3dparty/glew/include
    ${CMAKE_HOME_DIRECTORY}/3dparty/cppcore/include
    ${CMAKE_HOME_DIRECTORY}/3dparty/cppcore/contrib/gtest-1.7.0/include
    ${CMAKE_HOME_DIRECTORY}/3dparty/cppcore/contrib/gtest-1.7.0/
    ${CMAKE_HOME_DIRECTORY}/3dparty/assimp/contrib/irrXML
    ${CMAKE_HOME_DIRECTORY}/3dparty/zlib
    ${CMAKE_HOME_DIRECTORY}/3dparty/jsoncpp-1.8.4/include
	${CMAKE_HOME_DIRECTORY}/3dparty/lua-5.3.5/src
    "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    ${CMAKE_HOME_DIRECTORY}/3dparty/soil/src
    ${CMAKE_HOME_DIRECTORY}/3dparty/sdl2/include
	${CMAKE_HOME_DIRECTORY}/src/
)

if( "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)" OR "${CMAKE_GENERATOR}" MATCHES "16 2019")
	link_directories(
        ${CMAKE_HOME_DIRECTORY}/3dparty/sdl2/lib/x64
		${CMAKE_HOME_DIRECTORY}/lib
		${CMAKE_HOME_DIRECTORY}/bin
	)
else()
	link_directories(
        ${CMAKE_HOME_DIRECTORY}/3dparty/sdl2/lib/x86
		${CMAKE_HOME_DIRECTORY}/lib
		${CMAKE_HOME_DIRECTORY}/bin
	)
endif()

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

IF ((CMAKE_C_COMPILER_ID MATCHES "GNU") AND NOT CMAKE_COMPILER_IS_MINGW)
  IF(NOT ASSIMP_HUNTER_ENABLED)
    SET(CMAKE_CXX_STANDARD 11)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
  ENDIF()
  # hide all not-exported symbols
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall ${CMAKE_CXX_FLAGS}")
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
  SET(LIBSTDC++_LIBRARIES -lstdc++)
ELSEIF(MSVC)
    FIND_PACKAGE( DirectX )
    add_definitions( -DWIN32_LEAN_AND_MEAN )
ELSEIF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  IF(NOT ASSIMP_HUNTER_ENABLED)
    SET(CMAKE_CXX_STANDARD 11)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
  ENDIF()
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long ${CMAKE_CXX_FLAGS}" )
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
ELSEIF( CMAKE_COMPILER_IS_MINGW )
  IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
    message(FATAL_ERROR "MinGW is too old to be supported. Please update MinGW and try again.")
  ELSEIF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.3)
    message(WARNING "MinGW is old, if you experience errors, update MinGW.")
  ENDIF()
  IF(NOT ASSIMP_HUNTER_ENABLED)
    SET(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
  ENDIF()
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long -Wa,-mbig-obj -O3 ${CMAKE_CXX_FLAGS}")
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing ${CMAKE_C_FLAGS}")
  ADD_DEFINITIONS( -U__STRICT_ANSI__ )
ENDIF()

add_definitions( -DGLM_ENABLE_EXPERIMENTAL )

# Include all sub directories of the engine code component
ADD_SUBDIRECTORY( src/Engine )
IF(WIN32)
    ADD_SUBDIRECTORY( src/Editor_cpp )
    set_target_properties(  osre_ed       PROPERTIES FOLDER Editor )
ENDIF()
IF ( OSRE_BUILD_TESTS )
    ADD_SUBDIRECTORY( test/RenderTests )
    ADD_SUBDIRECTORY( test/UnitTests )
ENDIF(OSRE_BUILD_TESTS)

IF ( OSRE_BUILD_SAMPLES )
    ADD_SUBDIRECTORY( samples )
ENDIF(OSRE_BUILD_SAMPLES)

ADD_SUBDIRECTORY( 3dparty/glew )
ADD_SUBDIRECTORY( 3dparty/cppcore )
ADD_SUBDIRECTORY( 3dparty/zlib )
ADD_SUBDIRECTORY( 3dparty/unzip )
ADD_SUBDIRECTORY( 3dparty/soil )
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "no assimp tools")
set( ASSIMP_BUILD_ZLIB On CACHE BOOL "no assimp tools" )
ADD_SUBDIRECTORY( 3dparty/assimp )
ADD_SUBDIRECTORY( 3dparty/jsoncpp-1.8.4 )

# Include the doc component
IF( OSRE_BUILD_DOC )
  find_package(Doxygen)
  IF ( DOXYGEN_FOUND )
      CONFIGURE_FILE( doc/Doxygen/osre_doxygen.in doc/Doxygen/osre_doxyfile @ONLY )
      ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} doc/Doxygen/osre_doxyfile
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating OSRE-API documentation with Doxygen" VERBATIM
      )
  else (DOXYGEN_FOUND)
      message("Doxygen need to be installed to generate the doxygen documentation")
  ENDIF ( DOXYGEN_FOUND )
ENDIF( OSRE_BUILD_DOC )

IF ( OSRE_BUILD_DOC )  
  set_target_properties(  doc                PROPERTIES FOLDER Documentation )
ENDIF( OSRE_BUILD_DOC )
set_target_properties(  cppcore            PROPERTIES FOLDER ThirdParty )
set_target_properties(  assimp             PROPERTIES FOLDER ThirdParty )
