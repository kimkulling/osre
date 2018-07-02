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

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class SamplerState {
public:
    SamplerState();
    SamplerState( TextureTargetType targetType, TextureStageType stageType);
    ~SamplerState();
    bool operator == ( const SamplerState &rhs ) const;
    bool operator != ( const SamplerState &rhs ) const;

private:
    TextureTargetType m_targetType;
    TextureStageType  m_stageType;
};

inline
SamplerState::SamplerState() 
: m_targetType( TextureTargetType::Texture2D )
, m_stageType( TextureStageType::TextureStage0 ) {
    // empty
}

inline
SamplerState::SamplerState( TextureTargetType targetType, TextureStageType stageType)
: m_targetType( targetType )
, m_stageType( stageType ) {

    // empty
}

inline
SamplerState::~SamplerState() {
    // empty
}

inline
bool SamplerState::operator == ( const SamplerState &rhs ) const {
    return ( m_targetType == rhs.m_targetType 
        &&  m_stageType == rhs.m_stageType );
}

inline
bool SamplerState::operator != ( const SamplerState &rhs ) const {
    return !( *this == rhs );
}

} // Namespace RenderBackend
} // Namespace OSRE
