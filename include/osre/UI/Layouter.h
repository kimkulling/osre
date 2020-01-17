#pragma once

#include <osre/UI/UICommon.h>

namespace OSRE {
namespace UI {

class Widget;

class Layouter {
public:
    Layouter( LayoutPolicy policy );
    LayoutPolicy getPolicy() const;
    void layout( Widget *item );

private:
    LayoutPolicy m_policy;
};

}
}

