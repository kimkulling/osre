#pragma once

#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/Material.h"
#include "RenderBackend/MaterialBuilder.h"
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

struct FontService {
    static Font *getDefaultFont() {
        static Font defaultFont;
        defaultFont.Name = "buildin_arial";
        defaultFont.Size = 10;
        defaultFont.Style = 0;

        return &defaultFont;
    }

    static Material *getFont(const Font &font) {
        cppcore::TArray<TextureResource *> texResArray;
        TextureResource *texRes = new TextureResource("buildin_arial", IO::Uri("file://assets/Textures/Fonts/buildin_arial.bmp"));
        texResArray.add(texRes);
        Material *fontMaterial = MaterialBuilder::createTexturedMaterial("text_box_tex", texResArray, VertexType::RenderVertex);

        return fontMaterial;
    }
};


}
}
