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

namespace OSRE {

// Forward declarations
namespace IO {
    class Stream;
}

namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Common.Engine
///
///	@brief  The abstract class describes the en- and decoding algorithm for a given file format.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractCodec {
public:
    /// @brief  The class destructor, virtual
    virtual ~AbstractCodec();

    /// @brief  Will encode data.
    /// @return true in case of a success.
    virtual bool encode( IO::Stream *inStream, uc8 *data, ui32 &size );

    /// @brief  Will decode data
    /// @return true in case of a success.
    virtual bool decode( IO::Stream *outStream );

    virtual void releaseData( uc8 *data );

    /// @brief  Returns the codec name.
    /// @return The codec name.
    virtual const String &getCodecName() const;

    /// @brief  Returns the codec extension.
    /// @return The codec extension.
    virtual const String &getCodecExt() const;

protected:
    /// @brief  The class constructor.
    /// @param  name    [in] The codex name.
    ///	@param  ext     [in] The code extension.
    AbstractCodec( const String &name, const String &ext );

    // not used
    AbstractCodec() = delete;
    AbstractCodec( const AbstractCodec & ) = delete ;
    AbstractCodec &operator = ( const AbstractCodec & ) = delete;

private:
    const String m_name;
    const String m_ext;
};

} // Namespace Common
} // Namespace OSRE
