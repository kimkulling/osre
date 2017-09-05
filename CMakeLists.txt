CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( osre )
SET ( OSRE_VERSION_MAJOR 0 )
SET ( OSRE_VERSION_MINOR 1 )
SET ( OSRE_VERSION_PATCH 0 )
SET ( OSRE_VERSION ${OSRE_VERSION_MAJOR}.${OSRE_VERSION_MINOR}.${OSRE_VERSION_PATCH} )
SET ( PROJECT_VERSION "${OSRE_VERSION}" )

OPTION( OSRE_BUILD_SAMPLES
  "Build the samples of OSRE."
  ON
)

# Cache these to allow the user to override them manually.
SET( LIB_INSTALL_DIR "lib" CACHE PATH
    "Path the built library files are installed to." )
SET( INCLUDE_INSTALL_DIR "osre" CACHE PATH
    "Path the header files are installed to." )
SET( BIN_INSTALL_DIR "bin" CACHE PATH
    "Path the tool executables are installed to." )

INCLUDE_DIRECTORIES(
    include/
    3dparty/glew/include
    3dparty/assimp/contrib/irrXML
    3dparty/zlib
    d:/projects/libs/SDL2-2.0.3/include
    c:/Users/kullingk/Desktop/projects/libs/SDL2-2.0.3/include
    d:/projects/libs/SDL2-2.0.5/include
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10586.0/um"
    3dparty/soil/src
    /usr/include/SDL2/
    /usr/include/
	"C:/projects/libs/SDL2-2.0.5/include"
)

link_directories(
    d:/projects/libs/SDL2-2.0.5/lib/x86
    d:/libs/SDL2-2.0.5/lib/x86
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10586.0/um/x86"
	"C:/projects/libs/SDL2-2.0.5/lib/x86"
    ${CMAKE_HOME_DIRECTORY}/lib
    ${CMAKE_HOME_DIRECTORY}/bin
)

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

IF ( BUILD_SHARED_LIBS AND CMAKE_SIZEOF_VOID_P EQUAL 8 ) # -fPIC is only required for shared libs on 64 bit
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
  SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC" )
ENDIF()

if( WIN32 )
    add_definitions( -DWIN32_LEAN_AND_MEAN )
else()
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


SET( BUILD_SHARED_LIBS OFF )
# Include all sub directories of the engine code component
ADD_SUBDIRECTORY( src/Engine )
ADD_SUBDIRECTORY( src/Editor )
ADD_SUBDIRECTORY( test/RenderTests )
ADD_SUBDIRECTORY( test/UnitTests )
IF ( OSRE_BUILD_SAMPLES )
    ADD_SUBDIRECTORY( samples )
ENDIF(OSRE_BUILD_SAMPLES)
ADD_SUBDIRECTORY( 3dparty/glew )
ADD_SUBDIRECTORY( 3dparty/cppcore/build )
ADD_SUBDIRECTORY( 3dparty/zlib )
ADD_SUBDIRECTORY( 3dparty/unzip )
ADD_SUBDIRECTORY( 3dparty/soil )
ADD_SUBDIRECTORY( 3dparty/assimp/ )


# Include the doc component.
#ADD_SUBDIRECTORY( Doc )
#FIND_PACKAGE( doxygen )
#IF ( DOXYGEN_FOUND )
#    CONFIGURE_FILE( Doc/Doxygen/OSRE_dox_config.in Doc/Doxygen/Doxyfile @ONLY )
#    ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} Doc/Doxygen/Doxyfile
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating API documentation with Doxygen" VERBATIM
#    )
#ENDIF ( DOXYGEN_FOUND )

set_target_properties(  cppcore PROPERTIES FOLDER ThirdParty )
set_target_properties(  cppcore_unittest PROPERTIES FOLDER ThirdParty )
set_target_properties(  assimp PROPERTIES FOLDER ThirdParty )
#set_target_properties(  zlibstatic PROPERTIES FOLDER ThirdParty )
