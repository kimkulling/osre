#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace RenderBackend {

enum ParameterType {
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

    void *getData() const;
    static ParamDataBlob *create( ParameterType type, ui32 arraySize );
};

struct OSRE_EXPORT Parameter {
    String     m_name;
    ParameterType m_type;
    ui32          m_numItems;
    ParamDataBlob m_data;
    Parameter    *m_next;

    Parameter();
    ~Parameter();

    static ui32 getParamDataSize( ParameterType type, ui32 arraySize );
    static Parameter *create( const String &name, ParameterType type, ui32 arraySize=1 );
};

} // Namespace RenderBackend
} // Namespace OSRE
