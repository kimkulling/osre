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
#include <src/Engine/IO/ImageCodec.h>
#include <osre/IO/Stream.h>
#include <osre/Common/Logger.h>

#include "SOIL.h"

namespace OSRE {
namespace IO {

static const String Extensions = "jpg|png";
static const String Tag        = "ImageCodec";

ImageCodec::ImageCodec()
: AbstractCodec("image", Extensions ) {
    // empty
}

ImageCodec::~ImageCodec() {
    // empty
}

bool ImageCodec::encode( IO::Stream *inStream, uc8 *data, ui32 &size ) {
    if ( nullptr == inStream ) {
        osre_debug( Tag, "Input stream is nullptr." );
        return false;
    }
    if ( 0 == size ) {
        return true;
    }
    unsigned char *buffer = new unsigned char[ size ];
    i32 width( 0 ), height( 0 ), channels( 0 );
    data = SOIL_load_image_from_memory( buffer, size, &width, &height, &channels, SOIL_LOAD_AUTO );

    return true;
}

bool ImageCodec::decode( IO::Stream * ) {
    return false;
}

void ImageCodec::releaseData( uc8 *data ) {
    if ( nullptr == data ) {
        osre_error( Tag, "Pointer to data buffer is nullptr." );
        return;
    }

    SOIL_free_image_data( data );
}

}
}
