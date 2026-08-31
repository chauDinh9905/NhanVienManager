#pragma once
#include "afxdialogex.h"


// CDlgNhanVienInfo dialog

class CDlgNhanVienInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNhanVienInfo)

public:
	CDlgNhanVienInfo(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNhanVienInfo();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NHANVIEN_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	bool ValidateInput();
	bool IsValidDate(const CString& strDate);
	DECLARE_MESSAGE_MAP()
public:
	bool     m_bEditMode = false;
	CString  m_strID;
	CString m_strAccount;
	CString m_strHoTen;
	CString m_strQueQuan;
	CString m_strNgaySinh;
	CString m_strTruongHoc;
	int m_nGioiTinh = 0;
};
