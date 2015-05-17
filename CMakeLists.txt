CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( osre )
SET ( OSRE_VERSION_MAJOR 0 )
SET ( OSRE_VERSION_MINOR 1 )
SET ( OSRE_VERSION_PATCH 0 )
SET ( OSRE_VERSION ${OSRE_VERSION_MAJOR}.${OSRE_VERSION_MINOR}.${OSRE_VERSION_PATCH} )
SET ( PROJECT_VERSION "${OSRE_VERSION}" )


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
    d:/projects/libs/sdl2/SDL2-2.0.3/include
    c:/Users/kullingk/Desktop/projects/libs/SDL2-2.0.3/include
    3dparty/soil/src
    /usr/include/SDL2/
    /usr/include/
)

link_directories(
    d:/projects/libs/sdl2/SDL2-2.0.3/lib/x86
    ${CMAKE_HOME_DIRECTORY}/lib
    ${CMAKE_HOME_DIRECTORY}/bin
)

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if( WIN32 )
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
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++0x")
    endif()
endif()

# Generate a pkg-config .pc for the ZFXCE2 library.
#CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/zfxce2.pc.in" "${PROJECT_BINARY_DIR}/zfxce2.pc" @ONLY )
#INSTALL( FILES "${PROJECT_BINARY_DIR}/zfxce2.pc" DESTINATION ${LIB_INSTALL_DIR}/pkgconfig/ COMPONENT zfxce2-dev)

# Include all sub directories of the engine code component
ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( test/RenderTests )
ADD_SUBDIRECTORY( 3dparty/glew )
ADD_SUBDIRECTORY( 3dparty/cppcore/build )
ADD_SUBDIRECTORY( 3dparty/zlib )
ADD_SUBDIRECTORY( 3dparty/unzip )
ADD_SUBDIRECTORY( 3dparty/soil )

# Include the doc component.
#ADD_SUBDIRECTORY( Doc )
FIND_PACKAGE( doxygen )
IF ( DOXYGEN_FOUND )
    CONFIGURE_FILE( Doc/Doxygen/OSRE_dox_config.in Doc/Doxygen/Doxyfile @ONLY )
    ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} Doc/Doxygen/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
ENDIF ( DOXYGEN_FOUND )
