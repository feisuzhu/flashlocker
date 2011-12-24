// FlashLockerDlg.cpp : implementation file
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
// CAboutDlg dialog used for App About

#define SIZEOFLOADER 0x800

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashLockerDlg dialog

CFlashLockerDlg::CFlashLockerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashLockerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlashLockerDlg)
	m_FileToProtect = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFlashLockerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlashLockerDlg)
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
	DDX_Control(pDX, IDC_UDISKS, m_UDisks);
	DDX_Text(pDX, IDC_FILETOPROTECT, m_FileToProtect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlashLockerDlg, CDialog)
	//{{AFX_MSG_MAP(CFlashLockerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_UDISKS, OnSelchangeUdisks)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_PROTECT, OnProtect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlashLockerDlg message handlers

BOOL CFlashLockerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	RefreshUDiskList();
	m_IsFileValid=0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFlashLockerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFlashLockerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFlashLockerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFlashLockerDlg::OnSelchangeUdisks() 
{
	// TODO: Add your control notification handler code here
	//AddLine(CString::Format(m_UDisks.GetCurSel()
	
}

void CFlashLockerDlg::OnBrowse() 
{
	// TODO: Add your control notification handler code here
	m_Message.Clear();
	UpdateData(FALSE);
	m_IsFileValid=0;
	CFileDialog browser(TRUE);
	browser.m_ofn.lpstrFilter="应用程序(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0\0";
	if(browser.DoModal()==IDOK)
	{
		m_FileToProtect=browser.GetPathName();
		UpdateData(FALSE);
	}
	CFile op;
	ULONG tmp;
	IMAGE_NT_HEADERS nh;
	IMAGE_SECTION_HEADER sh;
	if(!op.Open(m_FileToProtect,CFile::modeReadWrite|CFile::typeBinary))
	{
		AddLine("不能打开文件 "+m_FileToProtect+"!");
		goto hell;
	}
	op.Read(&tmp,2);
	if((tmp&0xffff)!='ZM') {AddLine("该文件不是一个Win32/PE文件！");goto hell;}
	op.Seek(0x3c,CFile::SeekPosition::begin); // IMAGE_DOS_HEADERS.e_lfanew = 3ch
	op.Read(&tmp,4);
	op.Seek(tmp,CFile::SeekPosition::begin);
	op.Read(&nh,sizeof(nh));
	op.Read(&sh,sizeof(sh));
	if(nh.Signature!=IMAGE_NT_SIGNATURE) {AddLine("该文件不是一个Win32/PE文件！");goto hell;} else {AddLine("此文件是Win32/PE文件");}
	if(nh.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress!=0) {AddLine("此程序有TLS表，FlashLocker不支持带有TLS表的程序！");goto hell;}
	if(nh.OptionalHeader.AddressOfEntryPoint>=(sh.VirtualAddress+sh.Misc.VirtualSize)) {AddLine("此程序可能已被其他软件加壳，FlashLocker不支持此类程序！");goto hell;};
	if(nh.FileHeader.Characteristics & IMAGE_FILE_DLL) {AddLine("该文件是一个DLL！FlashLocker不支持DLL文件！");goto hell;}

	m_IsFileValid=TRUE;

hell:;
}

void CFlashLockerDlg::OnProtect() 
{
	// TODO: Add your control notification handler code here
	CFile op,opdst;
	CString s;
	IMAGE_NT_HEADERS *nh;
	IMAGE_SECTION_HEADER *sh,*shbk;
	IMAGE_IMPORT_DESCRIPTOR *iid;
	ULONG i,where,t1,t2,t3;
	ULONG seed,key[4];
	UCHAR theheader[0x1000],theloader[SIZEOFLOADER];
	ULONG robegin,vobegin,nSecs;
	UCHAR *pRsrc=0,*buf=0;
	PRGINFO PrgInfo;
	SECTION_INFO *pSecInfo;
	KEYSTRUCT ks;
	ENCINFO ei;
	//IMAGE_IMPORT_DESCRIPTOR iid[128]; //more than 128 DLLs? I don't believe.
//	ULONG thunks[16];
	//-------------------------------------
	if(!m_IsFileValid) 
		{AddLine("\r\n请先选择一个可以加密的程序!");goto hell;}
	if(!m_UDisks.GetCount()) 
		{AddLine("系统上并不存在闪存盘!");goto hell;}
	if(!MoveFileEx(m_FileToProtect,m_FileToProtect+"_original",MOVEFILE_REPLACE_EXISTING)) 
		{AddLine("无法复制文件！");goto hell;}

	if(!op.Open(m_FileToProtect+"_original",CFile::modeReadWrite|CFile::typeBinary))
	{
		AddLine("不能打开文件 "+op.GetFileName()+"!");
		goto hell;
	}
	op.Read(&theheader,0x1000);
	nh=(IMAGE_NT_HEADERS*)(&theheader[0]+(*(ULONG*)&theheader[0x3c])); // IMAGE_DOS_HEADERS.e_lfanew = 3ch
	sh=(IMAGE_SECTION_HEADER*)(&theheader[0]+(*(ULONG*)&theheader[0x3c])+sizeof(IMAGE_NT_HEADERS));

	shbk=new IMAGE_SECTION_HEADER[nh->FileHeader.NumberOfSections];
	memmove(shbk,sh,(nh->FileHeader.NumberOfSections)*sizeof(IMAGE_SECTION_HEADER));

	robegin=sh->PointerToRawData;
	vobegin=sh->VirtualAddress;
	nSecs=nh->FileHeader.NumberOfSections;

	//Display info
	s.Format("-----------------\r\n程序基本信息：\r\nEntryPoint:%08lX\r\nNumberOfSections:%08lx\r\nSizeOfImage:%08lX",
		nh->OptionalHeader.AddressOfEntryPoint,
		nh->FileHeader.NumberOfSections,
		nh->OptionalHeader.SizeOfImage);
	AddLine((LPCTSTR)s);
	//get seed & key
	__asm
	{
		rdtsc
		mov seed,eax
	}
	s.Format("-----------------\r\nSeed:%08lX",seed);
	AddLine((LPCTSTR)s);
	for(i=0;i<8;i++)
	{
		Sleep(25);
		__asm
		{
			rdtsc
			mov ecx,i
			lea edx,key
			mov word ptr [edx+2*ecx],ax
		}
	}
	s.Format("Key:%08lX%08lX%08lX%08lX",key[0],key[1],key[2],key[3],key[4]);
	AddLine((LPCTSTR)s);
	AddLine("分析、重建PE头...");
	memset(&PrgInfo,0,sizeof(PRGINFO));
	PrgInfo.EntryPoint=nh->OptionalHeader.AddressOfEntryPoint;
	PrgInfo.ImportTable=nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	where=99999;
	//if there isn't a resource section
	if(nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress==0)
	{
		nh->FileHeader.NumberOfSections=2;
		sh->Characteristics=0xE0000020; // for read,write,execute,contains executable data
		lstrcpy((char*)&(sh->Name),".mess");
		sh->Misc.VirtualSize=nh->OptionalHeader.SizeOfImage;
		sh->SizeOfRawData=0;

		(sh+1)->Characteristics=0xE0000020;
		(sh+1)->Misc.VirtualSize=0x1000;
		lstrcpy((char*)&((sh+1)->Name),".lock");
		(sh+1)->SizeOfRawData=SIZEOFLOADER;
		(sh+1)->VirtualAddress=nh->OptionalHeader.SizeOfImage;
		(sh+1)->PointerToRawData=robegin;

		nh->OptionalHeader.AddressOfEntryPoint=nh->OptionalHeader.SizeOfImage;
		nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size=0;
		nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress=
			nh->OptionalHeader.SizeOfImage+8+sizeof(ENCINFO); //8 for the "pushad|call @F|nop|nop",see src of the loader.

	}
	//if there is
	else
	{
		ULONG pRsrcData=CRvaOffs(nh->OptionalHeader
			.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress,sh,nSecs,&where);
		pRsrc=new UCHAR[shbk[where].SizeOfRawData];
		op.Seek(pRsrcData,CFile::SeekPosition::begin);
		op.Read(pRsrc,shbk[where].SizeOfRawData);
	

		if(where==nSecs-1) // resource is in the last section
		{
			nh->FileHeader.NumberOfSections=3;
			sh->Characteristics=0xE0000020;
			sh->Misc.VirtualSize=shbk[where].VirtualAddress-0x1000;
			lstrcpy((char*)(&sh->Name),".mess");
			sh->SizeOfRawData=0;

			memmove((sh+1),(shbk+where),sizeof(IMAGE_SECTION_HEADER));
			(sh+1)->PointerToRawData=robegin;
			t1=(sh+1)->SizeOfRawData;
			if(t1&nh->OptionalHeader.FileAlignment) t1=(t1|(nh->OptionalHeader.FileAlignment-1)+1);
			(sh+1)->SizeOfRawData=t1;
			
			(sh+2)->Characteristics=0xE0000020;
			(sh+2)->Misc.VirtualSize=0x1000;
			lstrcpy((char*)&((sh+2)->Name),".lock");
			(sh+2)->SizeOfRawData=SIZEOFLOADER;
			(sh+2)->VirtualAddress=nh->OptionalHeader.SizeOfImage;
			(sh+2)->PointerToRawData=robegin+(sh+1)->SizeOfRawData;

			nh->OptionalHeader.AddressOfEntryPoint=nh->OptionalHeader.SizeOfImage;
			nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size=0;
			nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress=
				nh->OptionalHeader.SizeOfImage+8+sizeof(ENCINFO); //8 for the "pushad|call @F|nop|nop",see src of the loader.
		}
		else  // not
		{
			AddLine("为简化程序，不将资源块不在最后一个的程序列入支持范围。");
			AddLine("这种情况十分罕见，你怎么能碰上了呢:)");
			goto hell;
		}
	}

	// for loader to differ whether the original program is a bound import program or not
	if(nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress)
	{
		nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress=
		nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size=
		0;
		PrgInfo.IsBoundImport=0x12345678;
	}


	// generate the overlay
	t2=(where==99999)?(nSecs):(nSecs-1);
	PrgInfo.Sections=t2;
	t1=0;
	for(i=0;i<t2;i++) t1+=shbk[i].SizeOfRawData;
	ei.OverlaySize=((sizeof(SECTION_INFO)*t2+sizeof(PRGINFO)+t1)|7)+1;
	ei.TheSeed=seed;
	if(pRsrc)
		ei.OverlayOffset=robegin+SIZEOFLOADER+shbk[where].SizeOfRawData;
	else
		ei.OverlayOffset=robegin+SIZEOFLOADER;
	buf=(UCHAR*)VirtualAlloc(0,ei.OverlaySize,MEM_COMMIT,PAGE_READWRITE);
	*(PRGINFO*)buf=PrgInfo;
	pSecInfo=(SECTION_INFO*)(buf+sizeof(PRGINFO));
	

	t3=sizeof(SECTION_INFO)*t2+sizeof(PRGINFO);
	op.Seek(robegin,CFile::SeekPosition::begin);
	for(i=0;i<t2;i++)
	{
		op.Read(buf+t3,shbk[i].SizeOfRawData);
		t3+=shbk[i].SizeOfRawData;
		pSecInfo[i].RVA=shbk[i].VirtualAddress;
		pSecInfo[i].Size=shbk[i].SizeOfRawData;
	}
	
	//HAHAHAHAHAHAHA!!!!!!!!!!!!!
	AddLine("加密程序中...");
	TEAEncrypt(buf,&key,ei.OverlaySize);
	
	AddLine("修正程序输入表...");
	//prepare the loader
	memcpy(&theloader,LockResource(LoadResource(GetModuleHandle(NULL),FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_LOADER),"DAT"))),SIZEOFLOADER);
	memcpy(&theloader[0]+8,&ei,sizeof(ei));
	iid=(IMAGE_IMPORT_DESCRIPTOR*)(&theloader[0]+8+sizeof(ENCINFO));
	t3=nh->OptionalHeader.SizeOfImage;
	while(iid->FirstThunk!=0) // adjust import table RVA
	{
		t1=iid->FirstThunk;
		iid->OriginalFirstThunk=(iid->FirstThunk+=t3);
		iid->Name+=t3;
		t1+=(ULONG)&theloader[0];
		while(*(ULONG*)t1)
		{
			*(ULONG*)t1+=t3;
			t1+=sizeof(ULONG);
		}
		iid++;
	}

	nh->OptionalHeader.SizeOfImage+=0x1000;

	AddLine("保存中...");
	//build an new executable
	opdst.Open(m_FileToProtect,CFile::OpenFlags::shareExclusive|CFile::OpenFlags::modeWrite|CFile::OpenFlags::modeCreate);
	opdst.Write(&theheader,robegin);
	if(pRsrc) opdst.Write(pRsrc,sh[1].SizeOfRawData);
	opdst.Write(&theloader,SIZEOFLOADER);
	opdst.Write(buf,ei.OverlaySize);
	opdst.Close();
	
	AddLine("将密钥写入闪存盘...");
	//write encrypt info to flash disk
	m_UDisks.GetText(m_UDisks.GetCurSel(),s);
	ks.Seed=seed;
	memcpy(&ks.TheKey,&key,sizeof(key));
	SetFlashDiskInfo(s[0],&ks);
	RefreshUDiskList();
	m_IsFileValid=0;
	AddLine("保护完成。By Proton 2007");
hell:;
	 VirtualFree(buf,0,MEM_RELEASE);
	 delete pRsrc;
}

void CFlashLockerDlg::AddLine(LPCTSTR op)
{
	ULONG hehe=m_Message.GetWindowTextLength();
	m_Message.SetSel(hehe,hehe,TRUE);
	m_Message.ReplaceSel(op);
	m_Message.ReplaceSel("\r\n");
	m_Message.SetFocus();
}

void CFlashLockerDlg::RefreshUDiskList()
{

	char drvstr[]="A:\\";
	char i;
	char buf[512];
	KEYSTRUCT ks;
	m_UDisks.ResetContent(); 
	for(i='D';i<='Z';i++)
	{
		drvstr[0]=i;
		if(GetDriveType(drvstr)==DRIVE_REMOVABLE)
		{
			buf[0]=i;
			buf[1]=0;
			if(!GetFlashDiskInfo(i,&ks))
			{
				wsprintf(buf,"%s: - 驱动器未准备",buf);
				m_UDisks.AddString(buf);
				continue;
			}
			if(ks.Signature=='KCOL')
			{
				wsprintf(buf,"%s: - 已使用,Seed:%08lX",buf,ks.Seed);
				m_UDisks.AddString(buf);
			}
			else
			{
				strcat(buf,": - 未使用");
				m_UDisks.AddString(buf);
			}

		}
	}
	m_UDisks.SetCurSel(0);
}

ULONG CFlashLockerDlg::CRvaOffs(ULONG rva, IMAGE_SECTION_HEADER *pSecHeaders, ULONG nHeaders, ULONG *Where)
{
	ULONG i; 
	ULONG tmp;
	if(rva<0x1000) return rva;
	for(i=0;i<nHeaders-1;i++)
	{
		tmp=pSecHeaders->VirtualAddress;
		if(rva>=tmp && rva<(pSecHeaders+1)->VirtualAddress)
		{
			if(Where) *Where=i;
			return rva-pSecHeaders->VirtualAddress+pSecHeaders->PointerToRawData;
		}
		pSecHeaders++;
	}
	if(rva>=pSecHeaders->VirtualAddress && rva<(pSecHeaders->VirtualAddress+pSecHeaders->Misc.VirtualSize))
	{
		if(Where) *Where=i;
		return rva-pSecHeaders->VirtualAddress+pSecHeaders->PointerToRawData;
	}
	else
		return 0;

}


#define DELTA 0x9e3779b9
#pragma warning(disable:4307)
long CFlashLockerDlg::TEAEncrypt(void *pData, void *pKey, ULONG cbData)
{
	ULONG i,limit,sum;
	ULONG y,z;
	if(cbData==0 || (cbData&7)) return FALSE;
	for(i=0;i<(cbData>>2);i+=2)
	{
		y=((ULONG*)pData)[i];
		z=((ULONG*)pData)[i+1];
		limit=DELTA*128;
		sum=0;
		while(sum!=limit)
		{
			 y+=((z<<4)^(z>>5)) + (z^sum) + ((ULONG*)pKey)[sum&3];
			 sum+=DELTA;
			 z+=((y<<4)^(y>>5)) + (y^sum) + ((ULONG*)pKey)[(sum>>11)&3];
		}
		((ULONG*)pData)[i]=y;
		((ULONG*)pData)[i+1]=z;
	}
	return TRUE;
}
/* Actually, this program did't use this function
long CFlashLockerDlg::TEADecrypt(void *pData, void *pKey, ULONG cbData)
{
	ULONG i,limit,sum=0;
	ULONG y,z;
	if(cbData==0 || (cbData&7)) return FALSE;
	for(i=0;i<(cbData>>2);i+=2)
	{
		y=((ULONG*)pData)[i];
		z=((ULONG*)pData)[i+1];
		sum=DELTA*128;
		while(sum)
		{
			z-=((y<<4)^(y>>5)) + (y^sum) + ((ULONG*)pKey)[(sum>>11)&3];
			sum-=DELTA;
			y-=((z<<4)^(z>>5)) + (z^sum) + ((ULONG*)pKey)[sum&3];
		}
		((ULONG*)pData)[i]=y;
		((ULONG*)pData)[i+1]=z;
	}
	return TRUE;
}
*/
#undef DELTA
#pragma warning(default:4307)