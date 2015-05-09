SET( LIBRARY_VERSION "1.0.0" )

IF( WIN32 )
    SET( GL_LIBRARY opengl32 )
ELSE( )
    SET( GL_LIBRARY GL )
ENDIF( )

ADD_LIBRARY( glew SHARED
    src/glew.c
    src/glewinfo.c
    include/GL/glew.h
    include/GL/glxew.h
    include/GL/wglew.h
)

target_link_libraries ( glew ${GL_LIBRARY} ) 
set_target_properties(  glew PROPERTIES FOLDER ThirdParty )
