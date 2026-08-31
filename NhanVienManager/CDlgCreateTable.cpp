// CDlgCreateTable.cpp : implementation file
//

#include "pch.h"
#include "NhanVienManager.h"
#include "afxdialogex.h"
#include "CDlgCreateTable.h"
#include "DBManager.h"


CDlgCreateTable::CDlgCreateTable(const CString& strTableName, CWnd* pParent)
    : CDialogEx(IDD_DLG_TABLE_SCHEMA, pParent), m_strTableName(strTableName) {
}
CDlgCreateTable::~CDlgCreateTable()
{
}

void CDlgCreateTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIELDS, m_listFields);
	DDX_Control(pDX, IDC_CMB_DATATYPE, m_cmbDataType);
}


BEGIN_MESSAGE_MAP(CDlgCreateTable, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD_FIELD, &CDlgCreateTable::OnClickedBtnAddField)
	ON_BN_CLICKED(IDC_BTN_DEL_FIELD, &CDlgCreateTable::OnClickedBtnDelField)
	ON_BN_CLICKED(IDC_BTN_EDIT_FIELD, &CDlgCreateTable::OnClickedBtnEditField)
    ON_BN_CLICKED(IDOK, &CDlgCreateTable::OnClickedOk)
END_MESSAGE_MAP()


// CDlgCreateTable message handlers

void CDlgCreateTable::OnClickedBtnAddField()
{
	// TODO: Add your control notification handler code here
    CString strName, strType;
    GetDlgItemText(IDC_EDIT_FIELDNAME, strName);
    m_cmbDataType.GetWindowText(strType);
    strName.Trim();

    if (!ValidateFieldName(strName)) return;
    if (strType.IsEmpty()) {
        AfxMessageBox(_T("Vui lòng chọn kiểu dữ liệu!"), MB_ICONERROR);
        return;
    }

    bool bPK = (((CButton*)GetDlgItem(IDC_CHK_PK))->GetCheck() == BST_CHECKED);
    bool bNN = (((CButton*)GetDlgItem(IDC_CHK_NOTNULL))->GetCheck() == BST_CHECKED);
    if (bPK) for (auto& f : m_vecFields) f.bPrimaryKey = false;

    m_vecFields.push_back({ _T(""), strName, strType, bPK, bNN || bPK, FieldOp::Add });
    RefreshListControl();
    SetDlgItemText(IDC_EDIT_FIELDNAME, _T(""));

}

void CDlgCreateTable::OnClickedBtnDelField()
{
	// TODO: Add your control notification handler code here
    POSITION pos = m_listFields.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(_T("Vui lòng chọn 1 trường để xóa!"), MB_ICONWARNING); return; }
    int nItem = m_listFields.GetNextSelectedItem(pos);
    if (m_vecFields[nItem].strName.CompareNoCase(_T("ID")) == 0) {
        AfxMessageBox(_T("Không được xóa cột khóa chính ID!"), MB_ICONERROR);
        return;
    }
    m_vecFields.erase(m_vecFields.begin() + nItem);
    RefreshListControl();
}

void CDlgCreateTable::OnClickedBtnEditField()
{
	// TODO: Add your control notification handler code here
    POSITION pos = m_listFields.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(_T("Vui lòng chọn 1 trường để sửa!"), MB_ICONWARNING); return; }
    int nItem = m_listFields.GetNextSelectedItem(pos);

    CString strName, strType;
    GetDlgItemText(IDC_EDIT_FIELDNAME, strName);
    m_cmbDataType.GetWindowText(strType);
    strName.Trim();
    if (!ValidateFieldName(strName, nItem)) return;

    bool bPK = (((CButton*)GetDlgItem(IDC_CHK_PK))->GetCheck() == BST_CHECKED);
    bool bNN = (((CButton*)GetDlgItem(IDC_CHK_NOTNULL))->GetCheck() == BST_CHECKED);
    if (bPK) for (auto& f : m_vecFields) f.bPrimaryKey = false;

    m_vecFields[nItem].strName = strName;
    m_vecFields[nItem].strDataType = strType;
    m_vecFields[nItem].bPrimaryKey = bPK;
    m_vecFields[nItem].bNotNull = bNN || bPK;
    RefreshListControl();
}
BOOL CDlgCreateTable::OnInitDialog() {
    CDialogEx::OnInitDialog();
    SetDlgItemText(IDC_EDIT_TABLENAME, m_strTableName);
    GetDlgItem(IDC_EDIT_TABLENAME)->EnableWindow(FALSE);

    m_listFields.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_listFields.InsertColumn(0, _T("Tên Trường"), LVCFMT_LEFT, 140);
    m_listFields.InsertColumn(1, _T("Kiểu Dữ Liệu"), LVCFMT_LEFT, 120);
    m_listFields.InsertColumn(2, _T("NOT NULL"), LVCFMT_CENTER, 80);
    m_listFields.InsertColumn(3, _T("Primary Key"), LVCFMT_CENTER, 80);

    const TCHAR* arrTypes[] = { _T("INT"), _T("VARCHAR(50)"), _T("VARCHAR(100)"),
                                 _T("VARCHAR(200)"), _T("TEXT"), _T("DATETIME"), _T("DATE") };
    for (auto t : arrTypes) m_cmbDataType.AddString(t);

    m_vecFields = {
        { _T(""), _T("ID"),         _T("INT"),          true,  true,  FieldOp::Add },
        { _T(""), _T("Account"),    _T("VARCHAR(100)"), false, true,  FieldOp::Add },
        { _T(""), _T("HoTen"),      _T("VARCHAR(150)"), false, true,  FieldOp::Add },
        { _T(""), _T("QueQuan"),    _T("VARCHAR(200)"), false, false, FieldOp::Add },
        { _T(""), _T("NgaySinh"),   _T("DATETIME"),     false, false, FieldOp::Add },
        { _T(""), _T("GioiTinh"),   _T("INT"),          false, false, FieldOp::Add },
        { _T(""), _T("TruongHoc"),  _T("VARCHAR(200)"), false, false, FieldOp::Add },
    };
    RefreshListControl();
    return TRUE;
}
void CDlgCreateTable::RefreshListControl() {
    m_listFields.DeleteAllItems();
    for (size_t i = 0; i < m_vecFields.size(); i++) {
        const FieldInfo& f = m_vecFields[i];
        int nItem = m_listFields.InsertItem((int)i, f.strName);
        m_listFields.SetItemText(nItem, 1, f.strDataType);
        m_listFields.SetItemText(nItem, 2, f.bNotNull ? _T("Có") : _T("Không"));
        m_listFields.SetItemText(nItem, 3, f.bPrimaryKey ? _T("✓") : _T(""));
    }
}
bool CDlgCreateTable::ValidateFieldName(const CString& strName, int nIgnoreIndex) {
    if (strName.IsEmpty()) {
        AfxMessageBox(_T("Tên trường không được để trống!"), MB_ICONERROR);
        return false;
    }
    for (size_t i = 0; i < m_vecFields.size(); i++) {
        if ((int)i == nIgnoreIndex) continue;
        if (m_vecFields[i].strName.CompareNoCase(strName) == 0) {
            AfxMessageBox(_T("Tên trường đã tồn tại trong danh sách!"), MB_ICONERROR);
            return false;
        }
    }
    return true;
}

CString CDlgCreateTable::BuildCreateTableSQL() {
    CString strSQL;
    strSQL.Format(_T("CREATE TABLE %s ("), m_strTableName);
    for (size_t i = 0; i < m_vecFields.size(); i++) {
        const FieldInfo& f = m_vecFields[i];
        CString strCol;
        strCol.Format(_T("%s %s%s%s"),
            f.strName, f.strDataType,
            f.bNotNull ? _T(" NOT NULL") : _T(""),
            f.bPrimaryKey ? _T(" AUTO_INCREMENT PRIMARY KEY") : _T(""));
        strSQL += strCol;
        if (i != m_vecFields.size() - 1) strSQL += _T(", ");
    }
    strSQL += _T(") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    return strSQL;
}
void CDlgCreateTable::OnClickedOk()
{
    // TODO: Add your control notification handler code here
    bool bHasPK = false;
    for (auto& f : m_vecFields)
        if (f.bPrimaryKey)
            bHasPK = true;
    if (!bHasPK) { AfxMessageBox(_T("Bảng phải có ít nhất 1 trường Primary Key!"), MB_ICONERROR); return; }
    if (m_vecFields.empty()) { AfxMessageBox(_T("Danh sách trường không được để trống!"), MB_ICONERROR); return; }

    m_strGeneratedSQL = BuildCreateTableSQL();
    if (CDBManager::GetInstance().ExecuteQuery(m_strGeneratedSQL)) {
        AfxMessageBox(_T("Tạo bảng thành công!"), MB_ICONINFORMATION);
        CDialogEx::OnOK();
    }
    else {
        AfxMessageBox(_T("Tạo bảng thất bại! Vui lòng kiểm tra lại cấu trúc."), MB_ICONERROR);
    }
}
