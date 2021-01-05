/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/Common/StringUtils.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace RenderBackend {

struct VertexLayout;

const c8 *getVertexCompShortCut(VertexAttribute &attrib);
const c8 *getAccessShortCut(BufferAccessType access);

template <class TBuffer>
class HWBufferManager {
public:
    using BufferDict = CPPCore::THashMap<ui32, TBuffer*>;

    HWBufferManager();
    ~HWBufferManager();
    TBuffer *createBuffer(const VertexLayout &vertexLayout, BufferAccessType access);
    TBuffer *getBufferByDesc(VertexLayout vertexLayout, BufferAccessType access);
    void clear();

private:
    CPPCore::TArray<TBuffer*> m_buffers;
    BufferDict m_bufferDict;
};

inline void getBufferKey(const VertexLayout &vertexLayout, BufferAccessType access, ui32 &hash) {
    String key;
    for (ui32 i = 0; i < vertexLayout.numComponents(); ++i) {
        VertComponent &vc = vertexLayout.getAt(i);
        key += getVertexCompShortCut(vc.m_attrib);
    }
    key += getAccessShortCut(access);
    hash = Common::StringUtils::hashName(key);
}

template <class TBuffer>
inline HWBufferManager<TBuffer>::HWBufferManager() :
        m_buffers(),
        m_bufferDict() {
    // empty
}

template <class TBuffer>
inline HWBufferManager<TBuffer>::~HWBufferManager() {
    // empty
}

template <class TBuffer>
TBuffer *HWBufferManager<TBuffer>::createBuffer(const VertexLayout &vertexLayout, BufferAccessType access) {
    TBuffer *buffer = new TBuffer();

    ui32 hash = 0;
    getBufferKey(vertexLayout, access, hash);
    m_bufferDict.insert(hash, buffer);
    const size_t id = m_buffers.size();
    m_buffers.add(buffer);
    buffer->m_id = id;

    return buffer;
}

template <class TBuffer>
inline TBuffer *HWBufferManager<TBuffer>::getBufferByDesc(VertexLayout vertexLayout, BufferAccessType access) {
    if (m_buffers.isEmpty()) {
        return nullptr;
    }

    ui32 hash = 0;
    getBufferKey(vertexLayout, access, hash);
    TBuffer *buffer = nullptr;
    if (m_bufferDict.hasKey(hash)) {
        m_bufferDict.getValue(hash, buffer);
    }

    return buffer;
}

template <class TBuffer>
inline void HWBufferManager<TBuffer>::clear() {
    for (ui32 i = 0; i < m_buffers.size(); ++i) {
        delete m_buffers[i];
    }
    m_buffers.clear();
    m_bufferDict.clear();
}

} // namespace RenderBackend
} // namespace OSRE
