#pragma once

#include <cassert>

#ifdef _DEBUG
#  define osre_validate( statement, msg ) assert( statement )
#else
#  define osre_validate( (statement), (msg) ) 
#endif

