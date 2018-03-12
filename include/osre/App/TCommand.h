#pragma once

#include <osre/Common/TFunctor.h>

namespace OSRE {
    namespace App {
        template<class TParam, class TRet>
        class TCommand {
        public:
            TCommand();
            ~TCommand();
            TRet execute( TParam param );
        };

        template<class TParam, class TRet>
        inline
        TCommand<TParam, TRet>::TCommand() {

        }

        template<class TParam, class TRet>
        inline
        TCommand<TParam, TRet>::~TCommand() {

        }

        template<class TParam, class TRet>
        inline
        TRet TCommand<TParam, TRet>::execute( TParam param ) {

        }

    }
}
