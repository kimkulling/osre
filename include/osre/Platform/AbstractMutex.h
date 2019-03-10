#pragma once

#include "PlatformCommon.h"

namespace OSRE {
namespace Platform {

class OSRE_EXPORT AbstractMutex {
public:
    virtual ~AbstractMutex();
    virtual void lock() = 0;
    virtual bool tryLock() = 0;
    virtual bool unlock() = 0;

protected:
    AbstractMutex();
};

inline
AbstractMutex::AbstractMutex() {
    // empty
}

inline
AbstractMutex::~AbstractMutex() {
    // empty
}

} // Namespace Platform
} // Namespace OSRE
