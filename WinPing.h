
// WinPing.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

HINSTANCE GetGlobalInstance();
// CWinPingApp:
// See WinPing.cpp for the implementation of this class
//

class CWinPingApp : public CWinAppEx
{
public:
	CWinPingApp();

// Overrides
	public:
	virtual BOOL InitInstance();

protected:
	BOOL InitSocket();
	void UninitSocket();
// Implementation

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CWinPingApp theApp;