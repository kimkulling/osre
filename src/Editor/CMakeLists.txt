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
    NativeEditorDll/src/UiLogStream.h
    NativeEditorDll/src/SceneNodeIterator.h
    NativeEditorDll/src/EditorApplication.h
    NativeEditorDll/src/EditorApplication.cpp
    NativeEditorDll/src/EditorMain.cpp
    NativeEditorDll/src/EditorMain.h
)

SET( native_editor_renderentities_src 
    NativeEditorDll/src/RenderEntities/Grid.h
    NativeEditorDll/src/RenderEntities/Grid.cpp
)

SOURCE_GROUP( Include\\Scene         FILES ${scene_inc} )
SOURCE_GROUP( RenderEntities         FILES ${native_editor_renderentities_src} )

#==============================================================================
# Define the editor dll
#==============================================================================
ADD_LIBRARY( osre_nativeeditor SHARED
    ${native_editor_dll_src}
    ${native_editor_renderentities_src}
)

target_link_libraries ( osre_nativeeditor osre ${platform_libs} )
set_target_properties(  osre_nativeeditor PROPERTIES FOLDER Editor )

if( WIN32 )
    include_external_msproject(
        OSREEditor ${PROJECT_SOURCE_DIR}/src/Editor/OSREEditor/OSREEditor.csproj
        TYPE FAE04EC0-301F-11D3-BF4B-00C04F79EFBC)
ENDIF( WIN32 )
