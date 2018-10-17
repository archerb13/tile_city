/**
 * \file CityApp.h
 *
 * \author Charles Owen
 *
 * 
 */

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

/**
 * Application class for the City example program
*/
class CCityApp : public CWinApp
{
public:
	CCityApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
    /// GDI+ support variable
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;

    /// GDI+ token
    ULONG_PTR gdiplusToken;
};

extern CCityApp theApp;
