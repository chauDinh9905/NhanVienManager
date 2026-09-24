// AppSettings.h
// Đọc / ghi cài đặt của người dùng (ngôn ngữ, host, port...) vào Registry.
// Tên các mục cài đặt nằm trong namespace SettingKey (AppConstants.h).
// Yêu cầu: InitInstance() đã gọi SetRegistryKey() trước khi dùng.
#pragma once
#include "pch.h"
namespace AppSettings
{
    CString GetString(LPCTSTR pszName, LPCTSTR pszDefault);
    void    SetString(LPCTSTR pszName, const CString& strValue);
    UINT    GetUInt(LPCTSTR pszName, UINT nDefault);
    void    SetUInt(LPCTSTR pszName, UINT nValue);
}