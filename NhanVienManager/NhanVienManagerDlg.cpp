
// NhanVienManagerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "NhanVienManager.h"
#include "NhanVienManagerDlg.h"
#include "afxdialogex.h"
#include "DBManager.h"
#include "CDlgNhanVienInfo.h"
#include "LanguageManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNhanVienManagerDlg dialog



CNhanVienManagerDlg::CNhanVienManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NHANVIENMANAGER_DIALOG, pParent)
{

}

void CNhanVienManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_NHANVIEN, m_listNhanVien);
	DDX_Control(pDX, IDC_CMB_LANGUAGE, m_cmbLanguage);
}

BEGIN_MESSAGE_MAP(CNhanVienManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ADD, &CNhanVienManagerDlg::OnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CNhanVienManagerDlg::OnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CNhanVienManagerDlg::OnClickedBtnDelete)
	ON_CBN_SELCHANGE(IDC_CMB_LANGUAGE, &CNhanVienManagerDlg::OnSelchangeCmbLanguage)
END_MESSAGE_MAP()


// CNhanVienManagerDlg message handlers

BOOL CNhanVienManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_cmbLanguage.AddString(_T("Tiếng Việt"));
	m_cmbLanguage.AddString(_T("English"));
	m_cmbLanguage.SetCurSel((int)CLanguageManager::GetInstance().GetLanguage());
	UpdateUIThemeLanguage();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNhanVienManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


void CNhanVienManagerDlg::OnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	CDlgNhanVienInfo dlg;
	dlg.m_bEditMode = false;
	dlg.m_strID = _T("(Tự động)");
	if (dlg.DoModal() != IDOK) return;

	CDBManager& db = CDBManager::GetInstance();
	CString strSQL;
	strSQL.Format(
		_T("INSERT INTO tb_nhanvien (Account, HoTen, QueQuan, NgaySinh, GioiTinh, TruongHoc) ")
		_T("VALUES ('%s', '%s', '%s', '%s', %d, '%s')"),
		db.EscapeString(dlg.m_strAccount), db.EscapeString(dlg.m_strHoTen),
		db.EscapeString(dlg.m_strQueQuan), dlg.m_strNgaySinh,
		dlg.m_nGioiTinh == 0 ? 1 : 0,   // radio index 0 (Nam) -> lưu GioiTinh=1
		db.EscapeString(dlg.m_strTruongHoc));

	if (db.ExecuteQuery(strSQL)) {
		AfxMessageBox(CLanguageManager::GetInstance().GetString(_T("MSG_ADD_SUCCESS")), MB_ICONINFORMATION);
		LoadDataToListView();
	}
	else {
		AfxMessageBox(CLanguageManager::GetInstance().GetString(_T("MSG_ADD_FAIL")), MB_ICONERROR);
	}
}

void CNhanVienManagerDlg::OnClickedBtnEdit()
{
	// TODO: Add your control notification handler code here
	UINT nSelectedCount = m_listNhanVien.GetSelectedCount();
	CLanguageManager& langMgr = CLanguageManager::GetInstance();
	if (nSelectedCount != 1) {
		AfxMessageBox(langMgr.GetString(_T("ERR_SELECT_ONE")), MB_ICONWARNING);
		return;
	}

	POSITION pos = m_listNhanVien.GetFirstSelectedItemPosition();
	int nItem = m_listNhanVien.GetNextSelectedItem(pos);

	CDlgNhanVienInfo dlg;
	dlg.m_bEditMode = true;
	dlg.m_strID = m_listNhanVien.GetItemText(nItem, 0);
	dlg.m_strAccount = m_listNhanVien.GetItemText(nItem, 1);
	dlg.m_strHoTen = m_listNhanVien.GetItemText(nItem, 2);
	dlg.m_strQueQuan = m_listNhanVien.GetItemText(nItem, 3);
	dlg.m_strNgaySinh = m_listNhanVien.GetItemText(nItem, 4);
	dlg.m_nGioiTinh = (m_listNhanVien.GetItemText(nItem, 5) == langMgr.GetString(_T("GENDER_MALE"))) ? 0 : 1;
	dlg.m_strTruongHoc = m_listNhanVien.GetItemText(nItem, 6);

	if (dlg.DoModal() != IDOK) return;

	CDBManager& db = CDBManager::GetInstance();
	CString strSQL;
	strSQL.Format(
		_T("UPDATE tb_nhanvien SET Account='%s', HoTen='%s', QueQuan='%s', NgaySinh='%s', GioiTinh=%d, TruongHoc='%s' WHERE ID=%s"),
		db.EscapeString(dlg.m_strAccount), db.EscapeString(dlg.m_strHoTen),
		db.EscapeString(dlg.m_strQueQuan), dlg.m_strNgaySinh,
		dlg.m_nGioiTinh == 0 ? 1 : 0,
		db.EscapeString(dlg.m_strTruongHoc), dlg.m_strID);

	if (db.ExecuteQuery(strSQL)) {
		AfxMessageBox(langMgr.GetString(_T("MSG_EDIT_SUCCESS")), MB_ICONINFORMATION);
		LoadDataToListView();
	}
	else {
		AfxMessageBox(langMgr.GetString(_T("MSG_EDIT_FAIL")), MB_ICONERROR);
	}
}

void CNhanVienManagerDlg::OnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	UINT nSelectedCount = m_listNhanVien.GetSelectedCount();
	CLanguageManager& langMgr = CLanguageManager::GetInstance();

	if (nSelectedCount == 0) {
		AfxMessageBox(langMgr.GetString(_T("ERR_SELECT_NONE_DELETE")), MB_ICONWARNING);
		return;
	}

	CString strConfirmMsg;
	strConfirmMsg.Format(langMgr.GetString(_T("CONFIRM_DELETE")), nSelectedCount);
	if (AfxMessageBox(strConfirmMsg, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	CStringArray arrIDs;
	POSITION pos = m_listNhanVien.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = m_listNhanVien.GetNextSelectedItem(pos);
		arrIDs.Add(m_listNhanVien.GetItemText(nItem, 0));
	}

	CString strIDList;
	for (int i = 0; i < arrIDs.GetSize(); i++) {
		strIDList += arrIDs[i];
		if (i < arrIDs.GetSize() - 1) strIDList += _T(",");
	}

	CString strSQL;
	strSQL.Format(_T("DELETE FROM tb_nhanvien WHERE ID IN (%s)"), strIDList);
	if (CDBManager::GetInstance().ExecuteQuery(strSQL)) {
		AfxMessageBox(langMgr.GetString(_T("MSG_DELETE_SUCCESS")), MB_ICONINFORMATION);
		LoadDataToListView();
	}
}

void CNhanVienManagerDlg::OnSelchangeCmbLanguage()
{
	// TODO: Add your control notification handler code here
	int nSel = m_cmbLanguage.GetCurSel();
	CLanguageManager::GetInstance().SetLanguage(nSel == 0 ? LANG_VIETNAMESE : LANG_ENGLISH);
	UpdateUIThemeLanguage();
}
void CNhanVienManagerDlg::InitListControl() {
	m_listNhanVien.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listNhanVien.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_listNhanVien.InsertColumn(1, _T("Tài Khoản"), LVCFMT_LEFT, 100);
	m_listNhanVien.InsertColumn(2, _T("Họ và Tên"), LVCFMT_LEFT, 140);
	m_listNhanVien.InsertColumn(3, _T("Quê Quán"), LVCFMT_LEFT, 120);
	m_listNhanVien.InsertColumn(4, _T("Ngày Sinh"), LVCFMT_CENTER, 100);
	m_listNhanVien.InsertColumn(5, _T("Giới Tính"), LVCFMT_CENTER, 70);
	m_listNhanVien.InsertColumn(6, _T("Trường Học"), LVCFMT_LEFT, 140);
}
void CNhanVienManagerDlg::LoadDataToListView() {
	m_listNhanVien.DeleteAllItems();
	CDBManager& db = CDBManager::GetInstance();
	CLanguageManager& langMgr = CLanguageManager::GetInstance();

	MYSQL_RES* res = db.FetchResult(_T("SELECT ID, Account, HoTen, QueQuan, ")
		_T("DATE_FORMAT(NgaySinh, '%Y-%m-%d'), GioiTinh, TruongHoc FROM tb_nhanvien"));
	if (!res) return;

	MYSQL_ROW row;
	int nIndex = 0;
	while ((row = mysql_fetch_row(res))) {
		CString strID(row[0]);
		CString strAccount(row[1] ? row[1] : "");
		CString strHoTen(row[2] ? row[2] : "");
		CString strQueQuan(row[3] ? row[3] : "");
		CString strNgaySinh(row[4] ? row[4] : "");
		int nGioiTinh = row[5] ? _ttoi(CA2T(row[5])) : 0;
		CString strGioiTinh = langMgr.GetString(nGioiTinh == 1 ? _T("GENDER_MALE") : _T("GENDER_FEMALE"));
		CString strTruongHoc(row[6] ? row[6] : "");

		int item = m_listNhanVien.InsertItem(nIndex, strID);
		m_listNhanVien.SetItemText(item, 1, strAccount);
		m_listNhanVien.SetItemText(item, 2, strHoTen);
		m_listNhanVien.SetItemText(item, 3, strQueQuan);
		m_listNhanVien.SetItemText(item, 4, strNgaySinh);
		m_listNhanVien.SetItemText(item, 5, strGioiTinh);
		m_listNhanVien.SetItemText(item, 6, strTruongHoc);
		nIndex++;
	}
	mysql_free_result(res);
}

void CNhanVienManagerDlg::UpdateUIThemeLanguage() {
	CLanguageManager& langMgr = CLanguageManager::GetInstance();

	SetWindowText(langMgr.GetString(_T("TITLE_MAIN")));
	GetDlgItem(IDC_BTN_ADD)->SetWindowText(langMgr.GetString(_T("BTN_ADD")));
	GetDlgItem(IDC_BTN_EDIT)->SetWindowText(langMgr.GetString(_T("BTN_EDIT")));
	GetDlgItem(IDC_BTN_DELETE)->SetWindowText(langMgr.GetString(_T("BTN_DELETE")));

	const TCHAR* arrColKeys[7] = {
		_T("COL_ID"), _T("COL_ACCOUNT"), _T("COL_HOTEN"), _T("COL_QUEQUAN"),
		_T("COL_NGAYSINH"), _T("COL_GIOITINH"), _T("COL_TRUONGHOC")
	};
	for (int nCol = 0; nCol < 7; nCol++) {
		CString strColText = langMgr.GetString(arrColKeys[nCol]);
		LVCOLUMN lvc;
		lvc.mask = LVCF_TEXT;
		lvc.pszText = strColText.GetBuffer(strColText.GetLength());
		m_listNhanVien.SetColumn(nCol, &lvc);
		strColText.ReleaseBuffer();
	}

	LoadDataToListView(); // Cột Giới Tính chứa chuỗi đã dịch -> phải nạp lại
}