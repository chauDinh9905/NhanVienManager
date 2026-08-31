#pragma once
// FieldInfo.h
#pragma once
#include <afxwin.h>

enum class FieldOp { None, Existing, Add, Modify, Delete };

struct FieldInfo {
    CString  strOldName;   // Tên cột cũ (rỗng nếu là cột mới thêm)
    CString  strName;      // Tên cột hiện tại (có thể bị đổi tên)
    CString  strDataType;  // "VARCHAR(100)", "INT", "DATETIME", "TEXT" ...
    bool     bPrimaryKey;
    bool     bNotNull;
    FieldOp  op;
};