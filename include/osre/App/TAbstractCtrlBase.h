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

#include <osre/Common/osre_common.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace App {

template<class T>
class TAbstractCtrlBase;

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	The abstract base class for application controller. 
//--------------------------------------------------------------------------------------------------------------------
template<class TEnum>
class TAbstractCtrlStateListener {
public:
    virtual ~TAbstractCtrlStateListener();
    virtual void onStateChanged( TEnum newState ) = 0;

protected:
    TAbstractCtrlStateListener( TAbstractCtrlBase<TEnum> *ctrl );

private:
    TAbstractCtrlBase<TEnum> *m_instance;
};

template<class TEnum>
inline
TAbstractCtrlStateListener<TEnum>::TAbstractCtrlStateListener( TAbstractCtrlBase<TEnum> *ctrl )
: m_instance( ctrl ) {
    // empty
}

template<class TEnum>
inline
    TAbstractCtrlStateListener<TEnum>::~TAbstractCtrlStateListener() {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	The abstract base class for application controller. 
//--------------------------------------------------------------------------------------------------------------------
template<class T>
class TAbstractCtrlBase {
public:
    virtual ~TAbstractCtrlBase();
    bool registerListener( TAbstractCtrlStateListener<T> *listener );
    bool unregisterListener( TAbstractCtrlStateListener<T> *listener );
    bool gotoState( T newState );
    T getState() const;
    bool update( d32 timetick );

protected:
    TAbstractCtrlBase( T initialState );
    void notifyListener();
    virtual bool onStateEnter( T newState );
    virtual bool onState() = 0;
    virtual bool onUpdate( d32 timetick );
    virtual bool onStateLeave( T oldState );

private:
    typedef typename CPPCore::TArray<TAbstractCtrlStateListener<T>* >::Iterator ListenerIt;
    CPPCore::TArray<TAbstractCtrlStateListener<T>* > m_listener;
    T m_state;
};

template<class T>
inline
TAbstractCtrlBase<T>::TAbstractCtrlBase( T initialState )
: m_listener()
, m_state( initialState ) {
    // empty
}

template<class T>
inline
TAbstractCtrlBase<T>::~TAbstractCtrlBase() {
    // empty
}

template<class T>
inline
bool TAbstractCtrlBase<T>::registerListener( TAbstractCtrlStateListener<T> *listener ) {
    if ( nullptr == listener ) {
        return false;
    }

    m_listener.add( listener );

    return true;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::unregisterListener( TAbstractCtrlStateListener<T> *listener ) {
    if ( nullptr == listener ) {
        return false;
    }

    ListenerIt it = m_listener.find( listener );
    if ( it == m_listener.end() ) {
        return false;
    }

    m_listener.remove( it );

    return true;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::gotoState( T newState ) {
    if ( !onStateLeave( m_state ) ) {
        return false;
    }
    if ( !onStateEnter( newState ) ) {
        return false;
    }

    m_state = newState;
    if ( !onState() ) {
        return false;
    }

    notifyListener();

    return true;
}

template<class T>
inline
T TAbstractCtrlBase<T>::getState() const {
    return m_state;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::update( d32 timetick ) {
    if ( onState() ) {
        return onUpdate( timetick );
    }
    return false;
}

template<class T>
inline
void TAbstractCtrlBase<T>::notifyListener() {
for ( ui32 i = 0; i < m_listener.size(); i++ ) {
        if ( nullptr != m_listener[ i ] ) {
            m_listener[ i ]->onStateChanged( m_state );
        }
    }
}

template<class T>
inline
bool TAbstractCtrlBase<T>::onStateEnter( T ) {
    return true;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::onUpdate( d32 ) {
    return true;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::onStateLeave( T ) {
    return true;
}

} // Namespace App
} // Namespace OSRE
