// ZrxProject6.h : main header file for the ZrxProject6 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CZrxProject6App
// See ZrxProject6.cpp for the implementation of this class
//

class CZrxProject6App : public CWinApp
{
public:
	CZrxProject6App();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
