#pragma once
#include "afxdialogex.h"


// CDlgConnectDB dialog

class CDlgConnectDB : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConnectDB)

public:
	CDlgConnectDB(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgConnectDB();

	virtual void OnFinalRelease();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CONNECT_DB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CString m_strDBName;
	CString m_strTableName;
	afx_msg void OnClickedBtnCheck();
//	afx_msg void OnBtnConnect();
	afx_msg void OnClickedBtnCreateTable();
	afx_msg void OnClickedBtnEditTable();
	bool CheckDatabaseExists(const CString& strDBName);
	bool CheckTableExists(const CString& strTableName);
};
