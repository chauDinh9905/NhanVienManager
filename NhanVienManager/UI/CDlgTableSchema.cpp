// CDlgTableSchema.cpp
#include "pch.h"
#include "resource.h"
#include "UI/CDlgTableSchema.h"
#include "UI/UiHelpers.h"
#include "Data/SqlUtil.h"
#include "Models/DataTypes.h"
#include "Models/NhanVienSchema.h"
#include "Common/AppConstants.h"
#include "Common/StringUtil.h"
#include "Localization/LanguageManager.h"

namespace
{
    // Các cột của ListView danh sách trường. Thứ tự ở đây = chỉ số cột (LC_...).
    struct ListColumn
    {
        LPCTSTR pszKey;
        int     nWidth;
        int     nAlign;
    };

    constexpr ListColumn kListColumns[] =
    {
        { K::COL_FIELDNAME,  140, LVCFMT_LEFT   },
        { K::COL_DATATYPE,   120, LVCFMT_LEFT   },
        { K::COL_NOTNULL,     80, LVCFMT_CENTER },
        { K::COL_PRIMARYKEY,  80, LVCFMT_CENTER },
        { K::COL_STATUS,     130, LVCFMT_LEFT   },
    };

    enum ListColumnIndex { LC_NAME, LC_TYPE, LC_NOTNULL, LC_PK, LC_STATUS, LC_COUNT };
    static_assert(static_cast<size_t>(LC_COUNT) == std::size(kListColumns), "kListColumns va ListColumnIndex phai khop nhau");
}

IMPLEMENT_DYNAMIC(CDlgTableSchema, CBaseDialog)

CDlgTableSchema::CDlgTableSchema(SchemaMode mode, const CString& strTableName, CWnd* pParent)
    : CBaseDialog(IDD_DLG_TABLE_SCHEMA, pParent)
    , m_mode(mode)
    , m_strTableName(strTableName)
{
}

void CDlgTableSchema::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_FIELDS, m_listFields);
    DDX_Control(pDX, IDC_CMB_DATATYPE, m_cmbDataType);
}

BEGIN_MESSAGE_MAP(CDlgTableSchema, CBaseDialog)
    ON_BN_CLICKED(IDC_BTN_ADD_FIELD, &CDlgTableSchema::OnClickedBtnAddField)
    ON_BN_CLICKED(IDC_BTN_EDIT_FIELD, &CDlgTableSchema::OnClickedBtnEditField)
    ON_BN_CLICKED(IDC_BTN_DEL_FIELD, &CDlgTableSchema::OnClickedBtnDelField)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FIELDS, &CDlgTableSchema::OnItemChangedListFields)
END_MESSAGE_MAP()

BOOL CDlgTableSchema::OnInitControls()
{
    SetDlgItemText(IDC_EDIT_TABLENAME, m_strTableName);

    m_listFields.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
    for (int i = 0; i < LC_COUNT; ++i)
    {
        m_listFields.InsertColumn(i, _T(""), kListColumns[i].nAlign, kListColumns[i].nWidth);
    }

    for (LPCTSTR pszType : kAllowedDataTypes)
    {
        m_cmbDataType.AddString(pszType);
    }

    static_cast<CEdit*>(GetDlgItem(IDC_EDIT_FIELDNAME))->LimitText(AppConst::MAX_IDENTIFIER_LENGTH);

    // Khóa chính luôn là cột ID tự tăng của chương trình -> không cho người dùng đổi.
    GetDlgItem(IDC_CHK_PK)->EnableWindow(FALSE);

    if (!LoadFields())
    {
        EndDialog(IDCANCEL);
        return TRUE;
    }
    // Danh sách sẽ được vẽ trong ApplyLanguage() (được gọi ngay sau hàm này)
    return TRUE;
}

bool CDlgTableSchema::LoadFields()
{
    if (m_mode == SchemaMode::Create)
    {
        // Bảng mới: có sẵn toàn bộ cột chương trình cần (bị khóa), người dùng thêm cột khác nếu muốn.
        m_fields = NvSchema::BuildRequiredFields();
        return true;
    }

    if (!m_schemaRepo.LoadColumns(m_strTableName, m_fields))
    {
        Ui::ShowError(K::ERR_LOAD_SCHEMA_FAIL, m_schemaRepo.GetLastError());
        return false;
    }

    // Cột chương trình cần và cột khóa chính: không cho sửa / xóa.
    for (auto& f : m_fields)
    {
        f.bLocked = f.bPrimaryKey || NvSchema::IsRequiredColumn(f.strName);
    }

    // Bảng cũ thiếu cột bắt buộc -> tự thêm vào danh sách (trạng thái "Mới"), bấm OK là bổ sung.
    const std::vector<CString> missing = NvSchema::FindMissingColumns(m_fields);
    for (const FieldInfo& required : NvSchema::BuildRequiredFields())
    {
        for (const CString& strMissing : missing)
        {
            if (strMissing.CompareNoCase(required.strName) == 0) m_fields.push_back(required);
        }
    }
    return true;
}

void CDlgTableSchema::SetupLayout()
{
    static const LayoutItem kLayout[] =
    {
        //  ID                  MoveX MoveY SizeX SizeY
        { IDC_EDIT_TABLENAME,      0,    0,  100,    0 },
        { IDC_LIST_FIELDS,         0,    0,  100,  100 },
        { IDC_LBL_FIELDNAME,       0,  100,    0,    0 },
        { IDC_EDIT_FIELDNAME,      0,  100,   50,    0 },
        { IDC_LBL_DATATYPE,       50,  100,    0,    0 },
        { IDC_CMB_DATATYPE,       50,  100,   50,    0 },
        { IDC_CHK_NOTNULL,         0,  100,    0,    0 },
        { IDC_CHK_PK,              0,  100,    0,    0 },
        { IDC_BTN_ADD_FIELD,       0,  100,    0,    0 },
        { IDC_BTN_EDIT_FIELD,      0,  100,    0,    0 },
        { IDC_BTN_DEL_FIELD,       0,  100,    0,    0 },
        { IDOK,                  100,  100,    0,    0 },
        { IDCANCEL,              100,  100,    0,    0 },
    };
    ApplyLayout(kLayout);
}

void CDlgTableSchema::ApplyLanguage()
{
    static const CtrlTextItem kTexts[] =
    {
        { IDC_LBL_TABLENAME,  K::LBL_TABLENAME  },
        { IDC_LBL_FIELDNAME,  K::LBL_FIELDNAME  },
        { IDC_LBL_DATATYPE,   K::LBL_DATATYPE   },
        { IDC_CHK_NOTNULL,    K::CHK_NOTNULL    },
        { IDC_CHK_PK,         K::CHK_PK         },
        { IDC_BTN_ADD_FIELD,  K::BTN_ADD_FIELD  },
        { IDC_BTN_EDIT_FIELD, K::BTN_EDIT_FIELD },
        { IDC_BTN_DEL_FIELD,  K::BTN_DEL_FIELD  },
        { IDOK,               K::BTN_OK         },
        { IDCANCEL,           K::BTN_CANCEL     },
    };
    SetWindowText(Tr(m_mode == SchemaMode::Create ? K::TITLE_CREATE_TABLE : K::TITLE_EDIT_TABLE));
    ApplyTexts(kTexts);

    for (int i = 0; i < LC_COUNT; ++i)
    {
        Ui::SetListColumnText(m_listFields, i, Tr(kListColumns[i].pszKey));
    }
    RefreshList();      // chữ "Có/Không", "Mới/Đã sửa" cũng phải dịch lại
}

CString CDlgTableSchema::GetStatusText(const FieldInfo& field) const
{
    std::vector<CString> parts;
    if (field.op == FieldOp::Add)    parts.push_back(Tr(K::TAG_NEW));
    if (field.op == FieldOp::Modify) parts.push_back(Tr(K::TAG_MODIFIED));
    if (field.bLocked)               parts.push_back(Tr(K::TAG_REQUIRED));
    return JoinStrings(parts, _T(", "));
}

void CDlgTableSchema::RefreshList()
{
    m_listFields.SetRedraw(FALSE);
    m_listFields.DeleteAllItems();

    int nRow = 0;
    for (size_t i = 0; i < m_fields.size(); ++i)
    {
        const FieldInfo& f = m_fields[i];
        if (f.op == FieldOp::Delete) continue;      // cột sắp xóa không hiển thị nữa

        const int nItem = m_listFields.InsertItem(nRow++, f.strName);
        m_listFields.SetItemText(nItem, LC_TYPE, f.strDataType);
        m_listFields.SetItemText(nItem, LC_NOTNULL, Tr(f.bNotNull ? K::VAL_YES : K::VAL_NO));
        m_listFields.SetItemText(nItem, LC_PK, f.bPrimaryKey ? Tr(K::VAL_YES) : CString());
        m_listFields.SetItemText(nItem, LC_STATUS, GetStatusText(f));
        // ItemData = vị trí trong m_fields (dòng trên list KHÔNG trùng vị trí vì cột bị xóa đã ẩn)
        m_listFields.SetItemData(nItem, static_cast<DWORD_PTR>(i));
    }

    m_listFields.SetRedraw(TRUE);
    m_listFields.Invalidate();
}

int CDlgTableSchema::GetSelectedFieldIndex() const
{
    POSITION pos = m_listFields.GetFirstSelectedItemPosition();
    if (pos == nullptr) return -1;
    const int nItem = m_listFields.GetNextSelectedItem(pos);
    return static_cast<int>(m_listFields.GetItemData(nItem));
}

// Chọn 1 dòng -> đưa thông tin lên các ô nhập bên dưới để sửa cho tiện.
void CDlgTableSchema::OnItemChangedListFields(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 0;
    const NMLISTVIEW* pInfo = reinterpret_cast<NMLISTVIEW*>(pNMHDR);
    if (pInfo->iItem < 0) return;
    if ((pInfo->uChanged & LVIF_STATE) == 0 || (pInfo->uNewState & LVIS_SELECTED) == 0) return;

    const size_t nIndex = static_cast<size_t>(m_listFields.GetItemData(pInfo->iItem));
    if (nIndex >= m_fields.size()) return;
    const FieldInfo& f = m_fields[nIndex];

    SetDlgItemText(IDC_EDIT_FIELDNAME, f.strName);
    m_cmbDataType.SetCurSel(m_cmbDataType.FindStringExact(-1, f.strDataType));   // không có trong list -> bỏ chọn
    CheckDlgButton(IDC_CHK_NOTNULL, f.bNotNull ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHK_PK, f.bPrimaryKey ? BST_CHECKED : BST_UNCHECKED);
}

bool CDlgTableSchema::IsDuplicateName(const CString& strName, int nIgnoreIndex) const
{
    for (size_t i = 0; i < m_fields.size(); ++i)
    {
        if (static_cast<int>(i) == nIgnoreIndex) continue;
        if (m_fields[i].op == FieldOp::Delete) continue;
        if (m_fields[i].strName.CompareNoCase(strName) == 0) return true;
    }
    return false;
}

// Đọc và kiểm tra các ô nhập. pCurrent != nullptr khi đang sửa 1 trường có sẵn.
bool CDlgTableSchema::ReadEditor(CString& strName, CString& strType, bool& bNotNull, const FieldInfo* pCurrent)
{
    GetDlgItemText(IDC_EDIT_FIELDNAME, strName);
    strName.Trim();
    if (strName.IsEmpty())
    {
        Ui::ShowWarning(K::ERR_FIELDNAME_EMPTY);
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_FIELDNAME));
        return false;
    }
    if (!SqlUtil::IsValidIdentifier(strName))
    {
        Ui::ShowWarning(K::ERR_INVALID_IDENTIFIER);
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_FIELDNAME));
        return false;
    }

    const int nSel = m_cmbDataType.GetCurSel();
    if (nSel != CB_ERR)
    {
        m_cmbDataType.GetLBText(nSel, strType);
    }
    else if (pCurrent != nullptr)
    {
        strType = pCurrent->strDataType;    // không chọn kiểu mới -> giữ nguyên kiểu cũ
    }
    else
    {
        Ui::ShowWarning(K::ERR_SELECT_TYPE);
        GotoDlgCtrl(&m_cmbDataType);
        return false;
    }

    bNotNull = (IsDlgButtonChecked(IDC_CHK_NOTNULL) == BST_CHECKED);
    return true;
}

void CDlgTableSchema::OnClickedBtnAddField()
{
    CString strName, strType;
    bool bNotNull = false;
    if (!ReadEditor(strName, strType, bNotNull, nullptr)) return;

    if (IsDuplicateName(strName, -1))
    {
        Ui::ShowWarning(K::ERR_FIELDNAME_DUP);
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_FIELDNAME));
        return;
    }

    FieldInfo field;
    field.strName = strName;
    field.strDataType = strType;
    field.bNotNull = bNotNull;
    field.op = FieldOp::Add;
    m_fields.push_back(field);

    RefreshList();
    SetDlgItemText(IDC_EDIT_FIELDNAME, _T(""));
    GotoDlgCtrl(GetDlgItem(IDC_EDIT_FIELDNAME));
}

void CDlgTableSchema::OnClickedBtnEditField()
{
    const int nIndex = GetSelectedFieldIndex();
    if (nIndex < 0)
    {
        Ui::ShowWarning(K::ERR_SELECT_FIELD);
        return;
    }

    FieldInfo& field = m_fields[static_cast<size_t>(nIndex)];
    if (field.bLocked)
    {
        Ui::ShowWarning(K::ERR_FIELD_LOCKED);
        return;
    }

    CString strName, strType;
    bool bNotNull = false;
    if (!ReadEditor(strName, strType, bNotNull, &field)) return;

    if (IsDuplicateName(strName, nIndex))
    {
        Ui::ShowWarning(K::ERR_FIELDNAME_DUP);
        return;
    }

    field.strName = strName;
    field.strDataType = strType;
    field.bNotNull = bNotNull;
    if (field.op == FieldOp::Existing) field.op = FieldOp::Modify;

    RefreshList();
}

void CDlgTableSchema::OnClickedBtnDelField()
{
    const int nIndex = GetSelectedFieldIndex();
    if (nIndex < 0)
    {
        Ui::ShowWarning(K::ERR_SELECT_FIELD);
        return;
    }

    FieldInfo& field = m_fields[static_cast<size_t>(nIndex)];
    if (field.bLocked)
    {
        Ui::ShowWarning(K::ERR_FIELD_LOCKED);
        return;
    }

    const CString strMessage = CLanguageManager::GetInstance().Format(K::CONFIRM_DELETE_FIELD,
        static_cast<LPCTSTR>(field.strName));
    if (!Ui::Confirm(strMessage)) return;

    if (field.op == FieldOp::Add)
        m_fields.erase(m_fields.begin() + nIndex);  // cột chưa có trong DB -> bỏ khỏi danh sách
    else
        field.op = FieldOp::Delete;                 // cột có trong DB -> đánh dấu xóa

    RefreshList();
}

void CDlgTableSchema::OnOK()
{
    // Đang gõ tên trường mà nhấn Enter -> hiểu là "Thêm trường", KHÔNG phải tạo / lưu bảng.
    if (::GetFocus() == GetDlgItem(IDC_EDIT_FIELDNAME)->GetSafeHwnd())
    {
        OnClickedBtnAddField();
        return;
    }

    bool bSuccess = false;
    {
        CWaitCursor waitCursor;
        if (m_mode == SchemaMode::Create)
        {
            bSuccess = m_schemaRepo.CreateTable(m_strTableName, m_fields);
        }
        else
        {
            if (!CSchemaRepository::HasChanges(m_fields))
            {
                Ui::ShowInfo(K::MSG_ALTER_NOTHING);
                EndDialog(IDOK);
                return;
            }
            bSuccess = m_schemaRepo.ApplyChanges(m_strTableName, m_fields);
        }
    }

    if (!bSuccess)
    {
        // Giữ dialog mở để người dùng sửa lại; do dùng 1 câu lệnh nên DB chưa bị đổi gì.
        Ui::ShowError(m_mode == SchemaMode::Create ? K::MSG_CREATE_TABLE_FAIL : K::MSG_ALTER_FAIL,
            m_schemaRepo.GetLastError());
        return;
    }

    Ui::ShowInfo(m_mode == SchemaMode::Create ? K::MSG_CREATE_TABLE_SUCCESS : K::MSG_ALTER_SUCCESS);
    EndDialog(IDOK);
}