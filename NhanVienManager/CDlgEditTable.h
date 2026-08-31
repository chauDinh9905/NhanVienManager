#pragma once
#include "afxdialogex.h"
#include "FieldInfo.h"
#include "vector"
using namespace std;
// CDlgEditTable dialog

class CDlgEditTable : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEditTable)

public:
	CDlgEditTable(const CString& strTableName, CWnd* pParent = nullptr);  // standard constructor
	virtual ~CDlgEditTable();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TABLE_SCHEMA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listFields;
	CComboBox m_cmbDataType;
	CString m_strTableName;
	vector<FieldInfo>    m_vecFields;
	afx_msg void OnClickedBtnAddField();
	afx_msg void OnClickedBtnDelField();
	afx_msg void OnClickedBtnEditField();
	afx_msg void OnBnClickedOk();
	void LoadCurrentSchema();
	void RefreshListControl();
	vector<CString> BuildAlterStatements();
};
