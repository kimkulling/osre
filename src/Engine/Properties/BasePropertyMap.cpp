/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Debugging/osre_debugging.h"
#include "Properties/BasePropertyMap.h"
#include "Properties/Property.h"

#include <cassert>

namespace OSRE {
namespace Properties {

using namespace ::cppcore;

BasePropertyMap::BasePropertyMap() :
        m_PropertyLookupMap() {
    // empty
}

BasePropertyMap::~BasePropertyMap() {
    for (auto & it : m_PropertyLookupMap) {
        osre_assert(nullptr != it.second);
        delete it.second;
    }
}

void BasePropertyMap::setProperty(ui32 id, const String &rName, const Variant &val) {
    if (hasProperty(id)) {
        removeProperty(id);
    }

    Property *pProperty = new Property(id, "", val);
    if (!rName.empty()) {
        pProperty->setPropertyName(rName);
    }
    m_PropertyLookupMap[id] = pProperty;
}

void BasePropertyMap::setProperty(ui32 id, Property *pProperty) {
    osre_assert(nullptr != pProperty);

    if (hasProperty(id)) {
        removeProperty(id);
    }

    m_PropertyLookupMap[id] = pProperty;
}

bool BasePropertyMap::hasProperty(ui32 id) const {
    if (Property::NotSet == id) {
        return false;
    }

    PropertyLookupMap::const_iterator it = m_PropertyLookupMap.find(id);
    if (m_PropertyLookupMap.end() == it) {
        return false;
    }

    return true;
}

Property *BasePropertyMap::getProperty(ui32 id) const {
    if (Property::NotSet == id) {
        return nullptr;
    }

    PropertyLookupMap::const_iterator it = m_PropertyLookupMap.find(id);
    if (m_PropertyLookupMap.end() == it) {
        return nullptr;
    }

    return it->second;
}

bool BasePropertyMap::removeProperty(ui32 id) {
    PropertyLookupMap::iterator it = m_PropertyLookupMap.find(id);
    if (m_PropertyLookupMap.end() == it) {
        return false;
    }

    m_PropertyLookupMap.erase(it);

    return true;
}

ui32 BasePropertyMap::size() const {
    return static_cast<ui32>(m_PropertyLookupMap.size());
}

bool BasePropertyMap::isEmpty() const {
    return m_PropertyLookupMap.empty();
}

void BasePropertyMap::clear() {
    for (PropertyLookupMap::iterator it = m_PropertyLookupMap.begin(); it != m_PropertyLookupMap.end(); ++it) {
        delete it->second;
    }
    m_PropertyLookupMap.clear();
}

} // Namespace Properties
} // Namespace OSRE
