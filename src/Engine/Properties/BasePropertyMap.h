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
#pragma once

#include "Properties/PropertiesCommon.h"
#include "Properties/AbstractPropertyMap.h"

#include <map>

namespace OSRE {
namespace Properties {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT BasePropertyMap : public AbstractPropertyMap {
public:
    ///	@brief	The default constructor.
    BasePropertyMap();

    ///	@brief	The copy constructor.
    ///	@param	rOther	The instance to copy from.

    ///	@brief	The class destructor.
    virtual ~BasePropertyMap();

    ///	@brief	A new property will be set.
    /// @param  id          [in] The property id.
    ///	@param	name	    [in] The name of the property.
    ///	@param	value	    [in] The value of the property.
    virtual void setProperty(ui32 id, const String &name, const cppcore::Variant &value);

    /// @brief  A new property will be set.
    /// @param  id          [in] The property id.
    /// @param  property    [in] a pointer showing to the new property.
    virtual void setProperty(ui32 id, Property *Property);

    ///	@brief	Returns true, if the name is stored as a property.
    /// @param  id          [in] The property id.
    ///	@return	true, if the property is there, false if not.
    virtual bool hasProperty(ui32 id) const;
    
    ///	@brief	Returns the property assigned to the name.
    /// @param  id          [in] The property id.
    /// @return Pointer showing to the instance or nullptr if property was not found.
    virtual Property *getProperty(ui32 id) const;

    /// @brief  Removes a stored property.
    /// @param  id          [in] The property id.
    /// @return true, if deletion was successful.
    virtual bool removeProperty(ui32 id);

    /// @brief  Returns the number of properties in the map.
    /// @return The number of properties.
    virtual ui32 size() const;

    /// @brief  Returns true, if property is empty.
    /// @return true, when map is empty,
    virtual bool isEmpty() const;

    /// @brief  Clears the map.
    virtual void clear();
    
    // No copying
    BasePropertyMap( const BasePropertyMap & ) = delete;
    BasePropertyMap &operator = ( const BasePropertyMap & ) = delete;
    bool operator == ( const BasePropertyMap & ) const = delete;

private:
    typedef std::map<ui32, Property*> PropertyLookupMap;
    PropertyLookupMap m_PropertyLookupMap;
};

} // Namespace Properties
} // Namespace OSRE
