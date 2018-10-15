/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/TFunctor.h>

namespace OSRE {
namespace App {

template<class TParam, class TRet, class TFunc>
class TCommand {
public:
    TCommand();
    explicit TCommand( TParam param, TFunc func );
    ~TCommand();
    TRet execute( TParam param );

private:
    TParam m_param;
    TFunc m_funcBinding;
};

template<class TParam, class TRet, class TFunc>
inline
TCommand<TParam, TRet, TFunc>::TCommand()
: m_param()
, m_funcBinding() {
    // empty
}

template<class TParam, class TRet, class TFunc>
inline
TCommand<TParam, TRet, TFunc>::TCommand(TParam param, TFunc func)
: m_param( param )
, m_funcBinding( func ) {
    // empty
}

template<class TParam, class TRet, class TFunc>
inline
TCommand<TParam, TRet, TFunc>::TCommand( TParam param )
: m_param( param ) {
    // empty
}

template<class TParam, class TRet, class TFunc>
inline
TCommand<TParam, TRet, TFunc>::~TCommand() {

}

template<class TParam, class TRet, class TFunc>
inline
TRet TCommand<TParam, TRet, TFunc>::execute( TParam param ) {

}


}
}
