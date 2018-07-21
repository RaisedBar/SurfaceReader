
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0347 */
/* at Thu Dec 01 10:33:05 2005
 */
/* Compiler settings for gwspeak.idl:
    Os, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __gwspeak_h__
#define __gwspeak_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISpeak_FWD_DEFINED__
#define __ISpeak_FWD_DEFINED__
typedef interface ISpeak ISpeak;
#endif 	/* __ISpeak_FWD_DEFINED__ */


#ifndef __Speak_FWD_DEFINED__
#define __Speak_FWD_DEFINED__

#ifdef __cplusplus
typedef class Speak Speak;
#else
typedef struct Speak Speak;
#endif /* __cplusplus */

#endif 	/* __Speak_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __ISpeak_INTERFACE_DEFINED__
#define __ISpeak_INTERFACE_DEFINED__

/* interface ISpeak */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISpeak;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A42771AC-03C7-11D3-9F6E-00C095EE683F")
    ISpeak : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SpeakString( 
            BSTR String) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Silence( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpeakVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISpeak * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISpeak * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISpeak * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISpeak * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISpeak * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISpeak * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISpeak * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SpeakString )( 
            ISpeak * This,
            BSTR String);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Silence )( 
            ISpeak * This);
        
        END_INTERFACE
    } ISpeakVtbl;

    interface ISpeak
    {
        CONST_VTBL struct ISpeakVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpeak_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpeak_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpeak_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpeak_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISpeak_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISpeak_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISpeak_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISpeak_SpeakString(This,String)	\
    (This)->lpVtbl -> SpeakString(This,String)

#define ISpeak_Silence(This)	\
    (This)->lpVtbl -> Silence(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISpeak_SpeakString_Proxy( 
    ISpeak * This,
    BSTR String);


void __RPC_STUB ISpeak_SpeakString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISpeak_Silence_Proxy( 
    ISpeak * This);


void __RPC_STUB ISpeak_Silence_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpeak_INTERFACE_DEFINED__ */



#ifndef __GWSPEAKLib_LIBRARY_DEFINED__
#define __GWSPEAKLib_LIBRARY_DEFINED__

/* library GWSPEAKLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_GWSPEAKLib;

EXTERN_C const CLSID CLSID_Speak;

#ifdef __cplusplus

class DECLSPEC_UUID("A42771AD-03C7-11D3-9F6E-00C095EE683F")
Speak;
#endif
#endif /* __GWSPEAKLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


