// FlashLockerDlg.h : header file
//

#if !defined(AFX_FLASHLOCKERDLG_H__6014132C_B142_48FA_90C4_8146E32EB31C__INCLUDED_)
#define AFX_FLASHLOCKERDLG_H__6014132C_B142_48FA_90C4_8146E32EB31C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFlashLockerDlg dialog
class CFlashLockerDlg : public CDialog
{
// Construction
public:
	//long TEADecrypt(void *pData,void *pKey,ULONG cbData);
	long TEAEncrypt(void *pData,void *pKey,ULONG cbData);
	ULONG CRvaOffs(ULONG rva,IMAGE_SECTION_HEADER* pSecHeaders,ULONG nHeaders, ULONG *Where=0);
	CFlashLockerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFlashLockerDlg)
	enum { IDD = IDD_FLASHLOCKER_DIALOG };
	CEdit	m_Message;
	CListBox	m_UDisks;
	CString	m_FileToProtect;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashLockerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshUDiskList();
	long m_IsFileValid;
	void AddLine(LPCTSTR op);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFlashLockerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeUdisks();
	afx_msg void OnBrowse();
	afx_msg void OnProtect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHLOCKERDLG_H__6014132C_B142_48FA_90C4_8146E32EB31C__INCLUDED_)
