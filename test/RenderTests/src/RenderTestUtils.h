#pragma once

#include "RenderTestSuite.h"

template<class T>
class AutoAttach {
public:
    AutoAttach() {
        T *pInstance = ::new T;
        OSRE::RenderTest::RenderTestSuite::getInstance()->attachRenderTest(pInstance);
    }
};

#define ATTACH_RENDERTEST( TestClass ) \
    AutoAttach<TestClass> _##TestClass##_getInstance;
