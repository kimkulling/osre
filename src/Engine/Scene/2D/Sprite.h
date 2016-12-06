#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Scene {

class Sprite {
public:
    Sprite( const String &name );
    ~Sprite();

private:
    String m_name;
};

} // Namespace Scene
} // Namespace OSRE
