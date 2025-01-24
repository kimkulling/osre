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
#include "Common/ArgumentParser.h"
#include "Common/Logger.h"
#include "Common/Tokenizer.h"

namespace OSRE {
namespace Common {

using namespace ::cppcore;

static constexpr c8 option[] = "--";
static constexpr c8 tokenArgIn = '<';
static constexpr c8 tokenArgOut = '>';

ArgumentParser::Argument::Argument() :
        mArgument(""), mDesc(""), mNumArgs(0) {
    // empty
}

ArgumentParser::Argument::Argument(const String &arg, const String &desc, ui32 numArgs) :
        mArgument(arg), mDesc(desc), mNumArgs(numArgs) {
    // empty
}

static bool parseExpectedArgs(const String &userDefinedArgs, const String &desc, 
        TArray<ArgumentParser::Argument> &supportedArgs, String &error) {
    error.clear();
    StringArray extractedArgs, extractedDescs;
    if (extractedDescs.size() != extractedArgs.size()) {
        error += "Missing description for arguments";
        return false;
    }

    Tokenizer::tokenize(userDefinedArgs, extractedArgs, ":");
    Tokenizer::tokenize(desc, extractedDescs, ":");
    ui32 numParam(0);
    for (ui32 i = 0; i < extractedArgs.size(); ++i) {
        String arg = extractedArgs[i];
        String currentDesc = extractedDescs[i];
        if (ArgumentParser::parseArgParameter(arg, numParam)) {
            supportedArgs.add(ArgumentParser::Argument(ArgumentParser::getBlankArgument(arg), 
                currentDesc, numParam));
        }
    }

    return true;
}

ArgumentParser::ArgumentParser(i32 argc, const c8 *ppArgv[], const String &supportedArgs, const String &desc) :
        mSupportedArguments(), mStoredArguments(), mCurrentIndex(0), mIsValid(true) {
    // Parse and store the expected arguments
    const size_t optionLen = ::strlen(option);
    String error;
    if (!parseExpectedArgs(supportedArgs, desc, mSupportedArguments, error)) {
        return;
    }

    if (argc > 1) {
        while (mCurrentIndex < static_cast<ui32>(argc)) {
            String argument(ppArgv[mCurrentIndex]);
            const String::size_type pos = argument.find(option);
            if (String::npos != pos) {
                argument = argument.substr(pos + optionLen, argument.size() - (pos + optionLen));
                if (isSupported(argument)) {
                    const ui32 numValues = getNumValues(argument);
                    if (numValues) {
                        // Check the number of expected values for being a new option
                        for (ui32 valIdx = mCurrentIndex + 1; valIdx < mCurrentIndex + numValues + 1; valIdx++) {
                            String tmpVal(ppArgv[valIdx]);
                            String::size_type pos1 = tmpVal.find(option);
                            if (pos1 != String::npos) {
                                setInvalid();
                                break;
                            }
                        }
                    }

                    // Store the data if its valid
                    if (mIsValid) {
                        if ((mCurrentIndex + 1) < static_cast<ui32>(argc)) {
                            ++mCurrentIndex;
                            mDetectedArgs.add(argument);
                            String content(ppArgv[mCurrentIndex]);
                            mStoredArguments.add(content);
                        } else {
                            mDetectedArgs.add(argument);
                            mStoredArguments.add("");
                        }
                    }
                } else {
                    setInvalid();
                }
            }
            ++mCurrentIndex;
        }
    }

    // validate incoming arguments
    if (!validateArguments(argc, ppArgv)) {
        setInvalid();
    }
    mCurrentIndex = 0;
}

String ArgumentParser::showHelp() const {
    if (mSupportedArguments.isEmpty()) {
        return String("");
    }

    String helpMsg;
    for (ui32 i = 0; i < mSupportedArguments.size(); i++) {
        Argument argument = mSupportedArguments[i];
        const String arg = argument.mArgument;
        const String desc = argument.mDesc;
        const ui32 numargs = argument.mNumArgs;

        helpMsg += "\t: ";
        helpMsg += arg; 
        helpMsg += "\t: ";
        helpMsg += desc;
        helpMsg += "\n";
        if (numargs > 0) {
            helpMsg += "\n\tNumber of arguments: ";
            c8 buffer[512];
            ::memset(buffer, '\0', 512);
            ::sprintf(buffer, "%d\n", numargs);
            helpMsg += String(buffer);
        }
    }

    return helpMsg;
}

bool ArgumentParser::getNext(String &arg, String &value) {
    if (mSupportedArguments.isEmpty() || mStoredArguments.isEmpty() ||
            mCurrentIndex >= mSupportedArguments.size()) {
        return false;
    }

    arg = mSupportedArguments[mCurrentIndex].mArgument;
    value = mStoredArguments[mCurrentIndex];
    ++mCurrentIndex;

    return true;
}

void ArgumentParser::clear() {
    mSupportedArguments.clear();
    mDetectedArgs.clear();
    mStoredArguments.clear();
    mCurrentIndex = 0;
    mIsValid = true;
}

bool ArgumentParser::isSupported(const String &arg) const {
    bool res(false);
    for (ui32 index = 0; index < mSupportedArguments.size(); ++index) {
        if (arg == mSupportedArguments[index].mArgument) {
            res = true;
            break;
        }
    }

    return res;
}

ui32 ArgumentParser::getNumValues(const String &argument) const {
    if (argument.empty()) {
        return 0;
    }

    ui32 numValues(0);
    for (ui32 index = 0; index < mSupportedArguments.size(); ++index) {
        if (mSupportedArguments[index].mArgument == argument) {
            numValues = mSupportedArguments[index].mNumArgs;
            break;
        }
    }

    return numValues;
}

const String &ArgumentParser::getArgument(const String &arg) const {
    static String Dummy;
    if (arg.empty()) {
        return Dummy;
    }

    for (ui32 index = 0; index < mDetectedArgs.size(); ++index) {
        if (mDetectedArgs[index] == arg) {
            return mStoredArguments[index];
        }
    }

    return Dummy;
}

bool ArgumentParser::hasArgument(const String &argument) const {
    bool result(false);
    for (ui32 idx = 0; idx < mDetectedArgs.size(); ++idx) {
        if (mDetectedArgs[idx] == argument) {
            result = true;
            break;
        }
    }
    return result;
}

bool ArgumentParser::hasValidArgs() const {
    return mIsValid;
}

bool ArgumentParser::parseArgParameter(const String &argument, ui32 &numPara) {
    if (argument.empty()) {
        return true;
    }

    String paraStr("");
    bool paraDefOpen(false);
    for (ui32 i = 0; i < argument.size(); i++) {
        if (argument[i] == tokenArgIn) {
            if (paraDefOpen) {
                numPara = 0;
                return false;
            } else {
                paraDefOpen = true;
            }
        } else if (argument[i] == tokenArgOut) {
            if (!paraDefOpen) {
                numPara = 0;
                return false;
            } else {
                paraDefOpen = false;
            }

        } else if (paraDefOpen) {
            paraStr += argument[i];
        }
    }

    if (!paraDefOpen) {
        numPara = atoi(paraStr.c_str());
    } else {
        return false;
    }

    return true;
}

String ArgumentParser::getBlankArgument(const String &argument) {
    String blankArg("");

    // Empty, return it
    if (argument.empty()) {
        return blankArg;
    }

    // Check if any token for parameter definition is there
    String::size_type pos = argument.find(tokenArgIn);
    if (String::npos == pos) {
        return argument;
    }

    // Clean from parameter-specific token
    ui32 i(0);
    while (argument[i] != tokenArgIn) {
        blankArg += argument[i];
        ++i;
    }

    return blankArg;
}

bool ArgumentParser::validateArguments(i32 argc, const c8 *ppArgv[]) {
    bool valid(true);
    for (ui32 i = 1; i < static_cast<ui32>(argc); ++i) {
        String incomingArg(ppArgv[i]);
        String::size_type pos = incomingArg.find("--");
        if (String::npos != pos) {
            bool supported(false);
            for (ui32 j = 0; j < mSupportedArguments.size(); ++j) {
                const String searchFor("--" + mSupportedArguments[j].mArgument);
                if (incomingArg == searchFor) {
                    supported = true;
                    break;
                }
            }
            if (!supported) {
                mLastError = "Invalid argument: " + incomingArg;
                valid = false;
                break;
            }
        }
    }

    return valid;
}

void ArgumentParser::setInvalid() {
    mIsValid = false;
}

} // Namespace Common
} // Namespace OSRE
