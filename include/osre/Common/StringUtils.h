/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Types.h>

namespace OSRE {
namespace Common {

class OSRE_DLL_EXPORT StringUtils {
public:
    static HashId hashName( char const *pIdentStr );
};

inline
HashId StringUtils::hashName( char const *pIdentStr ) {
    // Relatively simple hash of arbitrary text string into a
    // 32-bit identifier Output value is
    // input-valid-deterministic, but no guarantees are made
    // about the uniqueness of the output per-input
    //
    // Input value is treated as lower-case to cut down on false
    // separations cause by human mistypes. Sure, it could be
    // construed as a programming error to mix up your cases, and
    // it cuts down on permutations, but in Real World Usage
    // making this text case-sensitive will likely just lead to
    // Pain and Suffering.
    //
    // This code lossely based upon the adler32 checksum by Mark
    // Adler and published as part of the zlib compression
    // library sources.

    // largest prime smaller than 65536
    unsigned long BASE = 65521L;

    // NMAX is the largest n such that 255n(n+1)/2 +
    // (n+1)(BASE-1) <= 2^32-1
    unsigned long NMAX = 5552;

#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

    if (pIdentStr == NULL)
        return 0L;

    unsigned long s1 = 0;
    unsigned long s2 = 0;

    for ( size_t len = strlen( pIdentStr ); len > 0 ; ) {
        unsigned long k = len < NMAX ? len : NMAX;
        len -= k;

        while (k >= 16)	{
            DO16(pIdentStr);
            pIdentStr += 16;
            k -= 16;
        }

        if (k != 0) do {
            s1 += tolower( *pIdentStr++ );
            s2 += s1;
        } while (--k);

        s1 %= BASE;
        s2 %= BASE;
    }

#ifdef CE_WINDOWS
#   pragma warning(push)
#   pragma warning(disable : 4312)
#endif // CE_WINDOWS
    return static_cast<HashId>( (s2 << 16) | s1 );

#ifdef CE_WINDOWS
#   pragma warning(pop)
#endif // CE_WINDOWS
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}

}
}
