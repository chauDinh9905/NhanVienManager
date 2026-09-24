// StringUtil.h
// Các hàm xử lý chuỗi dùng chung.
#pragma once
#include "pch.h"

// ---------------------------------------------------------------------------
// UTF-8 <-> CString
// MySQL trả về / nhận vào chuỗi UTF-8 (vì kết nối dùng utf8mb4).
// CString trong project Unicode là UTF-16.
// => MỌI chuỗi đọc từ MySQL phải qua FromUtf8(), MỌI chuỗi gửi xuống phải qua ToUtf8().
// KHÔNG được viết CString(row[i]) vì như vậy Windows sẽ đọc theo bảng mã ANSI
// và tiếng Việt có dấu sẽ bị lỗi font.
// ---------------------------------------------------------------------------
inline CString FromUtf8(const char* pszUtf8)
{
    if (pszUtf8 == nullptr) return CString();
    return CString(CA2W(pszUtf8, CP_UTF8));
}

inline CStringA ToUtf8(const CString& str)
{
    return CStringA(CW2A(str, CP_UTF8));
}

// ---------------------------------------------------------------------------
// Kiểm tra ký tự ASCII (không phụ thuộc locale của máy như _istalpha/_istdigit)
// ---------------------------------------------------------------------------
inline bool IsAsciiLetter(TCHAR ch)
{
    return (ch >= _T('a') && ch <= _T('z')) || (ch >= _T('A') && ch <= _T('Z'));
}

inline bool IsAsciiDigit(TCHAR ch)
{
    return ch >= _T('0') && ch <= _T('9');
}

inline bool IsAllAsciiDigits(const CString& str)
{
    if (str.IsEmpty()) return false;
    for (int i = 0; i < str.GetLength(); ++i)
    {
        if (!IsAsciiDigit(str[i])) return false;
    }
    return true;
}

// Đọc số nguyên dương trong khoảng [nMin, nMax]. Trả về false nếu chuỗi không hợp lệ.
inline bool TryParseUInt(const CString& str, UINT nMin, UINT nMax, UINT& nOut)
{
    if (!IsAllAsciiDigits(str) || str.GetLength() > 9) return false;
    const UINT nValue = static_cast<UINT>(_ttoi(str));
    if (nValue < nMin || nValue > nMax) return false;
    nOut = nValue;
    return true;
}

inline CString IntToString(long long nValue)
{
    CString str;
    str.Format(_T("%lld"), nValue);
    return str;
}

// Nối danh sách chuỗi: {"a","b","c"} + ", " -> "a, b, c"
inline CString JoinStrings(const std::vector<CString>& items, LPCTSTR pszSeparator)
{
    CString strResult;
    for (size_t i = 0; i < items.size(); ++i)
    {
        if (i > 0) strResult += pszSeparator;
        strResult += items[i];
    }
    return strResult;
}