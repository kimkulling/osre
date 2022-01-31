
ADD_EXECUTABLE( osre_player
    main.cpp
)

IF( WIN32 )
    SET(platform_libs comctl32.lib Winmm.lib SDL2)
ELSE( WIN32 )
    SET(platform_libs SDL2)
ENDIF( WIN32 )

target_link_libraries(osre_player osre {$platform_libs})

set_target_properties(osre_player PROPERTIES FOLDER Player)
