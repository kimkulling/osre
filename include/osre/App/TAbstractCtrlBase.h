#pragma once

#include <osre/Common/osre_common.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace App {

class AbstractCtrlBase;

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	The abstract base class for application controller. 
//--------------------------------------------------------------------------------------------------------------------
template<class TCtrl, class T>
class TAbstractCtrlStateListener {
public:
    virtual ~TAbstractCtrlStateListener();
    virtual void onStateChanged( T newState ) = 0;

protected:
    TAbstractCtrlStateListener( TCtrl *ctrl );

private:
    TCtrl *m_instance;
};

template<class TCtrl, class T>
inline
TAbstractCtrlStateListener<TCtrl, T>::TAbstractCtrlStateListener( TCtrl *ctrl )
: m_instance( ctrl ) {
    // empty
}

template<class TCtrl, class T>
inline
TAbstractCtrlStateListener<TCtrl, T>::~TAbstractCtrlStateListener() {
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
    typedef TAbstractCtrlBase Type;
    typedef TAbstractCtrlStateListener<Type, T> Listener;

    virtual ~TAbstractCtrlBase();
    bool registerListener( Listener *listener );
    bool unregisterListener( Listener *listener );
    bool gotoState( T newState );
    T getState() const;
    bool update( d32 timetick);

protected:
    TAbstractCtrlBase( T initialState );
    void notifyListener();
    virtual bool onStateEnter(); 
    virtual bool onState() = 0;
    virtual bool onUpdate( d32 timetick );
    virtual bool onStateLeave();

private:
    typedef typename CPPCore::TArray<Listener*>::Iterator ListenerIt;
    CPPCore::TArray<Listener*> m_listener;
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
bool TAbstractCtrlBase<T>::registerListener( Listener *listener ) {
    if ( nullptr == listener ) {
        return false;
    }

    m_listener.add( listener );
}

template<class T>
inline
bool TAbstractCtrlBase<T>::unregisterListener( Listener *listener ) {
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
    m_state = newState;
    return onState();
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
            m_listener[ i ]->onStateChanged( m_state() );
        }
    }
}

template<class T>
inline
bool TAbstractCtrlBase<T>::onStateEnter() {
    return true;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::onUpdate( d32 timetick ) {
    return true;
}

template<class T>
inline
bool TAbstractCtrlBase<T>::onStateLeave() {
    return true;
}

}
}