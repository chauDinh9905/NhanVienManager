// SqlUtil.h
// Công cụ ghép câu SQL an toàn.
//
// Quy tắc bắt buộc của project:
//   - GIÁ TRỊ dữ liệu  -> SqlUtil::QuoteString(...)       (escape + đặt trong '...')
//   - TÊN DB/bảng/cột  -> SqlUtil::QuoteIdentifier(...)   (đặt trong `...`)
//   - Tên do NGƯỜI DÙNG gõ vào -> kiểm tra SqlUtil::IsValidIdentifier(...) trước.
//   - KHÔNG dùng CString::Format để ghép SQL có chứa dấu % (vd DATE_FORMAT '%Y') -> dùng phép cộng chuỗi.
#pragma once
#include "pch.h"
namespace SqlUtil
{
    // Chỉ chữ cái ASCII, chữ số, dấu gạch dưới; không bắt đầu bằng chữ số; tối đa 64 ký tự.
    bool    IsValidIdentifier(const CString& strName);

    // abc  -> `abc`     (dấu ` bên trong được nhân đôi)
    CString QuoteIdentifier(const CString& strName);

    // O'Neil -> 'O\'Neil'
    CString QuoteString(const CString& strValue);
}