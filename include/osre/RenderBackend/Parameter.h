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
#pragma once

#include <osre/Common/osre_common.h>
#include <cppcore/Container/THashMap.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

class RenderBackendService;

enum class ParameterType {
    PT_None,
    PT_Int,
    PT_Float,
    PT_Float2,
    PT_Float3,
    PT_Mat4, 
    PT_Mat4Array
};

struct OSRE_EXPORT ParamDataBlob {
    void *m_data;
    ui32  m_size;

    ParamDataBlob();
    ~ParamDataBlob();
    void *getData() const;
    void clear();
    static ParamDataBlob *create( ParameterType type, ui32 arraySize );
};

struct OSRE_EXPORT Parameter {
    String         m_name;
    ParameterType  m_type;
    ui32           m_numItems;
    ParamDataBlob  m_data;
    Parameter     *m_next;

    static ui32 getParamDataSize( ParameterType type, ui32 arraySize );
    static Parameter *create( const String &name, ParameterType type, ui32 arraySize=1 );
    static void destroy( Parameter *param );

private:
    Parameter();
    ~Parameter();
};

struct OSRE_EXPORT ParameterRegistry {
    static ParameterRegistry *create( RenderBackendService *rbSrv );
    static void destroy();
    static bool registerParameter( Parameter *param );
    static Parameter *getParameterByName( const String & name );
    static bool updateParameter( Parameter *param );
    static bool commitChanges();

private:
    ParameterRegistry( RenderBackendService *rbSrv );
    ~ParameterRegistry();

private:
    static ParameterRegistry *s_instance;
    
    CPPCore::THashMap<ui32, Parameter*> m_parameterMap;
    CPPCore::TArray<Parameter*> m_updates;
    RenderBackendService *m_rbService;
};
} // Namespace RenderBackend
} // Namespace OSRE
