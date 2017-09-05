/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

namespace OSRE {
namespace RenderBackend {

class PolygonState {
public:
    enum class PolygonMode {
        Point,
        Line,
        Fill
    };

    PolygonState();
    ~PolygonState();
    void setPolygoneMode( PolygonMode polyMode );
    PolygonMode getPolygonMode() const;
    bool operator == ( const PolygonState &rhs ) const; 
    bool operator != ( const PolygonState &rhs ) const;

private:
    PolygonMode m_polyMode;
};

inline
PolygonState::PolygonState()
: m_polyMode( PolygonMode::Fill ) {
    // empty
}

inline
PolygonState::~PolygonState() {
    // empty
}

inline
void PolygonState::setPolygoneMode( PolygonMode polyMode ) {
    m_polyMode = polyMode;
}

inline
PolygonState::PolygonMode PolygonState::getPolygonMode() const {
    return m_polyMode;
}

inline
bool PolygonState::operator == ( const PolygonState &rhs ) const {
    return ( m_polyMode == rhs.m_polyMode );
}

inline
bool PolygonState::operator != ( const PolygonState &rhs ) const {
    return !( *this == rhs );
}

}
}
