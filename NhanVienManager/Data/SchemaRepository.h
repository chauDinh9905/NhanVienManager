// SchemaRepository.h
// Mọi câu SQL thao tác CẤU TRÚC (database, bảng, cột) nằm ở đây.
// Lớp này KHÔNG biết gì về "nhân viên" -> dùng lại được cho bảng bất kỳ.
#pragma once
#include "Models/FieldInfo.h"

class CSchemaRepository
{
public:
    // Chọn database; nếu chưa có thì tạo mới (utf8mb4) rồi chọn.
    bool EnsureDatabase(const CString& strDbName);

    // bExists = bảng có tồn tại trong database đang chọn hay không.
    bool TableExists(const CString& strTableName, bool& bExists);

    // Đọc danh sách cột của bảng (mỗi cột có op = FieldOp::Existing).
    bool LoadColumns(const CString& strTableName, std::vector<FieldInfo>& outFields);

    bool CreateTable(const CString& strTableName, const std::vector<FieldInfo>& fields);

    // Áp dụng TẤT CẢ thay đổi bằng MỘT câu ALTER TABLE duy nhất:
    // lỗi ở bất kỳ cột nào -> không thay đổi nào được áp dụng (không bị "sửa dở dang").
    bool ApplyChanges(const CString& strTableName, const std::vector<FieldInfo>& fields);

    static bool HasChanges(const std::vector<FieldInfo>& fields);

    const CString& GetLastError() const { return m_strLastError; }

private:
    bool Run(const CString& strSql);
    bool ValidateFields(const std::vector<FieldInfo>& fields);
    static CString BuildColumnDefinition(const FieldInfo& field);

    CString m_strLastError;
};