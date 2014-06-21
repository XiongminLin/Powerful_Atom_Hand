/*++

Copyright(C) Zhiyuan Electronics Corporation. All rights reserved

Module Name:

    ArCorelib.h

Abstract:

	The public header file for the library of the driver for EPCM-505C-ESDC Board's ArriaII Core Chipset.

Author:

	HuaQiyan  2012-02-10
     
Environment:

	User mode

Notes:

	Support Windows XP and the later platform.

Revision History:


--*/

#pragma once


//
// Forward Declarations
//

typedef struct IAiObject IAiObject;
typedef struct IAoObject IAoObject;
typedef struct IGpioObject IGpioObject;

//
// The GPIO event message
//
#define GPIO_SWITCH_CHANGED 0x0480
#define GPIO_BUTTON_DOWN    0x0481
#define GPIO_BUTTON_UP      0x0482

//
// The GPIO event callback routine prototype
//
typedef void (__stdcall *GPIO_EVENT_PROC_ROUTINE)(
    int Event,      // Event message
    int wParam,     // Button or Switch status code, depend on the event message type
    int lParam,     // Index of button that's state has changed, or -1 while any button hold on down state, Switch not used.
    void* Context   // The user registered calbback context in IGpioObject::RegisterEventProc()
    );


//
// The AD data convert callback routine prototype
//
typedef void (__stdcall *AD_CONVERT_CALLBACK)(
    DWORD dwRawData,        // AD raw data
    void* Context           // The user registered calbback context in IAiObject::RegisterADCallback()
    );

#ifdef __cplusplus
extern "C"{
#endif 


// interface IAiObject

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    struct IAiObject
    {
        virtual DWORD __stdcall Initialize( 
           IN void *pContext) = 0;
        
        virtual DWORD __stdcall RegisterADCallback( 
            IN AD_CONVERT_CALLBACK pADCallback,
            IN void *pADContext) = 0;

        virtual DWORD __stdcall GetADCInfo(
            IN OUT int *ByteWidth,
            IN OUT int *BitWidth) = 0;

        virtual DWORD __stdcall GetDeviceName(
            IN OUT void* lpszName,
            IN OUT DWORD* pdwReqiredSize) = 0;

        virtual DWORD __stdcall Start() = 0;
        virtual DWORD __stdcall Stop() = 0;
    };
    
#else 	/* C style interface */

    typedef struct IAiObjectVtbl
    {
        DWORD ( __stdcall *Initialize )( 
            IAiObject * This,
            void *pContext);
        
        DWORD (__stdcall *RegisterADCallback)( 
            IAiObject * This,
            AD_CONVERT_CALLBACK pADCallback,
            void *pADContext);

        DWORD (__stdcall *GetADCInfo)(
            IAiObject * This,
            int *ByteWidth,
            int *BitWidth);

        DWORD (__stdcall *GetDeviceName)(
            IAiObject * This,
            void* lpszName,
            DWORD* pdwReqiredSize);

        DWORD (__stdcall *Start)(IAiObject * This);
        DWORD (__stdcall *Stop)(IAiObject * This);

    } IAiObjectVtbl;

    struct IAiObject
    {
        const struct IAiObjectVtbl *lpVtbl;
    };

#ifdef COBJMACROS

#define IAiObject_Initialize(This,pContext)	\
    (This)->lpVtbl -> Initialize(This,pContext)

#define IAiObject_RegisterADCallback(This,pADCallback,pADContext)	\
    (This)->lpVtbl -> RegisterADCallback(This,pADCallback,pADContext)

#define IAiObject_GetADCInfo(This,ByteWidth,BitWidth)	\
    (This)->lpVtbl -> GetADCInfo(This,ByteWidth,BitWidth)

#define IAiObject_GetDeviceName(This,lpszName,pdwRequiredSize)	\
    (This)->lpVtbl -> GetDeviceName(This,lpszName,pdwRequiredSize)

#define IAiObject_Start(This)	\
    (This)->lpVtbl -> Start(This)

#define IAiObject_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */


// interface IAoObject

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    struct IAoObject
    {
        virtual DWORD __stdcall Initialize( 
            IN void *pContext) = 0;
        
        virtual DWORD __stdcall SendAoData( 
            IN USHORT *pTxBuffer,
            IN unsigned long dwBufferSize) = 0;  
    };
    
#else 	/* C style interface */

    typedef struct IAoObjectVtbl
    {
        DWORD ( __stdcall *Initialize )( 
            IAoObject * This,
            void *pContext);
        
        DWORD ( __stdcall *SendAoData )( 
            IAoObject * This,
            USHORT *pTxBuffer,
            unsigned long dwBufferSize);

    } IAoObjectVtbl;

    struct IAoObject
    {
        const struct IAoObjectVtbl *lpVtbl;
    };
 

#ifdef COBJMACROS

#define IAoObject_Initialize(This,pContext)	\
    (This)->lpVtbl -> Initialize(This,pContext)

#define IAoObject_SendAoData(This,pTxBuffer,dwBufferSize)	\
    (This)->lpVtbl -> SendAoData(This,pTxBuffer,dwBufferSize)

#endif /* COBJMACROS */


#endif 	/* C style interface */



// interface IGpioObject

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    struct IGpioObject
    {
        virtual DWORD __stdcall Initialize( 
            void *pContext) = 0;
        
        virtual DWORD __stdcall RegisterEventProc( 
            GPIO_EVENT_PROC_ROUTINE pEventProcRoutine,
            void *pEventProcContext) = 0;

        virtual DWORD __stdcall SetLEDState(DWORD State) = 0;

        virtual DWORD __stdcall GetLEDState(DWORD* pSate) = 0;
    };
    
#else 	/* C style interface */

    typedef struct IGpioObjectVtbl
    {
        DWORD ( __stdcall *Initialize )( 
            IGpioObject * This,
            void *pContext);
        
        DWORD ( __stdcall *RegisterEventProc )( 
            IGpioObject * This,
            GPIO_EVENT_PROC_ROUTINE pEventProcRoutine,
            void *pEventProcContext);

        DWORD ( __stdcall *SetLEDState )( 
            IGpioObject * This,
            DWORD State);

        DWORD ( __stdcall *GetLEDState )( 
            IGpioObject * This,
            DWORD* pSate);
        
    } IGpioObjectVtbl;

    struct IGpioObject
    {
        const struct IGpioObjectVtbl *lpVtbl;
    };


#ifdef COBJMACROS

#define IGpioObject_Initialize(This,pContext)	\
    (This)->lpVtbl -> Initialize(This,pContext)

#define IGpioObject_RegisterEventProc(This,pEventProcRoutine,pEventProcContext)	\
    (This)->lpVtbl -> RegisterEventProc(This,pEventProcRoutine,pEventProcContext)

#define IGpioObject_SetLEDState(This,State)	\
    (This)->lpVtbl -> SetLEDState(This,State)

#define IGpioObject_GetLEDState(This, pState)	\
    (This)->lpVtbl -> GetLEDState(This, pState)

#endif /* COBJMACROS */


#endif 	/* C style interface */


//
// the export routines
//

DWORD __stdcall AiCreate(IAiObject **ppIAiObj, int index);
DWORD __stdcall AoCreate(IAoObject **ppIAoObj, int index);
DWORD __stdcall GpioCreate(IGpioObject **ppIGpioObj, int index);

void __stdcall AiClose(IAiObject *pIAiObj);
void __stdcall AoClose(IAoObject *pIAoObj);
void __stdcall GpioClose(IGpioObject *pIGpioObj);


#ifdef __cplusplus
}
#endif




// the end of this file