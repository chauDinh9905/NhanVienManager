// FieldInfo.h
// Mô tả 1 cột (trường) của bảng trong màn hình tạo / sửa cấu trúc bảng.
#pragma once
#include "pch.h"
enum class FieldOp
{
    Existing,   // cột đang có trong DB, chưa đổi gì
    Add,        // cột mới sẽ được thêm
    Modify,     // cột có sẵn, đã bị sửa tên / kiểu / NOT NULL
    Delete      // cột có sẵn, sẽ bị xóa
};

struct FieldInfo
{
    CString strOldName;          // tên trong DB (rỗng nếu là cột mới)
    CString strName;             // tên hiện tại
    CString strOldDataType;      // kiểu trong DB (rỗng nếu là cột mới)
    CString strDataType;         // kiểu hiện tại, vd "VARCHAR(100)"
    bool    bPrimaryKey = false;
    bool    bNotNull = false;
    bool    bLocked = false; // cột chương trình cần -> không cho sửa / xóa
    FieldOp op = FieldOp::Add;
};