CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( osre )
set ( OSRE_VERSION_MAJOR 0 )
set ( OSRE_VERSION_MINOR 1 )
set ( OSRE_VERSION_PATCH 0 )
set ( OSRE_VERSION ${OSRE_VERSION_MAJOR}.${OSRE_VERSION_MINOR}.${OSRE_VERSION_PATCH} )
set ( PROJECT_VERSION "${OSRE_VERSION}" )

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

# Cache these to allow the user to override them manually.
set( LIB_INSTALL_DIR "lib" CACHE PATH
    "Path the built library files are installed to." )
set( INCLUDE_INSTALL_DIR "osre" CACHE PATH
    "Path the header files are installed to." )
set( BIN_INSTALL_DIR "bin" CACHE PATH
    "Path the tool executables are installed to." )

INCLUDE_DIRECTORIES(
    include/
    3dparty/glew/include
    3dparty/assimp/contrib/irrXML
    3dparty/zlib
    3dparty/jsoncpp-1.8.4/include
    "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
    3dparty/soil/src
    /usr/include/SDL2/
    /usr/include/
	src/
	$ENV{SDL_PATH}/include
	SDL2-devel-2.0.5-VC.zip/include
)

link_directories(
	$ENV{SDL_PATH}/lib/x86
    ${CMAKE_HOME_DIRECTORY}/lib
    ${CMAKE_HOME_DIRECTORY}/bin
)

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if( WIN32 )
    FIND_PACKAGE( DirectX )
    add_definitions( -DWIN32_LEAN_AND_MEAN )
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC" )
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib-debug)
      set(LIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/lib-debug)
      set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin-dbg)
  else()
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
      set(LIB_INSTALL_DIR ${CMAKE_SOURCE_DIR}/lib)
      set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
  endif()
endif()

# we want to have all wanings
if( WIN32 AND NOT CYGWIN )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )  # Force to always compile with W4
  if( CMAKE_CXX_FLAGS MATCHES "/W[0-4]" )
    string( REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
  else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4" )
  endif()
elseif( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  # Update if necessary
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_EQUAL 4.6 )
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++0x")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=gnu++11")
    endif()
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wno-long-long -pedantic -std=c++11" )
endif()
add_definitions( -DGLM_ENABLE_EXPERIMENTAL )

# Configuring assimp
set( BUILD_SHARED_LIBS OFF )
set( ASSIMP_BUILD_ASSIMP_TOOLS OFF )
set( ASSIMP_BUILD_TESTS OFF )

# Include all sub directories of the engine code component
ADD_SUBDIRECTORY( src/Engine )
IF ( WIN32)
  ADD_SUBDIRECTORY( src/Editor )
ENDIF( WIN32 )

IF ( OSRE_BUILD_TESTS )
    ADD_SUBDIRECTORY( test/RenderTests )
    ADD_SUBDIRECTORY( test/UnitTests )
ENDIF(OSRE_BUILD_TESTS)

IF ( OSRE_BUILD_SAMPLES )
    ADD_SUBDIRECTORY( samples )
ENDIF(OSRE_BUILD_SAMPLES)

ADD_SUBDIRECTORY( 3dparty/glew )
ADD_SUBDIRECTORY( 3dparty/cppcore/build )
ADD_SUBDIRECTORY( 3dparty/zlib )
ADD_SUBDIRECTORY( 3dparty/unzip )
ADD_SUBDIRECTORY( 3dparty/soil )
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
set_target_properties(  cppcore_unittest   PROPERTIES FOLDER ThirdParty )
set_target_properties(  assimp             PROPERTIES FOLDER ThirdParty )
#set_target_properties(  assimp_cmd         PROPERTIES FOLDER ThirdParty )
#set_target_properties(  assimp_viewer      PROPERTIES FOLDER ThirdParty )
#set_target_properties(  IrrXML             PROPERTIES FOLDER ThirdParty )
#set_target_properties(  jsoncpp_lib_static PROPERTIES FOLDER ThirdParty )
#set_target_properties(  zlib               PROPERTIES FOLDER ThirdParty )
#set_target_properties(  zlibstatic         PROPERTIES FOLDER ThirdParty )
#set_target_properties(  unit               PROPERTIES FOLDER ThirdParty )

if( WIN32 )
    set_target_properties(  OSREEditor       PROPERTIES FOLDER Editor )
endif( WIN32 )

