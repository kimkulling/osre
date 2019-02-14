/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/THashMap.h>
#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>

namespace OSRE {
namespace RenderBackend {

class RenderBackendService;
class Mesh;

struct GeoInstanceData;
struct Light;

struct OSRE_EXPORT UniformDataBlob {
    void *m_data;
    ui32  m_size;

    UniformDataBlob();
    ~UniformDataBlob();
    void *getData() const;
    void clear();

    static UniformDataBlob *create( ParameterType type, ui32 arraySize );
};

struct OSRE_EXPORT UniformVar {
    String           m_name;
    ParameterType    m_type;
    ui32             m_numItems;
    UniformDataBlob  m_data;
    UniformVar      *m_next;

    static ui32 getParamDataSize( ParameterType type, ui32 arraySize );
    static UniformVar *create( const String &name, ParameterType type, ui32 arraySize=1 );
    static void destroy( UniformVar *param );

    ui32 getSize();

private:
    UniformVar();
    ~UniformVar();
};

} // Namespace RenderBackend
} // Namespace OSRE
