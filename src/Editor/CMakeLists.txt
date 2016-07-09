SET( LIBRARY_VERSION "0.1.0" )

SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/osre/ )
add_definitions( -DOSRE_BUILD_EXPORT )

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${HEADER_PATH}/include
    ../3dparty/cppcore/include
    ../3dparty/glew/include
    ../3dparty/glm/
    ../3dparty/unzip/
    ../3dparty/zlib/
)

# For Vulkan support
if( NOT USE_PLATFORM MATCHES "VK_USE_PLATFORM_.*" )
	if( WIN32 )
		set( USE_PLATFORM "VK_USE_PLATFORM_WIN32_KHR" )
	elseif( UNIX )
		set( USE_PLATFORM "VK_USE_PLATFORM_XCB_KHR" )
	endif()
endif()

IF( WIN32 )
    SET( platform_libs comctl32.lib Winmm.lib opengl32.lib glu32.lib SDL2 )
ELSE( WIN32 )
    SET( platform_libs SDL2 )
ENDIF( WIN32 )

#==============================================================================
# Common
#==============================================================================    
SET( common_src
    Common/main.cpp
)


#==============================================================================
# Defined Source groups
#==============================================================================
SOURCE_GROUP( Common              FILES ${common_src} )

add_definitions( -D${USE_PLATFORM} )

#==============================================================================
# Define the Infrastructure library
#==============================================================================
ADD_EXECUTABLE( osre_ed
    ${common_src}
)

target_link_libraries ( osre_ed osre cppcore zlib Unzip glew soil ${platform_libs} )
set_target_properties(  osre_ed PROPERTIES FOLDER Editor )

#==============================================================================
# Define the install targets.
#==============================================================================
#INSTALL( TARGETS osre
#    DESTINATION ${LIB_INSTALL_DIR}
#    COMPONENT lib_ce2_infrastructure${ZFXCE2_VERSION_MAJOR}
#)
