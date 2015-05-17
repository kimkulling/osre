#pragma once

namespace ZFXCE2 {
namespace Math {

template<class T>
class TVec2 {
    union {
        struct { T x, y; };
        T v[ 2 ];
    };
};

template<class T>
class TVec3 {
    union {
        struct { T x, y, z; };
        T v[ 3 ];
    };
};

template<class T>
class TVec4 {
    union {
        struct { T x, y, z, w; };
        T v[ 4 ];
    };
};

} // Namespace Math
} // Namespace ZFXCE2
