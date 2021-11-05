/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/Light.h>

namespace OSRE {
namespace RenderBackend {

namespace {

void initColors(glm::vec3 *cols, size_t numColors, f32 comp) {
    for (size_t i = 0; i < numColors; ++i) {
        cols[i].r = cols[i].g = cols[i].b = comp;
    }
}

} // namespace

Light::Light() :
        mType(LigthType::PointLight),
        mRadius(1.0f) {
    initColors(mColors, 3, 0.5f);
}

Light::Light( LigthType type, f32 radius ) :
        mType( type ),
        mRadius( radius ) {
    initColors(mColors, 3, 0.5f);
}

Light::~Light() {
    // empty
}

void Light::setLightType( LigthType type ) {
    mType = type;
}

LigthType Light::getLightType() const {
    return mType;
}

f32 Light::getRadius() const {
    return mRadius;
}

void Light::setColor( const glm::vec3 &col, ui32 index ) {
    mColors[index] = col;
}

const glm::vec3 &Light::getColor( ui32 index ) const {
    return mColors[index];
}

} // namespace RenderBackend
} // namespace OSRE
