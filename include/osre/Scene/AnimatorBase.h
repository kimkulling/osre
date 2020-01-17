#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Scene {

template<class T>
struct AnimationBase {
    void operator () ( T &out, const T &a, const T &b, f32 d ) const {
        out = a + ( b - a ) * d;
    }
};

}
}
