// CDlgLogin.cpp
#include "pch.h"
#include "resource.h"
#include "UI/CDlgLogin.h"
#include "UI/UiHelpers.h"
#include "Data/DBManager.h"
#include "Common/AppConstants.h"
#include "Common/AppSettings.h"
#include "Common/StringUtil.h"
#include "Localization/LanguageManager.h"

IMPLEMENT_DYNAMIC(CDlgLogin, CBaseDialog)

CDlgLogin::CDlgLogin(CWnd* pParent)
    : CBaseDialog(IDD_DLG_LOGIN, pParent)
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMB_LANGUAGE, m_cmbLanguage);
}

BEGIN_MESSAGE_MAP(CDlgLogin, CBaseDialog)
    ON_BN_CLICKED(IDC_BTN_CONNECT, &CDlgLogin::OnClickedBtnConnect)
    ON_CBN_SELCHANGE(IDC_CMB_LANGUAGE, &CDlgLogin::OnSelchangeCmbLanguage)
END_MESSAGE_MAP()

BOOL CDlgLogin::OnInitControls()
{
    Ui::FillLanguageCombo(m_cmbLanguage);

    // Điền sẵn thông tin lần đăng nhập trước (KHÔNG bao giờ lưu mật khẩu)
    const CString strUser = AppSettings::GetString(SettingKey::USER, _T(""));
    SetDlgItemText(IDC_EDIT_HOST, AppSettings::GetString(SettingKey::HOST, AppConst::DEFAULT_HOST));
    SetDlgItemText(IDC_EDIT_PORT, IntToString(AppSettings::GetUInt(SettingKey::PORT, AppConst::DEFAULT_PORT)));
    SetDlgItemText(IDC_EDIT_USER, strUser);

    GotoDlgCtrl(GetDlgItem(strUser.IsEmpty() ? IDC_EDIT_USER : IDC_EDIT_PASS));
    return FALSE;   // đã tự đặt focus
}

void CDlgLogin::SetupLayout()
{
    static const LayoutItem kLayout[] =
    {
        //  ID               MoveX MoveY SizeX SizeY
        { IDC_EDIT_HOST,        0,    0,  100,    0 },
        { IDC_EDIT_USER,        0,    0,  100,    0 },
        { IDC_EDIT_PASS,        0,    0,  100,    0 },
        { IDC_BTN_CONNECT,    100,  100,    0,    0 },
        { IDCANCEL,           100,  100,    0,    0 },
    };
    ApplyLayout(kLayout);
}

void CDlgLogin::ApplyLanguage()
{
    static const CtrlTextItem kTexts[] =
    {
        { IDC_LBL_LANGUAGE, K::LBL_LANGUAGE },
        { IDC_LBL_HOST,     K::LBL_HOST     },
        { IDC_LBL_PORT,     K::LBL_PORT     },
        { IDC_LBL_USER,     K::LBL_USER     },
        { IDC_LBL_PASS,     K::LBL_PASSWORD },
        { IDC_BTN_CONNECT,  K::BTN_CONNECT  },
        { IDCANCEL,         K::BTN_CANCEL   },
    };
    SetWindowText(Tr(K::TITLE_LOGIN));
    ApplyTexts(kTexts);
}

void CDlgLogin::OnSelchangeCmbLanguage()
{
    if (Ui::ChangeLanguageFromCombo(m_cmbLanguage))
    {
        ApplyLanguage();
    }
}

void CDlgLogin::OnClickedBtnConnect()
{
    DbConnectionInfo info;
    CString strPort;
    GetDlgItemText(IDC_EDIT_HOST, info.strHost);
    GetDlgItemText(IDC_EDIT_PORT, strPort);
    GetDlgItemText(IDC_EDIT_USER, info.strUser);
    GetDlgItemText(IDC_EDIT_PASS, info.strPassword);
    info.strHost.Trim();
    info.strUser.Trim();
    strPort.Trim();

    if (info.strHost.IsEmpty() || strPort.IsEmpty() || info.strUser.IsEmpty())
    {
        Ui::ShowWarning(K::ERR_LOGIN_EMPTY);
        return;
    }
    if (!TryParseUInt(strPort, AppConst::MIN_PORT, AppConst::MAX_PORT, info.nPort))
    {
        Ui::ShowWarning(K::ERR_PORT_INVALID);
        GotoDlgCtrl(GetDlgItem(IDC_EDIT_PORT));
        return;
    }

    bool bConnected = false;
    {
        CWaitCursor waitCursor;     // con trỏ đồng hồ cát trong lúc chờ kết nối
        bConnected = CDBManager::GetInstance().Connect(info);
    }
    info.strPassword.Empty();

    if (!bConnected)
    {
        Ui::ShowError(K::ERR_CONNECT_FAIL, CDBManager::GetInstance().GetLastErrorText());
        return;
    }

    AppSettings::SetString(SettingKey::HOST, info.strHost);
    AppSettings::SetUInt(SettingKey::PORT, info.nPort);
    AppSettings::SetString(SettingKey::USER, info.strUser);
    EndDialog(IDOK);
}