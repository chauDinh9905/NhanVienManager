#pragma once
#include "afxdialogex.h"


// CDlgLogin dialog

class CDlgLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgLogin();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strHost;
	CString m_strPort;
	CString m_strUser;
	CString m_strPass;
	afx_msg void OnClickedBtnConnect();
};
