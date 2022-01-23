
ADD_EXECUTABLE( osre_player
    main.cpp
)

target_link_libraries ( osre_player osre comctl32.lib winmm.lib )

set_target_properties(  osre_player PROPERTIES FOLDER Player )
