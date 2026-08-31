// CDlgLogin.cpp : implementation file
//

#include "pch.h"
#include "NhanVienManager.h"
#include "afxdialogex.h"
#include "CDlgLogin.h"
#include "LanguageManager.h"
#include "DBManager.h"


// CDlgLogin dialog

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_strHost(_T(""))
	, m_strPort(_T(""))
	, m_strUser(_T(""))
	, m_strPass(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CDlgLogin::OnClickedBtnConnect)
END_MESSAGE_MAP()

BOOL CDlgLogin::OnInitDialog() {
	CDialogEx::OnInitDialog();
	SetDlgItemText(IDC_EDIT_HOST, _T("localhost"));
	SetDlgItemText(IDC_EDIT_PORT, _T("3306"));
	return TRUE;
}

// CDlgLogin message handlers

void CDlgLogin::OnClickedBtnConnect()
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CLanguageManager& langMgr = CLanguageManager::GetInstance();

    if (m_strHost.IsEmpty() || m_strPort.IsEmpty() || m_strUser.IsEmpty()) {
        AfxMessageBox(langMgr.GetString(_T("ERR_LOGIN_EMPTY")), MB_ICONERROR);
        return;
    }
    if (!_istdigit(m_strPort[0])) {
        AfxMessageBox(langMgr.GetString(_T("ERR_PORT_INVALID")), MB_ICONERROR);
        return;
    }

    unsigned int nPort = (unsigned int)_ttoi(m_strPort);
    // Kết nối server MySQL trước, chưa chọn Database cụ thể (việc đó thuộc Lab 2)
    bool bOk = CDBManager::GetInstance().Connect(m_strHost, m_strUser, m_strPass, _T(""), nPort);

    if (bOk) {
        CDialogEx::OnOK();
    }
    else {
        AfxMessageBox(langMgr.GetString(_T("ERR_CONNECT_FAIL")), MB_ICONERROR);
        // Không đóng dialog -> cho phép sửa lại và thử kết nối tiếp
    }
}
