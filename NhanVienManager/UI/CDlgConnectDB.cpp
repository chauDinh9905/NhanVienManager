// CDlgConnectDB.cpp
#include "pch.h"
#include "resource.h"
#include "UI/CDlgConnectDB.h"
#include "UI/CDlgTableSchema.h"
#include "UI/UiHelpers.h"
#include "Data/SqlUtil.h"
#include "Models/NhanVienSchema.h"
#include "Common/AppConstants.h"
#include "Common/AppSettings.h"
#include "Common/StringUtil.h"
#include "Localization/LanguageManager.h"

IMPLEMENT_DYNAMIC(CDlgConnectDB, CBaseDialog)

CDlgConnectDB::CDlgConnectDB(CWnd* pParent)
    : CBaseDialog(IDD_DLG_CONNECT_DB, pParent)
{
}

BEGIN_MESSAGE_MAP(CDlgConnectDB, CBaseDialog)
    ON_BN_CLICKED(IDC_BTN_CHECK, &CDlgConnectDB::OnClickedBtnCheck)
    ON_BN_CLICKED(IDC_BTN_CREATE_TABLE, &CDlgConnectDB::OnClickedBtnCreateTable)
    ON_BN_CLICKED(IDC_BTN_EDIT_TABLE, &CDlgConnectDB::OnClickedBtnEditTable)
    ON_EN_CHANGE(IDC_EDIT_DBNAME, &CDlgConnectDB::OnChangeNames)
    ON_EN_CHANGE(IDC_EDIT_TABLENAME, &CDlgConnectDB::OnChangeNames)
END_MESSAGE_MAP()

BOOL CDlgConnectDB::OnInitControls()
{
    CEdit* pEditDb = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_DBNAME));
    CEdit* pEditTable = static_cast<CEdit*>(GetDlgItem(IDC_EDIT_TABLENAME));
    pEditDb->LimitText(AppConst::MAX_IDENTIFIER_LENGTH);
    pEditTable->LimitText(AppConst::MAX_IDENTIFIER_LENGTH);

    SetDlgItemText(IDC_EDIT_DBNAME, AppSettings::GetString(SettingKey::DB_NAME, _T("")));
    SetDlgItemText(IDC_EDIT_TABLENAME, AppSettings::GetString(SettingKey::TABLE_NAME, AppConst::DEFAULT_TABLE));
    SetState(TableState::Unknown);

    GotoDlgCtrl(pEditDb);
    return FALSE;
}

void CDlgConnectDB::SetupLayout()
{
    static const LayoutItem kLayout[] =
    {
        //  ID                   MoveX MoveY SizeX SizeY
        { IDC_EDIT_DBNAME,          0,    0,  100,    0 },
        { IDC_EDIT_TABLENAME,       0,    0,  100,    0 },
        { IDC_BTN_CREATE_TABLE,     0,  100,    0,    0 },
        { IDC_BTN_EDIT_TABLE,       0,  100,    0,    0 },
        { IDOK,                   100,  100,    0,    0 },
        { IDCANCEL,               100,  100,    0,    0 },
    };
    ApplyLayout(kLayout);
}

void CDlgConnectDB::ApplyLanguage()
{
    static const CtrlTextItem kTexts[] =
    {
        { IDC_LBL_DBNAME,       K::LBL_DBNAME       },
        { IDC_LBL_TABLENAME,    K::LBL_TABLENAME    },
        { IDC_BTN_CHECK,        K::BTN_CHECK        },
        { IDC_BTN_CREATE_TABLE, K::BTN_CREATE_TABLE },
        { IDC_BTN_EDIT_TABLE,   K::BTN_EDIT_TABLE   },
        { IDOK,                 K::BTN_OPEN         },
        { IDCANCEL,             K::BTN_CANCEL       },
    };
    SetWindowText(Tr(K::TITLE_CONNECT_DB));
    ApplyTexts(kTexts);
}

void CDlgConnectDB::SetState(TableState state)
{
    m_state = state;
    GetDlgItem(IDC_BTN_CREATE_TABLE)->EnableWindow(state == TableState::NotExists);
    GetDlgItem(IDC_BTN_EDIT_TABLE)->EnableWindow(state == TableState::Invalid || state == TableState::Ready);
}

// Người dùng sửa tên DB / bảng -> kết quả kiểm tra cũ không còn đúng nữa
void CDlgConnectDB::OnChangeNames()
{
    SetState(TableState::Unknown);
}

bool CDlgConnectDB::ReadInputs()
{
    GetDlgItemText(IDC_EDIT_DBNAME, m_strDbName);
    GetDlgItemText(IDC_EDIT_TABLENAME, m_strTableName);
    m_strDbName.Trim();
    m_strTableName.Trim();

    if (m_strDbName.IsEmpty() || m_strTableName.IsEmpty())
    {
        Ui::ShowWarning(K::ERR_DB_TABLE_EMPTY);
        return false;
    }
    if (!SqlUtil::IsValidIdentifier(m_strDbName))
    {
        Ui::ShowWarning(K::ERR_INVALID_IDENTIFIER);
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_DBNAME));
        return false;
    }
    if (!SqlUtil::IsValidIdentifier(m_strTableName))
    {
        Ui::ShowWarning(K::ERR_INVALID_IDENTIFIER);
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_TABLENAME));
        return false;
    }
    return true;
}

// Trả về true nếu bảng đã sẵn sàng để làm việc.
bool CDlgConnectDB::CheckTable(bool bShowReadyMessage)
{
    if (!ReadInputs()) return false;

    if (!m_schemaRepo.EnsureDatabase(m_strDbName))
    {
        SetState(TableState::Unknown);
        Ui::ShowError(K::ERR_OPEN_DB_FAIL, m_schemaRepo.GetLastError());
        return false;
    }

    bool bExists = false;
    if (!m_schemaRepo.TableExists(m_strTableName, bExists))
    {
        SetState(TableState::Unknown);
        Ui::ShowError(K::ERR_LOAD_SCHEMA_FAIL, m_schemaRepo.GetLastError());
        return false;
    }
    if (!bExists)
    {
        SetState(TableState::NotExists);
        Ui::ShowInfo(K::MSG_TABLE_NOT_EXISTS);
        return false;
    }

    std::vector<FieldInfo> columns;
    if (!m_schemaRepo.LoadColumns(m_strTableName, columns))
    {
        SetState(TableState::Unknown);
        Ui::ShowError(K::ERR_LOAD_SCHEMA_FAIL, m_schemaRepo.GetLastError());
        return false;
    }

    const std::vector<CString> missing = NvSchema::FindMissingColumns(columns);
    if (!missing.empty())
    {
        SetState(TableState::Invalid);
        const CString strMessage = CLanguageManager::GetInstance().Format(K::MSG_TABLE_MISSING_COLUMNS,
            static_cast<LPCTSTR>(JoinStrings(missing, _T(", "))));
        AfxMessageBox(strMessage, MB_OK | MB_ICONWARNING);
        return false;
    }

    SetState(TableState::Ready);
    if (bShowReadyMessage) Ui::ShowInfo(K::MSG_TABLE_READY);
    return true;
}

void CDlgConnectDB::OnClickedBtnCheck()
{
    CheckTable(true);
}

void CDlgConnectDB::OnClickedBtnCreateTable()
{
    CDlgTableSchema dlg(SchemaMode::Create, m_strTableName, this);
    if (dlg.DoModal() == IDOK) CheckTable(true);
}

void CDlgConnectDB::OnClickedBtnEditTable()
{
    CDlgTableSchema dlg(SchemaMode::Edit, m_strTableName, this);
    if (dlg.DoModal() == IDOK) CheckTable(true);
}

// Nút "Mở" (và phím Enter): chưa kiểm tra thì tự kiểm tra, sẵn sàng thì vào màn hình chính.
void CDlgConnectDB::OnOK()
{
    if (m_state != TableState::Ready && !CheckTable(false)) return;

    AppSettings::SetString(SettingKey::DB_NAME, m_strDbName);
    AppSettings::SetString(SettingKey::TABLE_NAME, m_strTableName);
    EndDialog(IDOK);
}