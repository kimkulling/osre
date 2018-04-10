# Install script for directory: D:/projects/osre/3dparty/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/osre")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/projects/osre/lib/Debug/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/projects/osre/lib/Release/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/projects/osre/lib/MinSizeRel/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/projects/osre/lib/RelWithDebInfo/assimp-vc140-mt.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/anim.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/ai_assert.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/camera.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/color4.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/color4.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/config.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/defs.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Defines.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/cfileio.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/light.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/material.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/material.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/matrix3x3.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/matrix3x3.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/matrix4x4.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/matrix4x4.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/mesh.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/pbrmaterial.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/postprocess.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/quaternion.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/quaternion.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/scene.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/metadata.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/texture.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/types.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/vector2.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/vector2.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/vector3.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/vector3.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/version.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/cimport.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/importerdesc.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Importer.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/IOStream.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/IOSystem.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Logger.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/LogStream.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/NullLogger.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/cexport.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Exporter.hpp"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/SceneCombiner.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/fast_atof.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/qnan.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/BaseImporter.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Hash.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/ParsingUtils.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/StreamReader.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/StreamWriter.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/StringComparison.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/StringUtils.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/GenericProperty.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/SpatialSort.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/StandardShapes.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/RemoveComments.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Subdivision.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Vertex.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/LineSplitter.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/TinyFormatter.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Profiler.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/LogAux.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Bitmap.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/XMLTools.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/irrXMLWrapper.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/MathFunctions.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Macros.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Exceptional.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/projects/osre/3dparty/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/projects/osre/3dparty/assimp/code/Debug/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/projects/osre/3dparty/assimp/code/RelWithDebInfo/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

