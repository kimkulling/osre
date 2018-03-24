# Install script for directory: C:/projects/osre/3dparty/assimp/code

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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/projects/osre/lib/Debug/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/projects/osre/lib/Release/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/projects/osre/lib/MinSizeRel/assimp-vc140-mt.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/projects/osre/lib/RelWithDebInfo/assimp-vc140-mt.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/projects/osre/bin/Debug/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/projects/osre/bin/Release/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/projects/osre/bin/MinSizeRel/assimp-vc140-mt.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/projects/osre/bin/RelWithDebInfo/assimp-vc140-mt.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/anim.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/ai_assert.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/camera.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/color4.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/color4.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/config.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/defs.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Defines.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/cfileio.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/light.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/material.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/material.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/matrix3x3.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/matrix3x3.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/matrix4x4.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/matrix4x4.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/mesh.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/pbrmaterial.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/postprocess.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/quaternion.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/quaternion.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/scene.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/metadata.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/texture.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/types.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/vector2.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/vector2.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/vector3.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/vector3.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/version.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/cimport.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/importerdesc.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Importer.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/DefaultLogger.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/ProgressHandler.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/IOStream.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/IOSystem.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Logger.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/LogStream.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/NullLogger.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/cexport.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Exporter.hpp"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/DefaultIOStream.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/DefaultIOSystem.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/SceneCombiner.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/fast_atof.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/qnan.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/BaseImporter.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Hash.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/ParsingUtils.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/StreamReader.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/StreamWriter.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/StringComparison.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/StringUtils.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/SGSpatialSort.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/GenericProperty.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/SpatialSort.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/SmoothingGroups.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/SmoothingGroups.inl"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/StandardShapes.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/RemoveComments.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Subdivision.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Vertex.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/LineSplitter.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/TinyFormatter.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Profiler.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/LogAux.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Bitmap.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/XMLTools.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/IOStreamBuffer.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/CreateAnimMesh.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/irrXMLWrapper.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/BlobIOSystem.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/MathFunctions.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Macros.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Exceptional.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Compiler/poppack1.h"
    "C:/projects/osre/3dparty/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/projects/osre/3dparty/assimp/code/Debug/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/projects/osre/3dparty/assimp/code/RelWithDebInfo/assimp-vc140-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

