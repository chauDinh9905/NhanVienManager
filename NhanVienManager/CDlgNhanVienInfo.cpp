// CDlgNhanVienInfo.cpp : implementation file
//

#include "pch.h"
#include "NhanVienManager.h"
#include "afxdialogex.h"
#include "CDlgNhanVienInfo.h"
#include "LanguageManager.h"

// CDlgNhanVienInfo dialog

IMPLEMENT_DYNAMIC(CDlgNhanVienInfo, CDialogEx)

CDlgNhanVienInfo::CDlgNhanVienInfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_NHANVIEN_INFO, pParent)
	, m_strAccount(_T(""))
	, m_strHoTen(_T(""))
	, m_strQueQuan(_T(""))
	, m_strNgaySinh(_T(""))
	, m_strTruongHoc(_T(""))
	, m_nGioiTinh(0)
{

}

CDlgNhanVienInfo::~CDlgNhanVienInfo()
{
}

void CDlgNhanVienInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strAccount);
	DDX_Text(pDX, IDC_EDIT_HOTEN, m_strHoTen);
	DDX_Text(pDX, IDC_EDIT_QUEQUAN, m_strQueQuan);
	DDX_Text(pDX, IDC_EDIT_NGAYSINH, m_strNgaySinh);
	DDX_Text(pDX, IDC_EDIT_TRUONGHOC, m_strTruongHoc);
	DDX_Radio(pDX, IDC_RADIO_NAM, m_nGioiTinh);
}


BEGIN_MESSAGE_MAP(CDlgNhanVienInfo, CDialogEx)
END_MESSAGE_MAP()


// CDlgNhanVienInfo message handlers
BOOL CDlgNhanVienInfo::OnInitDialog() {
	CDialogEx::OnInitDialog();
	CLanguageManager& langMgr = CLanguageManager::GetInstance();

	SetWindowText(m_bEditMode ? langMgr.GetString(_T("DLG_TITLE_EDIT")) : langMgr.GetString(_T("DLG_TITLE_ADD")));
	SetDlgItemText(IDC_LBL_ACCOUNT, langMgr.GetString(_T("LBL_ACCOUNT")));
	SetDlgItemText(IDC_LBL_HOTEN, langMgr.GetString(_T("LBL_HOTEN")));
	SetDlgItemText(IDC_LBL_QUEQUAN, langMgr.GetString(_T("LBL_QUEQUAN")));
	SetDlgItemText(IDC_LBL_NGAYSINH, langMgr.GetString(_T("LBL_NGAYSINH")));
	SetDlgItemText(IDC_LBL_TRUONGHOC, langMgr.GetString(_T("LBL_TRUONGHOC")));
	SetDlgItemText(IDC_STATIC_ID, m_strID);

	if (m_bEditMode) GetDlgItem(IDC_EDIT_ACCOUNT)->SetFocus();
	return TRUE;
}

bool CDlgNhanVienInfo::IsValidDate(const CString& strDate) {
	if (strDate.GetLength() != 10) return false;
	if (strDate[4] != _T('-') || strDate[7] != _T('-')) return false;
	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) continue;
		if (!_istdigit(strDate[i])) return false;
	}
	int nYear = _ttoi(strDate.Left(4));
	int nMonth = _ttoi(strDate.Mid(5, 2));
	int nDay = _ttoi(strDate.Mid(8, 2));
	if (nMonth < 1 || nMonth > 12) return false;
	if (nYear < 1900 || nYear > 2100) return false;
	int arrDaysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	bool bLeap = (nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0);
	int nMaxDay = arrDaysInMonth[nMonth - 1] + ((nMonth == 2 && bLeap) ? 1 : 0);
	return (nDay >= 1 && nDay <= nMaxDay);
}

bool CDlgNhanVienInfo::ValidateInput() {
	UpdateData(TRUE);
	CLanguageManager& langMgr = CLanguageManager::GetInstance();

	if (m_strAccount.IsEmpty() || m_strHoTen.IsEmpty() ||
		m_strQueQuan.IsEmpty() || m_strTruongHoc.IsEmpty() || m_strNgaySinh.IsEmpty()) {
		AfxMessageBox(langMgr.GetString(_T("ERR_EMPTY_FIELD")), MB_ICONERROR);
		return false;
	}
	for (int i = 0; i < m_strHoTen.GetLength(); i++) {
		if (_istdigit(m_strHoTen[i])) {
			AfxMessageBox(langMgr.GetString(_T("ERR_INVALID_HOTEN")), MB_ICONERROR);
			return false;
		}
	}
	for (int i = 0; i < m_strAccount.GetLength(); i++) {
		TCHAR ch = m_strAccount[i];
		if (!_istalnum(ch) && ch != _T('_')) {
			AfxMessageBox(langMgr.GetString(_T("ERR_INVALID_ACCOUNT")), MB_ICONERROR);
			return false;
		}
	}
	if (!IsValidDate(m_strNgaySinh)) {
		AfxMessageBox(langMgr.GetString(_T("ERR_INVALID_NGAYSINH")), MB_ICONERROR);
		return false;
	}
	return true;
}

void CDlgNhanVienInfo::OnOK() {
	if (!ValidateInput()) return;
	CDialogEx::OnOK();
}