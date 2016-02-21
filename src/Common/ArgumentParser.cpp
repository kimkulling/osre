/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/ArgumentParser.h>
#include <osre/Common/Tokenizer.h>
#include <osre/Common/Logger.h>

namespace OSRE {
namespace Common {

using namespace ::CPPCore;

static const String option = "--";
static const c8 tokenArgIn    = '<';
static const c8 tokenArgOut   = '>';
static const String dummy = "";

//--------------------------------------------------------------------------------------------------------------------
ArgumentParser::Argument::Argument()
: m_argument( "" )
, m_desc( "" )
, m_numArgs( 0 ) {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
ArgumentParser::Argument::Argument( const String &arg, const String &desc, ui32 numArgs )
: m_argument( arg )
, m_desc( desc )
, m_numArgs( numArgs ) {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
ArgumentParser::ArgumentParser( i32 argc, c8 *ppArgv[], const String &supportedArgs, const String &desc )
: m_SupportedArguments()
, m_StoredArguments()
, m_CurrentIndex( 0 )
, m_isValid( true ) {
    // Parse and store the expected arguments
    const ui32 optionLen = option.size();
    TArray<String> extractedArgs, extractedDescs;
    Tokenizer::tokenize( supportedArgs, extractedArgs, ":" );
    Tokenizer::tokenize( desc, extractedDescs, ":" );
    ui32 numParam( 0 );
    ui32 expectedArgc( 0 );
    for( ui32 i = 0; i<extractedArgs.size(); ++i ) {
        String arg = extractedArgs[ i ];
        String currentDesc = extractedDescs[ i ];
        if ( parseArgParameter( arg, numParam ) ) {
            expectedArgc += ( 1 + numParam );
            m_SupportedArguments.add( Argument( getBlankArgument( arg ), currentDesc, numParam ) );
        }
    }

    // The number of arguments is right, parse the arguments
    if ( expectedArgc <= static_cast<ui32>( argc-1 ) ) {
        if ( argc > 1 )	{
            while ( m_CurrentIndex < static_cast<ui32>( argc - 1 ) ) {
                String argument( ppArgv[ m_CurrentIndex ] );
                const String::size_type pos = argument.find( option );
                if( String::npos != pos ) {
                    argument = argument.substr( pos+optionLen, argument.size() - (pos+optionLen) );
                    if ( isSupported( argument ) ) {
                        const ui32 numValues = getNumValues( argument );
                        if ( numValues ) {
                            // Check the number of expected values for being a new option
                            for ( ui32 valIdx=m_CurrentIndex+1; valIdx<m_CurrentIndex+numValues+1; valIdx++ ) {
                                String tmpVal( ppArgv[ valIdx ] );
                                String::size_type pos1 = tmpVal.find( option );
                                if( pos1 != String::npos ) {
                                    setInvalid();
                                    break;
                                }
                            }
                        }

                        // Store the data if its valid
                        if ( m_isValid ) {
                            if ( ( m_CurrentIndex + 1 ) < static_cast<ui32>( argc ) ) {
                                ++m_CurrentIndex;
                                m_detectedArgs.add( argument );
                                String content( ppArgv[ m_CurrentIndex ] );
                                m_StoredArguments.add( content );
                            }
                        } 
                    } else {
                        setInvalid();
                    }
                }
                ++m_CurrentIndex;
            }
        }
    } else {
        setInvalid();
    }

    // validate incoming arguments
    for ( ui32 i=1; i<static_cast<ui32>( argc ); ++i ) {
        String incomingArg( ppArgv[ i ] );
        String::size_type pos = incomingArg.find( "--" );
        if( String::npos != pos ) {
            bool supported( false );
            for ( ui32 j=0; j<m_SupportedArguments.size(); ++j ) {
                const String searchFor( "--" + m_SupportedArguments[ j ].m_argument );
                if ( incomingArg == searchFor ) {
                    supported = true;
                    break;
                }
            }
            if ( !supported ) {
                setInvalid();
                break;
            }
        }
    }
    m_CurrentIndex = 0;
}

//--------------------------------------------------------------------------------------------------------------------
ArgumentParser::~ArgumentParser() {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
bool ArgumentParser::getNext( String &arg, String &value ) {
    if ( m_SupportedArguments.isEmpty() || m_StoredArguments.isEmpty() || 
        m_CurrentIndex >= m_SupportedArguments.size() ) {
        return false;
    }

    arg = m_SupportedArguments[ m_CurrentIndex ].m_argument;
    value = m_StoredArguments[ m_CurrentIndex ];
    ++m_CurrentIndex;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
void ArgumentParser::clear() {
    m_SupportedArguments.clear();
    m_detectedArgs.clear();
    m_StoredArguments.clear();
    m_CurrentIndex = 0;
    m_isValid = true;
}

//--------------------------------------------------------------------------------------------------------------------
bool ArgumentParser::isSupported( const String &arg ) const {
    bool res( false );
    for ( ui32 index = 0; index < m_SupportedArguments.size(); ++index ) {
        if ( arg == m_SupportedArguments[ index ].m_argument ) {
            res = true;
            break;
        }
    }
    
    return res;
}

//--------------------------------------------------------------------------------------------------------------------
ui32 ArgumentParser::getNumValues( const String &rArgument ) const {
    if ( rArgument.empty() ) {
        return 0;
    }

    ui32 numValues( 0 );
    for ( ui32 index = 0; index < m_SupportedArguments.size(); ++index ) {
        if ( m_SupportedArguments[ index ].m_argument == rArgument ) {
            numValues = m_SupportedArguments[ index ].m_numArgs;
            break;
        }
    }
    
    return numValues;
}

//--------------------------------------------------------------------------------------------------------------------
const String &ArgumentParser::getArgument( const String &arg ) const {
    if ( arg.empty() ) {
        return dummy;
    }

    for ( ui32 index=0; index < m_detectedArgs.size(); ++index ) {
        if ( m_detectedArgs[ index ] == arg ) {
            return m_StoredArguments[ index ];
        }
    }
    
    return dummy;
}

//--------------------------------------------------------------------------------------------------------------------
bool ArgumentParser::hasArgument( const String &arg ) const {
    bool result( false );
    for ( ui32 idx=0; idx<m_detectedArgs.size(); ++idx ) {
        if ( m_detectedArgs[ idx ] == arg ) {
            if ( !m_StoredArguments[ idx ].empty() )  {
                result = true;
            }
            break;
        }
    }
    return result;
}

//--------------------------------------------------------------------------------------------------------------------
bool ArgumentParser::hasValidArgs() const {
    return m_isValid;
}

//--------------------------------------------------------------------------------------------------------------------
bool ArgumentParser::parseArgParameter( const String &arg, ui32 &numPara ) {
    if ( arg.empty() ) {
        return true;
    }

    String paraStr( "" );
    bool paraDefOpen( false );
    for ( ui32 i=0; i<arg.size(); i++ ) {
        if ( arg[ i ] == tokenArgIn ) {
            if ( paraDefOpen ) {
                numPara = 0;
                return false;
            } else {
                paraDefOpen = true;
            }
        } else if ( arg[ i ] == tokenArgOut ) {
            if ( !paraDefOpen ) {
                numPara = 0;
                return false;
            } else {
                paraDefOpen = false;
            }

        } else if ( paraDefOpen ) {
            paraStr += arg[ i ];
        }
    }

    if ( !paraDefOpen ) {
        numPara = atoi( paraStr.c_str() );
    } else {
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
String ArgumentParser::getBlankArgument( const String &arg ) {
    String blankArg( "" );

    // Empty, return it
    if ( arg.empty() ) {
        return blankArg;
    }

    // Check if any token for parameter definition is there
    String::size_type pos = arg.find( tokenArgIn );
    if( String::npos == pos ) {
        return arg;
    }

    // Clean from parameter-specific token
    ui32 i( 0 );
    while ( arg[ i ] != tokenArgIn ) {
        blankArg += arg[ i ];
        ++i;
    }

    return blankArg;
}

//--------------------------------------------------------------------------------------------------------------------
void ArgumentParser::setInvalid() {
    m_isValid = false;
}

//--------------------------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace OSRE
