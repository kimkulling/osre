SET( LIBRARY_VERSION "1.0.0" )

INCLUDE_DIRECTORIES( HEADER_PATH 
	../zlib
)

find_library( 
	${ZLIB} Zlib
	PATHS
	"../zlib"
	DOC "The Zlib installation path"
)

SET( unzip_src
	crypt.h
	ioapi.c
	ioapi.h
	unzip.c
	unzip.h	
)

if( WIN32 )
    ADD_LIBRARY( Unzip STATIC ${unzip_src} )
else()
    ADD_LIBRARY( Unzip SHARED ${unzip_src} )
endif()

target_link_libraries ( Unzip zlib ) 
set_target_properties(  Unzip PROPERTIES FOLDER ThirdParty )
