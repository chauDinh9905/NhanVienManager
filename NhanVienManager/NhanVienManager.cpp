// NhanVienManager.cpp : điểm khởi động của ứng dụng.
#include "pch.h"
#include "framework.h"
#include "NhanVienManager.h"
#include "Common/AppConstants.h"
#include "Localization/LanguageManager.h"
#include "UI/CDlgLogin.h"
#include "UI/CDlgConnectDB.h"
#include "UI/NhanVienManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CNhanVienManagerApp, CWinApp)
END_MESSAGE_MAP()

CNhanVienManagerApp::CNhanVienManagerApp()
{
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CNhanVienManagerApp theApp;

BOOL CNhanVienManagerApp::InitInstance()
{
    // Đăng ký các loại control dùng trong chương trình (ListView, Date Time Picker...)
    INITCOMMONCONTROLSEX initCtrls = {};
    initCtrls.dwSize = sizeof(initCtrls);
    initCtrls.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES;
    InitCommonControlsEx(&initCtrls);

    CWinApp::InitInstance();

    // Lưu cài đặt vào Registry (HKCU) thay vì file .ini trong thư mục Windows.
    // PHẢI gọi trước khi đọc / ghi bất kỳ cài đặt nào.
    SetRegistryKey(AppConst::REGISTRY_KEY);

    if (!CLanguageManager::GetInstance().Initialize())
    {
        // Chỗ DUY NHẤT phải viết chữ trong code: chưa nạp được file ngôn ngữ nào thì không thể dịch.
        AfxMessageBox(_T("Không tìm thấy file ngôn ngữ trong thư mục \"lang\" cạnh file .exe.\n")
            _T("Language files were not found in the \"lang\" folder next to the .exe."),
            MB_OK | MB_ICONERROR);
        return FALSE;
    }

    // Luồng màn hình: Đăng nhập -> Chọn DB/bảng -> Màn hình chính
    CDlgLogin dlgLogin;
    if (dlgLogin.DoModal() != IDOK) return FALSE;

    CDlgConnectDB dlgConnect;
    if (dlgConnect.DoModal() != IDOK) return FALSE;

    CNhanVienManagerDlg dlgMain(dlgConnect.GetTableName());
    m_pMainWnd = &dlgMain;
    dlgMain.DoModal();

    // Trả về FALSE để kết thúc ứng dụng (không chạy vòng lặp thông điệp của CWinApp)
    return FALSE;
}