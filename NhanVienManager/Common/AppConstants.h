// AppConstants.h
// NƠI DUY NHẤT chứa các hằng số cấu hình của ứng dụng.
// Muốn đổi giá trị mặc định (cổng, tên bảng, thư mục ngôn ngữ...) -> chỉ sửa ở đây.
// Lưu ý: file này KHÔNG chứa chữ hiển thị cho người dùng (chữ nằm trong lang\*.lang).
#pragma once
#include "pch.h"
namespace AppConst
{
    // Cài đặt được lưu trong Registry: HKEY_CURRENT_USER\Software\<REGISTRY_KEY>\NhanVienManager\Settings
    constexpr LPCTSTR REGISTRY_KEY = _T("NhanVienManager");
    constexpr LPCTSTR SETTINGS_SECTION = _T("Settings");

    // Ngôn ngữ: thư mục "lang" nằm cạnh file .exe, mỗi file *.lang là 1 ngôn ngữ.
    constexpr LPCTSTR LANG_FOLDER = _T("lang");
    constexpr LPCTSTR LANG_FILE_PATTERN = _T("*.lang");
    constexpr LPCTSTR DEFAULT_LANGUAGE = _T("vi");   // cũng là ngôn ngữ dự phòng khi thiếu key

    // Kết nối MySQL
    constexpr LPCTSTR DEFAULT_HOST = _T("localhost");
    constexpr UINT    DEFAULT_PORT = 3306;
    constexpr UINT    MIN_PORT = 1;
    constexpr UINT    MAX_PORT = 65535;
    constexpr UINT    CONNECT_TIMEOUT_SECONDS = 5;

    // Database
    constexpr LPCTSTR DEFAULT_TABLE = _T("tb_nhanvien");
    constexpr int     MAX_IDENTIFIER_LENGTH = 64;         // giới hạn tên DB/bảng/cột của MySQL

    // Nghiệp vụ
    constexpr int     MIN_BIRTH_YEAR = 1900;
}

// Tên các mục cài đặt trong Registry
namespace SettingKey
{
    constexpr LPCTSTR LANGUAGE = _T("Language");
    constexpr LPCTSTR HOST = _T("Host");
    constexpr LPCTSTR PORT = _T("Port");
    constexpr LPCTSTR USER = _T("User");
    constexpr LPCTSTR DB_NAME = _T("DatabaseName");
    constexpr LPCTSTR TABLE_NAME = _T("TableName");
}