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
    virtual bool onState() = 0;
    bool registerListener( Listener *listener );
    bool unregisterListener( Listener *listener );
    T getState() const;

protected:
    TAbstractCtrlBase();
    void notifyListener();

private:
    typedef typename CPPCore::TArray<typename Listener*>::Iterator ListenerIt;
    CPPCore::TArray<Listener*> m_listener;
    T m_state;
};

template<class T>
inline
TAbstractCtrlBase<T>::TAbstractCtrlBase()
: m_listener()
, m_state( 0 ){
    // 
}

template<class T>
inline
TAbstractCtrlBase<T>::~TAbstractCtrlBase() {
    // 
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
T TAbstractCtrlBase<T>::getState() const {
    return m_state;
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

}
}
