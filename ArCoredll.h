/*++

Copyright(C) Zhiyuan Electronics Corporation. All rights reserved

Module Name:

    ArCoreDll.h

Abstract:

	the header file for the library of the driver for EPCM-505C-ESDC Board's ArriaII Core Chipset.

Author:

	HuaQiyan  2012-01-10
     
Environment:

	User mode

Notes:

	Support Windows XP and the later platform.

Revision History:


--*/

#pragma once


// device open operation
DWORD _internalOpenDev(
    IN LPGUID pDevInterface,
	IN int DevIndex,
	OUT HANDLE *phDev
	);

DWORD _internalGetDevName(
    IN LPGUID pDevInterface,
    IN int m_devIndex,
    OUT void* lpszName, 
    OUT DWORD* pdwRequiredSize
    );