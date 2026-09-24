// DataTypes.h
// Danh sách DUY NHẤT các kiểu dữ liệu người dùng được chọn khi tạo / sửa cột.
// Combobox kiểu dữ liệu và phần kiểm tra an toàn trước khi chạy SQL đều dùng danh sách này.
// Muốn cho phép thêm kiểu mới -> thêm 1 dòng ở đây.
#pragma once
#include "pch.h"
constexpr LPCTSTR kAllowedDataTypes[] =
{
    _T("INT"),
    _T("TINYINT"),
    _T("BIGINT"),
    _T("DECIMAL(18,2)"),
    _T("VARCHAR(50)"),
    _T("VARCHAR(100)"),
    _T("VARCHAR(150)"),
    _T("VARCHAR(200)"),
    _T("VARCHAR(255)"),
    _T("TEXT"),
    _T("DATE"),
    _T("DATETIME"),
};

inline bool IsAllowedDataType(const CString& strType)
{
    for (LPCTSTR pszType : kAllowedDataTypes)
    {
        if (strType.CompareNoCase(pszType) == 0) return true;
    }
    return false;
}