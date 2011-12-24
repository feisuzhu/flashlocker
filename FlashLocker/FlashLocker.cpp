// FlashLocker.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FlashLocker.h"
#include "FlashLockerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlashLockerApp

BEGIN_MESSAGE_MAP(CFlashLockerApp, CWinApp)
	//{{AFX_MSG_MAP(CFlashLockerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashLockerApp construction

CFlashLockerApp::CFlashLockerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFlashLockerApp object

CFlashLockerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFlashLockerApp initialization

BOOL CFlashLockerApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFlashLockerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

long GetFlashDiskInfo(char drvletter,KEYSTRUCT* ks)
{
	char buf[512];
	char drvpath[]="A:\\";
	char drvslink[]="\\\\.\\A:";
	long space[2];
 	drvpath[0]=drvletter;
	drvslink[4]=drvletter;
	ULONG BytesRead;
	HANDLE hdrv;
	if(FAILED(hdrv=CreateFile(drvslink,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL))) return 0;
	if(!GetDiskFreeSpaceEx(drvpath,0,(ULARGE_INTEGER*)&space,0)) return 0;
	SetFilePointer(hdrv,space[0]-512,&space[1],FILE_BEGIN);
	ReadFile(hdrv,&buf,512,&BytesRead,NULL);
	CloseHandle(hdrv);
	memmove(ks,buf+512-sizeof(KEYSTRUCT),sizeof(KEYSTRUCT));
	return 1;
}

long SetFlashDiskInfo(char drvletter,KEYSTRUCT* ks)
{
	char buf[512];
	char drvpath[]="A:\\";
	char drvslink[]="\\\\.\\A:";
	long space[2];
	drvpath[0]=drvletter;
	drvslink[4]=drvletter;
	ks->Signature='KCOL';
	ULONG BytesRead;
	HANDLE hdrv;
	if(FAILED(hdrv=CreateFile(drvslink,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL))) return 0;
	if(!GetDiskFreeSpaceEx(drvpath,0,(ULARGE_INTEGER*)&space,0)) return 0;
	SetFilePointer(hdrv,space[0]-512,&space[1],FILE_BEGIN);
	ReadFile(hdrv,&buf,512,&BytesRead,NULL);
	memmove(buf+512-sizeof(KEYSTRUCT),ks,sizeof(KEYSTRUCT));
	SetFilePointer(hdrv,space[0]-512,&space[1],FILE_BEGIN);
	WriteFile(hdrv,&buf,512,&BytesRead,NULL);
	CloseHandle(hdrv);
	return 1;
}

