INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
    ../../3dparty/imgui/
)

SET( imgui_src
    ../../3dparty/imgui/imconfig.h
    ../../3dparty/imgui/imgui.cpp
    ../../3dparty/imgui/imgui.h 
    #../../3dparty/imgui/imgui_demo.cpp
    ../../3dparty/imgui/imgui_draw.cpp
    ../../3dparty/imgui/imgui_internal.h 
    ../../3dparty/imgui/imgui_widgets.cpp
    ../../3dparty/imgui/imstb_rectpack.h
    ../../3dparty/imgui/imstb_textedit.h
    ../../3dparty/imgui/imstb_truetype.h
)
ADD_EXECUTABLE( osre_ed
    ${imgui_src}
    src/main.cpp
)

