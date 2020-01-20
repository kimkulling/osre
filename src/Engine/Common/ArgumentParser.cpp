/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
static const c8 tokenArgIn  = '<';
static const c8 tokenArgOut = '>';
static const String dummy   = "";

ArgumentParser::Argument::Argument()
: m_argument( "" )
, m_desc( "" )
, m_numArgs( 0 ) {
    // empty
}

ArgumentParser::Argument::Argument( const String &arg, const String &desc, ui32 numArgs )
: m_argument( arg )
, m_desc( desc )
, m_numArgs( numArgs ) {
    // empty
}

static void parseExpectedArgs( const String &userDefinedArgs, const String &desc, TArray<ArgumentParser::Argument> &supportedArgs ) {
    TArray<String> extractedArgs, extractedDescs;
    Tokenizer::tokenize( userDefinedArgs, extractedArgs, ":" );
    Tokenizer::tokenize( desc, extractedDescs, ":" );
    ui32 numParam( 0 );
    for ( ui32 i = 0; i < extractedArgs.size(); ++i ) {
        String arg = extractedArgs[ i ];
        String currentDesc = extractedDescs[ i ];
        if ( ArgumentParser::parseArgParameter( arg, numParam ) ) {
            supportedArgs.add( ArgumentParser::Argument( ArgumentParser::getBlankArgument( arg ), currentDesc, numParam ) );
        }
    }
}

ArgumentParser::ArgumentParser( i32 argc, const c8 *ppArgv[], const String &supportedArgs, const String &desc )
: m_SupportedArguments()
, m_StoredArguments()
, m_CurrentIndex( 0 )
, m_isValid( true ) {
    // Parse and store the expected arguments
    const size_t optionLen = static_cast<size_t>( option.size() );
    parseExpectedArgs( supportedArgs, desc, m_SupportedArguments );
    if ( argc > 1 )	{
        while ( m_CurrentIndex < static_cast<ui32>( argc ) ) {
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

    // validate incoming arguments
    if ( !validateArguments( argc, ppArgv ) ) {
        setInvalid();
    }
    m_CurrentIndex = 0;
}

ArgumentParser::~ArgumentParser() {
    // empty
}

String ArgumentParser::showHelp() const {
    if ( m_SupportedArguments.isEmpty() ) {
        return String( "" );
    }

    String helpMsg;
    for ( ui32 i = 0; i < m_SupportedArguments.size(); i++ ) {
        Argument argument = m_SupportedArguments[ i ];
        const String arg = argument.m_argument;
        const String desc = argument.m_desc;
        const ui32 numargs = argument.m_numArgs;

        helpMsg += arg + "\t:";
        helpMsg += desc;
        if ( numargs > 0 ) {
            helpMsg += "\n\tNumber of arguments: ";
            c8 buffer[ 512 ];
            ::memset( buffer, '\0', 512 );
            sprintf( buffer, "%d", numargs );
            helpMsg += String( buffer );
        }
    }

    return helpMsg;
}

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

void ArgumentParser::clear() {
    m_SupportedArguments.clear();
    m_detectedArgs.clear();
    m_StoredArguments.clear();
    m_CurrentIndex = 0;
    m_isValid = true;
}

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

ui32 ArgumentParser::getNumValues( const String &argument ) const {
    if ( argument.empty() ) {
        return 0;
    }

    ui32 numValues( 0 );
    for ( ui32 index = 0; index < m_SupportedArguments.size(); ++index ) {
        if ( m_SupportedArguments[ index ].m_argument == argument ) {
            numValues = m_SupportedArguments[ index ].m_numArgs;
            break;
        }
    }
    
    return numValues;
}

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

bool ArgumentParser::hasArgument( const String &argument ) const {
    bool result( false );
    for ( ui32 idx=0; idx<m_detectedArgs.size(); ++idx ) {
        if ( m_detectedArgs[ idx ] == argument ) {
            if ( !m_StoredArguments[ idx ].empty() )  {
                result = true;
            }
            break;
        }
    }
    return result;
}

bool ArgumentParser::hasValidArgs() const {
    return m_isValid;
}

bool ArgumentParser::parseArgParameter( const String &argument, ui32 &numPara ) {
    if ( argument.empty() ) {
        return true;
    }

    String paraStr( "" );
    bool paraDefOpen( false );
    for ( ui32 i=0; i<argument.size(); i++ ) {
        if ( argument[ i ] == tokenArgIn ) {
            if ( paraDefOpen ) {
                numPara = 0;
                return false;
            } else {
                paraDefOpen = true;
            }
        } else if ( argument[ i ] == tokenArgOut ) {
            if ( !paraDefOpen ) {
                numPara = 0;
                return false;
            } else {
                paraDefOpen = false;
            }

        } else if ( paraDefOpen ) {
            paraStr += argument[ i ];
        }
    }

    if ( !paraDefOpen ) {
        numPara = atoi( paraStr.c_str() );
    } else {
        return false;
    }

    return true;
}

String ArgumentParser::getBlankArgument( const String &argument ) {
    String blankArg( "" );

    // Empty, return it
    if ( argument.empty() ) {
        return blankArg;
    }

    // Check if any token for parameter definition is there
    String::size_type pos = argument.find( tokenArgIn );
    if( String::npos == pos ) {
        return argument;
    }

    // Clean from parameter-specific token
    ui32 i( 0 );
    while ( argument[ i ] != tokenArgIn ) {
        blankArg += argument[ i ];
        ++i;
    }

    return blankArg;
}

bool ArgumentParser::validateArguments( i32 argc, const c8 *ppArgv[] ) {
    bool valid( true );
    for ( ui32 i = 1; i < static_cast< ui32 >( argc ); ++i ) {
        String incomingArg( ppArgv[ i ] );
        String::size_type pos = incomingArg.find( "--" );
        if ( String::npos != pos ) {
            bool supported( false );
            for ( ui32 j = 0; j < m_SupportedArguments.size(); ++j ) {
                const String searchFor( "--" + m_SupportedArguments[ j ].m_argument );
                if ( incomingArg == searchFor ) {
                    supported = true;
                    break;
                }
            }
            if ( !supported ) {
                valid = false;
                break;
            }
        }
    }

    return valid;
}

void ArgumentParser::setInvalid() {
    m_isValid = false;
}

} // Namespace Common
} // Namespace OSRE
