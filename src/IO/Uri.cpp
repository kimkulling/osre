/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2012, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions 
  and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
  and the following disclaimer in the documentation and/or other materials provided with the 
  distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or 
  promote products derived from this software without specific prior written permission of the 
  ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <osre/IO/Uri.h>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
Uri::Uri() 
: m_URI( "" )
, m_Scheme( "" )
, m_Path( "" )
, m_AbsPath( "" )
, m_Resource( "" ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
Uri::Uri( const String &rhs )
: m_URI( rhs )
, m_Scheme( "" )
, m_Path( "" )
, m_AbsPath( "" )
, m_Resource( "" ) {
	static_cast<void>( parse() );
}

//-------------------------------------------------------------------------------------------------
Uri::Uri( const Uri &rhs ) 
: m_URI( rhs.m_URI )
, m_Scheme( rhs.m_Scheme )
, m_Path( rhs.m_Path )
, m_AbsPath( rhs.m_AbsPath )
, m_Resource( rhs.m_Resource ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
Uri::~Uri() {
	// empty
}

//-------------------------------------------------------------------------------------------------
void Uri::setUri(  const String &uri ) {
	if ( uri == m_URI ) {
		return;
	}

	clear();
	m_URI = uri;
	static_cast<void>( parse() );
}

//-------------------------------------------------------------------------------------------------
const String &Uri::getUri() const {
	return m_URI;
}

//-------------------------------------------------------------------------------------------------
const String &Uri::getScheme() const {
	return m_Scheme;
}

//-------------------------------------------------------------------------------------------------
const String &Uri::getPath() const {
	return m_Path;
}

//-------------------------------------------------------------------------------------------------
const String &Uri::getAbsPath() const {
	return m_AbsPath;
}

//-------------------------------------------------------------------------------------------------
const String &Uri::getResource() const {
	return m_Resource;
}

//-------------------------------------------------------------------------------------------------
bool Uri::isEmpty() const {
	return m_URI.empty();
}

//-------------------------------------------------------------------------------------------------
bool Uri::parse() {
	if ( m_URI.empty() ) {
		clear();
		return false;
	}

	// validate the URI syntax
	String::size_type pos0 = m_URI.find( "://" );
	if ( String::npos == pos0 ) {
		clear();
		return false;
	}

	// extract the schema type
	m_Scheme = m_URI.substr( 0, pos0 );
	String::size_type pos1 = m_URI.rfind( "/" );
	if ( String::npos == pos1 ) {
		return false;
	}

	// get the length
	const ui32 resLen = m_URI.size() - pos1;
	const ui32 len    = m_URI.size() - pos0 - resLen - 2;
	
	// get the components
	m_Path     = m_URI.substr( pos0 + 3, len );
	m_Resource = m_URI.substr( pos1 + 1, m_URI.size() - pos1 );
	m_AbsPath  = m_Path + m_Resource;

	return true;
}

//-------------------------------------------------------------------------------------------------
bool Uri::isValid() const {
	return !m_AbsPath.empty();
}

//-------------------------------------------------------------------------------------------------
void Uri::clear() {
	m_URI.clear();
	m_Scheme.clear();
	m_Path.clear();
	m_AbsPath.clear();
	m_Resource.clear();
}

//-------------------------------------------------------------------------------------------------
Uri &Uri::operator = ( const Uri &rhs ) {
	if ( !( *this == rhs ) ) {
		m_URI      = rhs.m_URI;
		m_Scheme   = rhs.m_Scheme;
		m_Path     = rhs.m_Path;
		m_AbsPath  = rhs.m_AbsPath;
		m_Resource = rhs.m_Resource;
	}

	return *this;
}

//-------------------------------------------------------------------------------------------------
bool Uri::operator == ( const Uri &rhs ) const {
	return (m_URI      == rhs.m_URI && 
			m_Scheme   == rhs.m_Scheme && 
			m_Path     == rhs.m_Path && 
			m_AbsPath  == rhs.m_AbsPath && 
			m_Resource == rhs.m_Resource );
}

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
