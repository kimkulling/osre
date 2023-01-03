/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Tokenizer.h>

namespace OSRE {
namespace Common {

using namespace ::cppcore;

static const String EmptyToken = "";

Tokenizer::Tokenizer() : m_Token() {
    // empty
}

Tokenizer::Tokenizer( const Tokenizer &rhs ) : m_Token( rhs.m_Token ) {
    // empty
}

size_t Tokenizer::getNumOfToken() const {
    return m_Token.size();
}

const String &Tokenizer::getTokenAt( ui32 index ) const {
    if ( m_Token.isEmpty() || m_Token.size() >= index ) {
        return EmptyToken;
    }

    return m_Token[ index ];
}

bool Tokenizer::isEmpty() const {
    return m_Token.isEmpty();
}

void Tokenizer::clear() {
    m_Token.clear();
}

size_t Tokenizer::tokenize(const String &str, StringArray &tokens, const String &delimiters) {
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

size_t Tokenizer::tokenize( const String& str, const String& delimiters ) {
    if( !m_Token.isEmpty() ) {
        m_Token.resize( 0 );
    }
    
    tokenize( str, m_Token, delimiters );
    
    return m_Token.size();
}

bool Tokenizer::isLineBreak( c8 c ) {
    if ( c == '\n' ) {
        return true;
    } else {
        return false;
    }
}

} // Namespace Common
} // Namespace OSRE
