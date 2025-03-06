message(STATUS "The CMake version used is ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.5...${CMAKE_VERSION})

PROJECT( osre )
SET( OSRE_VERSION_MAJOR 0 )
SET( OSRE_VERSION_MINOR 1 )
SET( OSRE_VERSION_PATCH 0 )
SET( OSRE_VERSION ${OSRE_VERSION_MAJOR}.${OSRE_VERSION_MINOR}.${OSRE_VERSION_PATCH} )
SET( PROJECT_VERSION "${OSRE_VERSION}" )
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/contrib/assimp/cmake-modules/")
 
OPTION( OSRE_BUILD_PLAYER "Build the plyer of OSRE." ON)
OPTION( OSRE_BUILD_SAMPLES "Build the samples of OSRE." ON)
OPTION( OSRE_BUILD_TESTS "Build the test suite for OSRE." ON)
OPTION( OSRE_BUILD_DOC "Build the doxygen-based documentation for OSRE." OFF)
OPTION( OSRE_BUILD_ED "Build the OSRE Ed." ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(glm         REQUIRED)
find_package(GLEW        REQUIRED)

# Cache these to allow the user to override them manually.
SET( LIB_INSTALL_DIR "lib"      CACHE PATH "Path the built library files are installed to." )
SET( INCLUDE_INSTALL_DIR "osre" CACHE PATH "Path the header files are installed to." )
SET( BIN_INSTALL_DIR "bin"      CACHE PATH "Path the tool executables are installed to." )

# Set the default build output folders
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

INCLUDE_DIRECTORIES( BEFORE
    include/
    /usr/include/
    contrib/assimp/include
    contrib/stb
    contrib/glew/include
    contrib/cppcore/include
    contrib/assimp/contrib/zlib
    contrib/cppcore/contrib/gtest-1.7.0/include
    contrib/cppcore/contrib/gtest-1.7.0/
    ${SDL2_INCLUDE_DIRS}
    "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    src/Engine
    ${CMAKE_CURRENT_BINARY_DIR}/
)

link_directories(
    ${CMAKE_HOME_DIRECTORY}/lib
    ${CMAKE_HOME_DIRECTORY}/bin
    ${SDL2_LIBRARIES}
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

IF ((CMAKE_C_COMPILER_ID MATCHES "GNU") AND NOT CMAKE_COMPILER_IS_MINGW)
  IF(NOT ASSIMP_HUNTER_ENABLED)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
  ENDIF()
  # hide all not-exported symbols
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -fPIC ${CMAKE_CXX_FLAGS}")
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing -fPIC ${CMAKE_C_FLAGS}")
  SET(LIBSTDC++_LIBRARIES -lstdc++)
ELSEIF(MSVC)
  FIND_PACKAGE( DirectX )
  ADD_DEFINITIONS( -DWIN32_LEAN_AND_MEAN )
  ADD_COMPILE_OPTIONS(/MP /bigobj /W4)
ELSEIF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
  IF(NOT ASSIMP_HUNTER_ENABLED)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
  ENDIF()
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long -fPIC ${CMAKE_CXX_FLAGS}" )
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing  -fPIC ${CMAKE_C_FLAGS}")
ELSEIF( CMAKE_COMPILER_IS_MINGW )
  IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
    MESSAGE(FATAL_ERROR "MinGW is too old to be supported. Please update MinGW and try again.")
  ELSEIF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.3)
    MESSAGE(WARNING "MinGW is old, if you experience errors, update MinGW.")
  ENDIF()
  IF(NOT ASSIMP_HUNTER_ENABLED)
    SET(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
  ENDIF()
  SET(CMAKE_CXX_FLAGS "-fvisibility=hidden -fno-strict-aliasing -Wall -Wno-long-long -Wa,-mbig-obj -O3 ${CMAKE_CXX_FLAGS} -fPIC")
  SET(CMAKE_C_FLAGS "-fno-strict-aliasing  -fPIC ${CMAKE_C_FLAGS}")
  ADD_DEFINITIONS( -U__STRICT_ANSI__ )
ENDIF()

# Include all sub-directories of the engine code component
ADD_SUBDIRECTORY( src/Engine )
if (OSRE_BUILD_PLAYER)
    ADD_SUBDIRECTORY( src/Player )
endif()

if (OSRE_BUILD_ED)
    ADD_SUBDIRECTORY( src/Editor)
    set_target_properties(osre_ed PROPERTIES FOLDER Editor)
endif()

if (OSRE_BUILD_TESTS)
    ADD_SUBDIRECTORY( test/RenderTests )
    ADD_SUBDIRECTORY( test/UnitTests )
endif(OSRE_BUILD_TESTS)

IF (OSRE_BUILD_SAMPLES)
    ADD_SUBDIRECTORY( samples )
ENDIF(OSRE_BUILD_SAMPLES)

option(cppcore_BUILD_UNITTESTS OFF)
ADD_SUBDIRECTORY( contrib/cppcore )

# Config for assimp
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "no assimp tools")
SET(ASSIMP_BUILD_ZLIB ON CACHE BOOL "no assimp tools" )
SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL "no assimp tools" )
ADD_SUBDIRECTORY( contrib/assimp )

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
      MESSAGE("Doxygen needs to be installed to generate the doxygen documentation")
  ENDIF ( DOXYGEN_FOUND )
ENDIF( OSRE_BUILD_DOC )

if(OSRE_BUILD_DOC)  
  set_target_properties(doc PROPERTIES FOLDER Documentation )
endif(OSRE_BUILD_DOC)
set_target_properties(cppcore PROPERTIES FOLDER Contrib )
set_target_properties(assimp  PROPERTIES FOLDER Contrib )
