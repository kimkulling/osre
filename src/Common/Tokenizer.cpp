/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions 
  and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
  and the following disclaimer in the documentation and/or other materials provided with the 
  distribution.

* Neither the name of the ASSIMP team, nor the names of its contributors may be used to endorse or 
  promote products derived from this software without specific prior written permission of the 
  ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <osre/Common/Tokenizer.h>

namespace OSRE {
namespace Common {

using namespace ::CPPCore;

static const String EmptyToken = "";

//-------------------------------------------------------------------------------------------------
Tokenizer::Tokenizer() 
: m_Token() {
    // empty
}

//-------------------------------------------------------------------------------------------------
Tokenizer::Tokenizer( const Tokenizer &rhs ) 
: m_Token( rhs.m_Token ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
Tokenizer::~Tokenizer() {
    // empty
}

//-------------------------------------------------------------------------------------------------
ui32 Tokenizer::getNumOfToken() const {
    return m_Token.size();
}

//-------------------------------------------------------------------------------------------------
const String &Tokenizer::getTokenAt( ui32 index ) const {
    if ( m_Token.isEmpty() || m_Token.size() >= index )
        return EmptyToken;

    return m_Token[ index ];
}

//-------------------------------------------------------------------------------------------------
bool Tokenizer::isEmpty() const {
    return m_Token.isEmpty();
}

//-------------------------------------------------------------------------------------------------
void Tokenizer::clear() {
    m_Token.clear();
}

//-------------------------------------------------------------------------------------------------
ui32 Tokenizer::tokenize( const String& str, TArray<String>& tokens, const String& delimiters ) {
    // Skip delimiters at beginning.
    String::size_type lastPos = str.find_first_not_of( delimiters, 0 );
    
    // find first "non-delimiter".
    String::size_type pos = str.find_first_of( delimiters, lastPos );
    while( String::npos != pos || String::npos != lastPos ) {
        // Found a token, add it to the vector.
        String tmp = str.substr( lastPos, pos - lastPos );
        if ( !tmp.empty() && ' ' != tmp[ 0 ] ) {
            tokens.add( tmp );
        }
        
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of( delimiters, pos );
        
        // Find next "non-delimiter"
        pos = str.find_first_of( delimiters, lastPos );
    }

    return static_cast<ui32>( tokens.size() );
}
//-------------------------------------------------------------------------------------------------
ui32 Tokenizer::tokenize( const String& str, const String& delimiters ) {
    if( !m_Token.isEmpty() ) {
        m_Token.resize( 0 );
    }
    
    tokenize( str, m_Token, delimiters );
    
    return m_Token.size();
}

//-------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace OSRE
