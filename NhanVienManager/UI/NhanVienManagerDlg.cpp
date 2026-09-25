// NhanVienManagerDlg.cpp
#include "pch.h"
#include "resource.h"
#include "UI/NhanVienManagerDlg.h"
#include "UI/CDlgNhanVienInfo.h"
#include "UI/UiHelpers.h"
#include "Common/DateUtil.h"
#include "Localization/LanguageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CNhanVienManagerDlg, CBaseDialog)

CNhanVienManagerDlg::CNhanVienManagerDlg(const CString& strTableName, CWnd* pParent)
    : CBaseDialog(IDD_NHANVIENMANAGER_DIALOG, pParent)
    , m_repo(strTableName)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNhanVienManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_NHANVIEN, m_listNhanVien);
    DDX_Control(pDX, IDC_CMB_LANGUAGE, m_cmbLanguage);
}

BEGIN_MESSAGE_MAP(CNhanVienManagerDlg, CBaseDialog)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_ADD, &CNhanVienManagerDlg::OnClickedBtnAdd)
    ON_BN_CLICKED(IDC_BTN_EDIT, &CNhanVienManagerDlg::OnClickedBtnEdit)
    ON_BN_CLICKED(IDC_BTN_DELETE, &CNhanVienManagerDlg::OnClickedBtnDelete)
    ON_CBN_SELCHANGE(IDC_CMB_LANGUAGE, &CNhanVienManagerDlg::OnSelchangeCmbLanguage)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_NHANVIEN, &CNhanVienManagerDlg::OnDblclkListNhanVien)
END_MESSAGE_MAP()

BOOL CNhanVienManagerDlg::OnInitControls()
{
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_listNhanVien.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

    // Các cột sinh từ bảng mô tả -> thêm cột trong NvSchema là ListView tự có cột mới.
    // Tiêu đề cột được gán trong ApplyLanguage().
    for (int i = 0; i < NvSchema::kColumnCount; ++i)
    {
        const NvColumnDef& col = NvSchema::kColumns[i];
        m_listNhanVien.InsertColumn(i, _T(""), col.nAlign, col.nWidth);
    }

    Ui::FillLanguageCombo(m_cmbLanguage);
    ReloadData();
    return TRUE;
}

void CNhanVienManagerDlg::SetupLayout()
{
    static const LayoutItem kLayout[] =
    {
        //  ID                 MoveX MoveY SizeX SizeY
        { IDC_LBL_LANGUAGE,     100,    0,    0,    0 },
        { IDC_CMB_LANGUAGE,     100,    0,    0,    0 },
        { IDC_LIST_NHANVIEN,      0,    0,  100,  100 },
        { IDC_BTN_ADD,          100,  100,    0,    0 },
        { IDC_BTN_EDIT,         100,  100,    0,    0 },
        { IDC_BTN_DELETE,       100,  100,    0,    0 },
    };
    ApplyLayout(kLayout);
}

void CNhanVienManagerDlg::ApplyLanguage()
{
    static const CtrlTextItem kTexts[] =
    {
        { IDC_LBL_LANGUAGE, K::LBL_LANGUAGE },
        { IDC_BTN_ADD,      K::BTN_ADD      },
        { IDC_BTN_EDIT,     K::BTN_EDIT     },
        { IDC_BTN_DELETE,   K::BTN_DELETE   },
    };
    SetWindowText(Tr(K::TITLE_MAIN));
    ApplyTexts(kTexts);

    for (int i = 0; i < NvSchema::kColumnCount; ++i)
    {
        Ui::SetListColumnText(m_listNhanVien, i, Tr(NvSchema::kColumns[i].headerKey));
    }

    // Giới tính và định dạng ngày phụ thuộc ngôn ngữ -> vẽ lại danh sách (không cần đọc lại DB)
    RefreshListView();
}

void CNhanVienManagerDlg::OnSelchangeCmbLanguage()
{
    if (Ui::ChangeLanguageFromCombo(m_cmbLanguage))
    {
        ApplyLanguage();
    }
}

void CNhanVienManagerDlg::ReloadData()
{
    std::vector<NhanVienInfo> data;
    bool bSuccess = false;
    {
        CWaitCursor waitCursor;
        bSuccess = m_repo.GetAll(data);
    }
    if (!bSuccess)
    {
        Ui::ShowError(K::MSG_LOAD_FAIL, m_repo.GetLastError());
        return;
    }
    m_data.swap(data);
    RefreshListView();
}

// Chữ hiển thị của 1 ô. Dữ liệu gốc (0/1, "YYYY-MM-DD") vẫn nằm nguyên trong m_data.
CString CNhanVienManagerDlg::GetDisplayText(const NhanVienInfo& nv, const NvColumnDef& col) const
{
    switch (col.kind)
    {
    case ColKind::Gender:
        return Tr(nv.gioiTinh == GioiTinh::Nam ? K::GENDER_MALE : K::GENDER_FEMALE);
    case ColKind::Date:
        return FormatIsoDate(NvSchema::GetFieldText(nv, col.field), Tr(K::FMT_DATE_DISPLAY));
    default:
        return NvSchema::GetFieldText(nv, col.field);
    }
}

void CNhanVienManagerDlg::RefreshListView()
{
    m_listNhanVien.SetRedraw(FALSE);
    m_listNhanVien.DeleteAllItems();

    for (size_t row = 0; row < m_data.size(); ++row)
    {
        const NhanVienInfo& nv = m_data[row];
        const int nItem = m_listNhanVien.InsertItem(static_cast<int>(row),
            GetDisplayText(nv, NvSchema::kColumns[0]));
        for (int col = 1; col < NvSchema::kColumnCount; ++col)
        {
            m_listNhanVien.SetItemText(nItem, col, GetDisplayText(nv, NvSchema::kColumns[col]));
        }
        m_listNhanVien.SetItemData(nItem, static_cast<DWORD_PTR>(row));
    }

    m_listNhanVien.SetRedraw(TRUE);
    m_listNhanVien.Invalidate();
}

std::vector<size_t> CNhanVienManagerDlg::GetSelectedIndexes() const
{
    std::vector<size_t> indexes;
    POSITION pos = m_listNhanVien.GetFirstSelectedItemPosition();
    while (pos != nullptr)
    {
        const int nItem = m_listNhanVien.GetNextSelectedItem(pos);
        indexes.push_back(static_cast<size_t>(m_listNhanVien.GetItemData(nItem)));
    }
    return indexes;
}

void CNhanVienManagerDlg::OnClickedBtnAdd()
{
    CDlgNhanVienInfo dlg(NhanVienInfo(), false, this);
    if (dlg.DoModal() != IDOK) return;

    if (!m_repo.Insert(dlg.GetNhanVien()))
    {
        Ui::ShowError(K::MSG_ADD_FAIL, m_repo.GetLastError());
        return;
    }
    Ui::ShowInfo(K::MSG_ADD_SUCCESS);
    ReloadData();
}

void CNhanVienManagerDlg::OnClickedBtnEdit()
{
    const std::vector<size_t> selected = GetSelectedIndexes();
    if (selected.size() != 1)
    {
        Ui::ShowWarning(K::ERR_SELECT_ONE);
        return;
    }

    // Lấy dữ liệu GỐC từ m_data, không đọc lại chữ hiển thị trên ListView
    CDlgNhanVienInfo dlg(m_data[selected[0]], true, this);
    if (dlg.DoModal() != IDOK) return;

    if (!m_repo.Update(dlg.GetNhanVien()))
    {
        Ui::ShowError(K::MSG_EDIT_FAIL, m_repo.GetLastError());
        return;
    }
    Ui::ShowInfo(K::MSG_EDIT_SUCCESS);
    ReloadData();
}

void CNhanVienManagerDlg::OnClickedBtnDelete()
{
    const std::vector<size_t> selected = GetSelectedIndexes();
    if (selected.empty())
    {
        Ui::ShowWarning(K::ERR_SELECT_NONE_DELETE);
        return;
    }

    const CString strConfirm = CLanguageManager::GetInstance().Format(K::CONFIRM_DELETE,
        static_cast<int>(selected.size()));
    if (!Ui::Confirm(strConfirm)) return;

    std::vector<int> ids;
    for (size_t index : selected) ids.push_back(m_data[index].nID);

    if (!m_repo.DeleteByIds(ids))
    {
        Ui::ShowError(K::MSG_DELETE_FAIL, m_repo.GetLastError());
        return;
    }
    Ui::ShowInfo(K::MSG_DELETE_SUCCESS);
    ReloadData();
}

void CNhanVienManagerDlg::OnDblclkListNhanVien(NMHDR* pNMHDR, LRESULT* pResult)
{
    const NMITEMACTIVATE* pInfo = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
    if (pInfo->iItem >= 0) OnClickedBtnEdit();
    *pResult = 0;
}

void CNhanVienManagerDlg::OnOK()
{
    // Cố ý để trống: nhấn Enter không được đóng chương trình.
}

void CNhanVienManagerDlg::OnCancel()
{
    // Cố ý để trống: nhấn Esc không được đóng chương trình.
}

// Nút X trên thanh tiêu đề -> đóng chương trình
void CNhanVienManagerDlg::OnClose()
{
    EndDialog(IDCANCEL);
}