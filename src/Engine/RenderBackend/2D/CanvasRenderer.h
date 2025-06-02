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

#include "Common/osre_common.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"

namespace OSRE::RenderBackend {

// Forward declarations ---------------------------------------------------------------------------
class RenderBackendService;
class Mesh;

struct DrawCmd;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This class implements the 2D canvas renderer.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT CanvasRenderer : public IRenderPath {
public:
    /// @brief The class constructor.
    /// @param[in] numLayers    The number of layers.
    /// @param[in] x            The x position.
    /// @param[in] y            The y position.
    /// @param[in] w            The width.
    /// @param[in] h            The height.
    CanvasRenderer(i32 numLayers, i32 x, i32 y, i32 w, i32 h);

    /// @brief The class constructor.
    /// @param[in] numLayers    The number of layers.
    /// @param[in] rect         The requested resolution.
    CanvasRenderer(i32 numLayers, const Rect2i &rect);

    /// @brief The class destructor.
    ~CanvasRenderer() override;

    /// @brief Will be called before rendering.
    /// @param[in] rbSrv    The render-backend service.
    void preRender(RenderBackendService *rbSrv) override;

    /// @brief Will be called to render the frame.
    /// @param[in] rbSrv    The render-backend service.
    void render(RenderBackendService *rbSrv) override;

    /// @brief Will be called after rendering.
    /// @param[in] rbSrv    The render-backend service.
    void postRender(RenderBackendService *rbSrv) override;
    
    /// @brief Will set the resolution of the canvas area.
    /// @param[in] x    The x position.
    /// @param[in] y    The y position.
    /// @param[in] w    The width.
    /// @param[in] h    The height.
    void setResolution(i32 x, i32 y, i32 w, i32 h);
    
    /// @brief Will set the resolution of the canvas area. 
    /// @param[in] rect     The resolution as a rectangle.
    void setResolution(const Rect2i &rect);

    /// @brief Will return the resolution of the canvas area.
    /// @param[in] layer    The layer to select for rendering.
    /// @return true if the layer is valid, false if not.
    bool selectLayer(i32 layer);

    /// @brief  Will return the active layer.
    /// @return The active layer.
    i32 getActiveLayer() const;

    /// @brief Will set the color for the pen.
    /// @param[in] color    The color to set.
    void setColor(const Color4 &color);

    /// @brief Will return the current color.
    /// @return The current color.
    const Color4 &getColor() const;

    /// @brief Will draw a 2D-line.
    /// @param[in] x1    The x position of the start point.
    /// @param[in] y1    The y position of the start point.
    /// @param[in] x2    The x position of the end point.
    /// @param[in] y2    The y position of the end point.
    void drawline(i32 x1, i32 y1, i32 x2, i32 y2);
    
    /// @brief Will draw a 2D-line.
    /// @param[in] p1      The first point.
    /// @param[in] p2      The second point.
    void drawline(const Point2Di &p1, const Point2Di &p2);

    /// @brief Will draw a 2D-triangle.
    /// @param[in] x1       The x position of the first point.
    /// @param[in] y1       The y position of the first point.
    /// @param[in] x2       The x position of the second point.
    /// @param[in] y2       The y position of the second point.
    /// @param[in] x3       The x position of the third point.
    /// @param[in] y3       The y position of the third point.
    /// @param[in] filled   The flag to set the triangle filled or not.
    void drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, bool filled);
    
    /// @brief Will draw a 2D-triangle.
    /// @param[in] p1  The first point.
    /// @param[in] p2  The second point.
    /// @param[in] p3  The third point.
    /// @param[in] filled The flag to set the triangle filled or not.
    void drawTriangle(const Point2Di &p1, const Point2Di &p2, const Point2Di &p3, bool filled);

    /// @brief Will draw a 2D-rectangle.
    /// @param[in] x        The x position of the rectangle.
    /// @param[in] y        The y position of the rectangle.
    /// @param[in] w        The width of the rectangle.
    /// @param[in] h        The height of the rectangle.
    /// @param[in] filled   The flag to set the rectangle filled or not.
    void drawRect(i32 x, i32 y, i32 w, i32 h, bool filled);

    /// @brief Will select a font.
    /// @param[in] font The font to select.
    void selectFont(Font *font);

    /// @brief Will return the active font.
    /// @return The active font.
    Font *getActiveFont() const;

    /// @brief Will draw a text using the current font.
    /// @param[in] x    The x position.
    /// @param[in] y    The y position.
    /// @param[in] text The text to draw.
    void drawText(i32 x, i32 y, i32 size, const String &text);

    /// @brief Will set the dirty flag.
    void setDirty();

    /// @brief Will set the clean flag.
    void setClean();

    /// @brief Will return true, if recreation of the render context needs to be done.
    /// @return The flag.
    bool isDirty() const;

protected:
    bool onCreate() override;

private:
    bool mDirty;
    DrawCmdArray mDrawCmdArray;
    DrawCmdArray mFontCmdArray;
    TransformMatrixBlock mTransformMatrixBlock;
    Color4 mPenColor;
    Rect2i mResolution;
    i32 mActiveLayer;
    i32 mNumLayers;
    Font *mFont;
    Mesh *mMesh;
    Mesh *mText;
};

inline Font* CanvasRenderer::getActiveFont() const {
    return mFont;
}

inline void CanvasRenderer::setDirty() {
    mDirty = true;
}

inline void CanvasRenderer::setClean() {
    mDirty = false;
}

inline bool CanvasRenderer::isDirty() const {
    return mDirty;
}

} // namespace OSRE::RenderBackend
