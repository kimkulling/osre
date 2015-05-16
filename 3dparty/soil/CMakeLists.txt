SET( LIBRARY_VERSION "1.0.0" )

SET( HEADER_PATH include )

SET( soil_src
    src/image_DXT.c
    src/image_DXT.h
    src/image_helper.c
    src/image_helper.h
    src/SOIL.c
    src/SOIL.h
    src/stbi_DDS_aug.h
    src/stbi_DDS_aug_c.h
    src/stb_image_aug.c
    src/stb_image_aug.h
)

if( WIN32 )
    ADD_LIBRARY( soil STATIC ${soil_src} )
else()
    ADD_LIBRARY( soil SHARED ${soil_src} )
endif( WIN32 )

set_target_properties(  soil PROPERTIES FOLDER ThirdParty )
