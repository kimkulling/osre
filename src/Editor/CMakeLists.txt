SET( HEADER_PATH ${PROJECT_SOURCE_DIR}/include/osre/ )

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ${HEADER_PATH}/include
    ../../3dparty/cppcore/include
    ../../3dparty/glew/include
    ../../3dparty/glm/
    ../../3dparty/unzip/
    ../../3dparty/zlib/
    ../../3dparty/assimp/include
)

#==============================================================================
# NativeEditorDll
#==============================================================================
SET( native_editor_dll_src
    NativeEditorDll/src/EditorMain.cpp
    NativeEditorDll/src/EditorMain.h
)

SOURCE_GROUP( Include\\Scene         FILES ${scene_inc} )

#==============================================================================
# Define the editor dll
#==============================================================================
ADD_LIBRARY( osre_nativeeditor SHARED
    ${native_editor_dll_src}
)

target_link_libraries ( osre_nativeeditor osre ${platform_libs} )
set_target_properties(  osre_nativeeditor PROPERTIES FOLDER Editor )

include_external_msproject(
    OSREEditor ${PROJECT_SOURCE_DIR}/src/Editor/OSREEditor/OSREEditor.csproj
    TYPE FAE04EC0-301F-11D3-BF4B-00C04F79EFBC)