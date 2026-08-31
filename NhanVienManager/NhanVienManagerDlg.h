
// NhanVienManagerDlg.h : header file
//

#pragma once


// CNhanVienManagerDlg dialog
class CNhanVienManagerDlg : public CDialogEx
{
// Construction
public:
	CNhanVienManagerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NHANVIENMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
//	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_listSchema;
	CListCtrl m_listNhanVien;
	CComboBox m_cmbLanguage;
	afx_msg void OnClickedBtnAdd();
	afx_msg void OnClickedBtnEdit();
	afx_msg void OnClickedBtnDelete();
	afx_msg void OnSelchangeCmbLanguage();
    virtual void InitListControl();
	virtual void LoadDataToListView();
	virtual void UpdateUIThemeLanguage();
};
