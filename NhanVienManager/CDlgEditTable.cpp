// CDlgEditTable.cpp : implementation file
//

#include "pch.h"
#include "NhanVienManager.h"
#include "afxdialogex.h"
#include "CDlgEditTable.h"
#include "DBManager.h"


// CDlgEditTable dialog

IMPLEMENT_DYNAMIC(CDlgEditTable, CDialogEx)

CDlgEditTable::CDlgEditTable(const CString& strTableName, CWnd* pParent)
    : CDialogEx(IDD_DLG_TABLE_SCHEMA, pParent), m_strTableName(strTableName) {
}

CDlgEditTable::~CDlgEditTable()
{
}

void CDlgEditTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIELDS, m_listFields);
	DDX_Control(pDX, IDC_CMB_DATATYPE, m_cmbDataType);
}


BEGIN_MESSAGE_MAP(CDlgEditTable, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD_FIELD, &CDlgEditTable::OnClickedBtnAddField)
	ON_BN_CLICKED(IDC_BTN_DEL_FIELD, &CDlgEditTable::OnClickedBtnDelField)
	ON_BN_CLICKED(IDC_BTN_EDIT_FIELD, &CDlgEditTable::OnClickedBtnEditField)
	ON_BN_CLICKED(IDOK, &CDlgEditTable::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CDlgEditTable::OnInitDialog() {
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

    LoadCurrentSchema();
    RefreshListControl();
    return TRUE;
}
void CDlgEditTable::LoadCurrentSchema() {
    m_vecFields.clear();
    CDBManager& db = CDBManager::GetInstance();
    MYSQL_RES* res = db.FetchResult(_T("DESCRIBE ") + m_strTableName);
    if (!res) return;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        FieldInfo f;
        f.strOldName = CString(row[0]);
        f.strName = f.strOldName;
        f.strDataType = CString(row[1]);
        f.bNotNull = (CString(row[2]) == _T("NO"));
        f.bPrimaryKey = (CString(row[3]) == _T("PRI"));
        f.op = FieldOp::Existing;
        m_vecFields.push_back(f);
    }
    mysql_free_result(res);
}
void CDlgEditTable::RefreshListControl() {
    m_listFields.DeleteAllItems();
    for (size_t i = 0; i < m_vecFields.size(); i++) {
        const FieldInfo& f = m_vecFields[i];
        if (f.op == FieldOp::Delete) continue;

        CString strDisplayName = f.strName;
        if (f.op == FieldOp::Add)    strDisplayName += _T(" (Mới)");
        if (f.op == FieldOp::Modify) strDisplayName += _T(" (Đã sửa)");

        int nItem = m_listFields.InsertItem((int)i, strDisplayName);
        m_listFields.SetItemText(nItem, 1, f.strDataType);
        m_listFields.SetItemText(nItem, 2, f.bNotNull ? _T("Có") : _T("Không"));
        m_listFields.SetItemText(nItem, 3, f.bPrimaryKey ? _T("✓") : _T(""));
        m_listFields.SetItemData(nItem, (DWORD_PTR)i);
    }
}
// CDlgEditTable message handlers

void CDlgEditTable::OnClickedBtnAddField()
{
	// TODO: Add your control notification handler code here
    CString strName, strType;
    GetDlgItemText(IDC_EDIT_FIELDNAME, strName);
    m_cmbDataType.GetWindowText(strType);
    strName.Trim();

    if (strName.IsEmpty() || strType.IsEmpty()) {
        AfxMessageBox(_T("Vui lòng nhập tên trường và chọn kiểu dữ liệu!"), MB_ICONERROR);
        return;
    }
    for (auto& f : m_vecFields) {
        if (f.op != FieldOp::Delete && f.strName.CompareNoCase(strName) == 0) {
            AfxMessageBox(_T("Trường này đã tồn tại!"), MB_ICONERROR);
            return;
        }
    }

    bool bNN = (((CButton*)GetDlgItem(IDC_CHK_NOTNULL))->GetCheck() == BST_CHECKED);
    m_vecFields.push_back({ _T(""), strName, strType, false, bNN, FieldOp::Add });
    RefreshListControl();
    SetDlgItemText(IDC_EDIT_FIELDNAME, _T(""));

}

void CDlgEditTable::OnClickedBtnDelField()
{
	// TODO: Add your control notification handler code here
    POSITION pos = m_listFields.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(_T("Vui lòng chọn 1 trường để xóa!"), MB_ICONWARNING); return; }
    int nSelected = m_listFields.GetNextSelectedItem(pos);
    size_t nIndex = (size_t)m_listFields.GetItemData(nSelected);
    FieldInfo& f = m_vecFields[nIndex];

    if (f.strOldName.CompareNoCase(_T("ID")) == 0) {
        AfxMessageBox(_T("Không được xóa cột khóa chính ID!"), MB_ICONERROR);
        return;
    }

    CString strMsg;
    strMsg.Format(_T("Bạn có chắc chắn muốn xóa cột '%s' khỏi bảng?"), f.strName);
    if (AfxMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

    if (f.op == FieldOp::Add) {
        m_vecFields.erase(m_vecFields.begin() + nIndex);
    }
    else {
        f.op = FieldOp::Delete;
    }
    RefreshListControl();
}

void CDlgEditTable::OnClickedBtnEditField()
{
	// TODO: Add your control notification handler code here
    POSITION pos = m_listFields.GetFirstSelectedItemPosition();
    if (!pos) { AfxMessageBox(_T("Vui lòng chọn đúng 1 trường để sửa!"), MB_ICONWARNING); return; }
    int nSelected = m_listFields.GetNextSelectedItem(pos);
    size_t nIndex = (size_t)m_listFields.GetItemData(nSelected);
    FieldInfo& f = m_vecFields[nIndex];

    if (f.strOldName.CompareNoCase(_T("ID")) == 0) {
        AfxMessageBox(_T("Không được sửa cột khóa chính ID!"), MB_ICONERROR);
        return;
    }

    CString strNewName, strNewType;
    GetDlgItemText(IDC_EDIT_FIELDNAME, strNewName);
    m_cmbDataType.GetWindowText(strNewType);
    strNewName.Trim();
    if (strNewName.IsEmpty() || strNewType.IsEmpty()) {
        AfxMessageBox(_T("Vui lòng nhập tên trường mới và chọn kiểu dữ liệu!"), MB_ICONERROR);
        return;
    }

    f.strName = strNewName;
    f.strDataType = strNewType;
    f.bNotNull = (((CButton*)GetDlgItem(IDC_CHK_NOTNULL))->GetCheck() == BST_CHECKED);
    if (f.op == FieldOp::Existing) f.op = FieldOp::Modify;
    RefreshListControl();
}
std::vector<CString> CDlgEditTable::BuildAlterStatements() {
    std::vector<CString> vecSQL;
    for (const auto& f : m_vecFields) {
        CString strSQL;
        switch (f.op) {
        case FieldOp::Add:
            strSQL.Format(_T("ALTER TABLE %s ADD COLUMN %s %s%s"),
                m_strTableName, f.strName, f.strDataType, f.bNotNull ? _T(" NOT NULL") : _T(""));
            vecSQL.push_back(strSQL);
            break;
        case FieldOp::Modify:
            strSQL.Format(_T("ALTER TABLE %s CHANGE COLUMN %s %s %s%s"),
                m_strTableName, f.strOldName, f.strName, f.strDataType, f.bNotNull ? _T(" NOT NULL") : _T(""));
            vecSQL.push_back(strSQL);
            break;
        case FieldOp::Delete:
            strSQL.Format(_T("ALTER TABLE %s DROP COLUMN %s"), m_strTableName, f.strOldName);
            vecSQL.push_back(strSQL);
            break;
        default: break;
        }
    }
    return vecSQL;
}

void CDlgEditTable::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
    std::vector<CString> vecSQL = BuildAlterStatements();
    if (vecSQL.empty()) {
        AfxMessageBox(_T("Không có thay đổi nào để áp dụng."), MB_ICONINFORMATION);
        CDialogEx::OnOK();
        return;
    }

    CDBManager& db = CDBManager::GetInstance();
    int nSuccess = 0;
    for (const auto& sql : vecSQL) if (db.ExecuteQuery(sql)) nSuccess++;

    if (nSuccess == (int)vecSQL.size()) {
        AfxMessageBox(_T("Cập nhật cấu trúc bảng thành công!"), MB_ICONINFORMATION);
        CDialogEx::OnOK();
    }
    else {
        CString strMsg;
        strMsg.Format(_T("Chỉ %d/%d thay đổi thành công. Vui lòng kiểm tra lại."), nSuccess, (int)vecSQL.size());
        AfxMessageBox(strMsg, MB_ICONWARNING);
    }
}
