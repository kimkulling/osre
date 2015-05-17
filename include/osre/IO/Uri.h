#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::Uri
///	@ingroup	Infrastructure
///
///	@brief	This class represents a URI ( see
///	http://en.wikipedia.org/wiki/Uniform_Resource_Identifier ). You can use it to describe the
///	location of resources like:
///
///	file://c:/texture-jpg
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Uri {
public:
    ///	@brief	The default class constructor.
    Uri();

    ///	@brief	The class constructor with a string containing a valid URI. If the string description
    ///			is not ok the URI representation will be empty.
    ///	@param	uri			[in] The string with the URI.
    Uri( const String &uri );

    ///	@brief	The copy class constructor.
    ///	@param	rhs		[in] Instance to copy.
    Uri( const Uri &rhs );

    ///	@brief	The class destructor.
    ~Uri();

    ///	@brief	Assigns a new uri string, the older data will be erased.
    ///	@param	uri			[in] The string with the URI.
    void setUri( const String &uri );

    ///	@brief	Returns the uri string.
    ///	@return	The uri string.
    const String &getUri() const;

    ///	@brief	Returns the scheme of the URI.
    ///	@return	A string containing the scheme.
    const String &getScheme() const;

    ///	@brief	Returns the path of the URI.
    ///	@return	A string containing the path.
    const String &getPath() const;

    ///	@brief	Returns the resource of the URI.
    ///	@return	A string containing the resource.
    const String &getAbsPath() const;

    ///	@brief	Returns the absolute path ( path + resource ) of the URI.
    ///	@return	A string containing the absolute path.
    const String &getResource() const;

    ///	@brief	Returns true, if the scheme is empty.
    ///	@return	true, if the URI is empty, false if not.
    bool isEmpty() const;

    ///	@brief	Parse the URI.
    ///	@return	true, if the URI is valid, false if not.
    bool parse();

    ///	@brief	Will return true, if the uri content is valid.
    ///	@return	true, if the uri itself is valid, false if not.
    bool isValid() const;

    ///	@brief	The URI representation will be cleared.
    void clear();

    ///	@brief	The assignment operator.
    Uri &operator = ( const Uri &rhs );

    ///	@brief	The compare operator.
    bool operator == ( const Uri &rhs ) const;

private:
    String m_URI;
    String m_Scheme;
    String m_Path;
    String m_AbsPath;
    String m_Resource;
};

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
