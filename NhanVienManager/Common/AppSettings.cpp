// AppSettings.cpp
#include "pch.h"
#include "Common/AppSettings.h"
#include "Common/AppConstants.h"

namespace AppSettings
{   /*Hàm này dùng để đọc thông tin cấu hình kết nối MySQL từ Windows Registry*/
    CString GetString(LPCTSTR pszName, LPCTSTR pszDefault)
    {
        return AfxGetApp()->GetProfileString(AppConst::SETTINGS_SECTION, pszName, pszDefault);
    }
    /*Hàm này dùng để ghi thông tin cấu hình kết nối MySQL vào Window registry*/
    void SetString(LPCTSTR pszName, const CString& strValue)
    {
        AfxGetApp()->WriteProfileString(AppConst::SETTINGS_SECTION, pszName, strValue);
    }
    /*Đọc một thông tin có giá trị nguyên trong cấu hình kết nối MySQL từ Window Registry*/
    UINT GetUInt(LPCTSTR pszName, UINT nDefault)
    {
        return static_cast<UINT>(AfxGetApp()->GetProfileInt(AppConst::SETTINGS_SECTION, pszName, static_cast<int>(nDefault)));
    }
    /*Tương tự hàm trên nhưng là ghi vào chứ không phải đọc*/
    void SetUInt(LPCTSTR pszName, UINT nValue)
    {
        AfxGetApp()->WriteProfileInt(AppConst::SETTINGS_SECTION, pszName, static_cast<int>(nValue));
    }
}