INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

SET( modules_src 
    src/Modules/ModuleBase.cpp
    src/Modules/ModuleBase.h
)


SET( scripting_src 
    src/Scripting/LuaBase.cpp
    src/Scripting/LuaBase.h
)

SOURCE_GROUP( src\\Modules\\    FILES ${modules_src} )
SOURCE_GROUP( src\\Scripting\\  FILES ${scripting_src} )

ADD_EXECUTABLE( osre_ed
    ${modules_src}
    ${scripting_src}
    src/OsreEdApp.cpp
    src/OsreEdApp.h
    src/main.cpp
)

target_link_libraries ( osre_ed osre )
