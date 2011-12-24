// FlashLocker.h : main header file for the FLASHLOCKER application
//

#if !defined(AFX_FLASHLOCKER_H__6C5ADA46_9CF8_47B3_808B_28E6A9F512CF__INCLUDED_)
#define AFX_FLASHLOCKER_H__6C5ADA46_9CF8_47B3_808B_28E6A9F512CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/*
overlay definition:

<ENCRYPTED>
PRGINFO
SECTION_INFO[nSecs]
BYTE[#^@$%@$] data 
<QWORD aligned>
*/


typedef struct _SECTION_INFO //describes the sections
{
	ULONG RVA;
	ULONG Size;
} SECTION_INFO;

typedef struct _ENCINFO  // build into the loader
{
	ULONG OverlayOffset;
	ULONG OverlaySize;
	ULONG TheSeed;
} ENCINFO;

typedef struct _PRGINFO // begin of the overlay 
{
	ULONG EntryPoint;
	ULONG ImportTable;
	ULONG Sections;
	ULONG IsBoundImport;
} PRGINFO;

typedef struct _KEYSTRUCT // used in the flash disk
{
	ULONG Signature;
	ULONG Seed;
	ULONG TheKey[4];
} KEYSTRUCT;

long GetFlashDiskInfo(char drvletter,KEYSTRUCT* ks);
long SetFlashDiskInfo(char drvletter,KEYSTRUCT* ks);



/////////////////////////////////////////////////////////////////////////////
// CFlashLockerApp:
// See FlashLocker.cpp for the implementation of this class
//

class CFlashLockerApp : public CWinApp
{
public:
	CFlashLockerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashLockerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFlashLockerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHLOCKER_H__6C5ADA46_9CF8_47B3_808B_28E6A9F512CF__INCLUDED_)
