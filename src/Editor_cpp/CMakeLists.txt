INCLUDE_DIRECTORIES(
    ./
    ./src
    ${PROJECT_SOURCE_DIR}
)

SET( inspector_module_src 
    src/Modules/InspectorModule/InspectorModule.cpp
    src/Modules/InspectorModule/InspectorModule.h
)

SET( log_module_src
    src/Modules/LogModule/LogModule.h
    src/Modules/LogModule/LogModule.cpp
)

SET( editor_module_src
    src/Modules/EditorModule/EditorModule.h
    src/Modules/EditorModule/EditorModule.cpp
)

SET( modules_src 
    src/Modules/IModuleView.h
    src/Modules/ModuleBase.cpp
    src/Modules/ModuleBase.h
)


SET( scripting_src 
    src/Scripting/LuaBase.cpp
    src/Scripting/LuaBase.h
)

SOURCE_GROUP( Modules\\InspectorModule FILES ${inspector_module_src} )
SOURCE_GROUP( Modules\\LogModule       FILES ${log_module_src} )
SOURCE_GROUP( Modules\\EditorModule    FILES ${editor_module_src} )

SOURCE_GROUP( Modules\\                FILES ${modules_src} )
SOURCE_GROUP( Scripting\\              FILES ${scripting_src} )

ADD_EXECUTABLE( osre_ed
    ${inspector_module_src}
    ${log_module_src}
    ${editor_module_src}
    ${modules_src}
    ${scripting_src}
    src/OsreEdApp.cpp
    src/OsreEdApp.h
    src/main.cpp
)

target_link_libraries ( osre_ed osre )
