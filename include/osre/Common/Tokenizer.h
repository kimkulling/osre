/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	This class implements a simple string tokenizer.
///
///	You can use it to split an arbitrary strings by self-defined separators. You can use it to
///	parse a string from any source.
///	Here is a simple example:
///	@code
///	String tst = "/usr/bin/ls";
///	TArray<String> token;
///	Tokenizer::tokenize( tst, token, "/" );
///	@endcode
///	This will return the strings usr, bin and ls.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Tokenizer {
public:
    ///	@brief	The class default constructor.
    Tokenizer();
    
    ///	@brief	The class copy constructor.
    ///	@param	rInstance	[in] Instance to copy from.
    Tokenizer( const Tokenizer &rOther );

    ///	@brief	The class destructor.
    ~Tokenizer();

    ///	@brief	Returns the number of stored token.
    ///	@return	The number of stored token.
    size_t getNumOfToken() const;
    
    ///	@brief	Returns the token at the given index.
    ///	@param	idx		[in] The given index, must be between ( 0 ... numToken-1 ).
    ///	@return	The token at the given index. If the index is out of range an empty string will be 
    ///			returned.
    const String &getTokenAt( ui32 idx ) const;
    
    ///	@brief	Returns true, if the stored list of token is empty.
    ///	@return	true, if no token are store, false if tokens are stored.
    bool isEmpty() const;

    ///	@brief	The internal token list will be cleared.
    void clear();
    
    ///	@brief	Will tokenize a given string by a given delimiter ( static version ).
    ///	@param	str			[in] The string to tokenize.
    ///	@param	tokens		[in] The result token list.
    ///	@param	delimiters	[in] The delimiter.
    ///	@return	The number of token.
    static size_t tokenize( const String& str, CPPCore::TArray<String>& tokens, const String& delimiters );

    ///	@brief	Will tokenize a given string by a given delimiter.
    ///	@param	str			[in] The string to tokenize.
    ///	@param	delimiters	[in] The delimiter.
    ///	@return	The number of token.
    size_t tokenize( const String& str, const String& delimiters );

    static bool isLineBreak( c8 c );

private:
    CPPCore::TArray<String> m_Token;
};

} // Namespace Common
} // Namespace OSRE

