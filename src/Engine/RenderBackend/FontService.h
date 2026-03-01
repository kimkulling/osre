/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/Material.h"
#include "RenderBackend/MaterialBuilder.h"
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
/// @brief Provides access to the fonts.
//-------------------------------------------------------------------------------------------------
struct FontService {
    /// @brief  Will return the default font-
    /// @return The default font.
    static Font *getDefaultFont() {
        static Font defaultFont;
        defaultFont.Name = "file://buildin_arial.bmp";
        defaultFont.Size = 20;
        defaultFont.Style = 0;

        return &defaultFont;
    }

    /// @brief  Will look for the material for a givens font.
    /// @param      The font description
    /// @return The correspondit material.
    static Material *getFont(const Font &font) {
        if (font.Name.empty()) {
            return nullptr;
        }

        cppcore::TArray<TextureResource *> texResArray;
        const IO::Uri uri("file://assets/Textures/Fonts/" + font.Name);
        TextureResource *texRes = new TextureResource(font.Name, uri);
        texResArray.add(texRes);
        Material *fontMaterial = MaterialBuilder::createBuildinMaterial(font.Name + ".mat", texResArray, VertexType::RenderVertex);

        return fontMaterial;
    }
};

} // namespace RenderBackend
} // namespace OSRE
