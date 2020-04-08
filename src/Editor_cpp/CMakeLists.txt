INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

ADD_EXECUTABLE( osre_ed
    src/OsreEdApp.cpp
    src/OsreEdApp.h
    src/main.cpp
)

target_link_libraries ( osre_ed osre )
