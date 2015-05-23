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

#include <osre/Properties/IPropertyMap.h>

#include <map>

namespace OSRE {
namespace Properties {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Properties::BasePropertyMap
///	@ingroup	Infrastructure
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT BasePropertyMap : public IPropertyMap {
public:
    ///	@brief	The default constructor.
    BasePropertyMap();

    ///	@brief	The copy constructor.
    ///	@param	rOther	The instance to copy from.
    BasePropertyMap( const BasePropertyMap &rOther );

    ///	@brief	The destructor.
    ~BasePropertyMap();

    ///	@brief	A new property will be set.
    ///	@param	rName	The name of the property.
    ///	@param	rValue	The value of the property.
    void setProperty( ui32 id, const String &rName, const CPPCore::Variant &rValue );

    void setProperty( ui32 id, Property *Property );

    ///	@brief	Returns true, if the name is stored as a property.
    ///	@param	rName	The name of the property.
    ///	@return	true, if the property is there, false if not.
    bool hasProperty( ui32 id ) const;
    
    ///	@brief	Returns the property assigned to the name.
    ///
    Property *getProperty( ui32 id ) const;
    bool removeProperty( ui32 id );
    ui32 size() const;
    bool isEmpty() const;
    void clear();
    
private:
    BasePropertyMap &operator = ( const BasePropertyMap &rOther );
    bool operator == ( const BasePropertyMap &rOther ) const;


private:
    typedef std::map<ui32, Property*> PropertyLookupMap;
    PropertyLookupMap m_PropertyLookupMap;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Properties
} // Namespace OSRE

