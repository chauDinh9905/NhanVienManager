// SchemaRepository.cpp
#include "pch.h"
#include "Data/SchemaRepository.h"
#include "Data/DBManager.h"
#include "Data/SqlUtil.h"
#include "Models/DataTypes.h"
#include "Common/StringUtil.h"

bool CSchemaRepository::Run(const CString& strSql)
{
    CDBManager& db = CDBManager::GetInstance();
    if (!db.Execute(strSql))
    {
        m_strLastError = db.GetLastErrorText();
        return false;
    }
    m_strLastError.Empty();
    return true;
}

bool CSchemaRepository::EnsureDatabase(const CString& strDbName)
{
    if (!SqlUtil::IsValidIdentifier(strDbName))
    {
        m_strLastError = _T("Invalid database name: ") + strDbName;
        return false;
    }

    CDBManager& db = CDBManager::GetInstance();

    // Thử chọn trước: tài khoản không có quyền CREATE vẫn dùng được database có sẵn.
    if (db.SelectDatabase(strDbName)) return true;

    if (!Run(_T("CREATE DATABASE IF NOT EXISTS ") + SqlUtil::QuoteIdentifier(strDbName)
        + _T(" CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci")))
    {
        return false;
    }

    if (!db.SelectDatabase(strDbName))
    {
        m_strLastError = db.GetLastErrorText();
        return false;
    }
    return true;
}

bool CSchemaRepository::TableExists(const CString& strTableName, bool& bExists)
{
    bExists = false;

    // Dùng information_schema với phép so sánh "=" (KHÔNG dùng SHOW TABLES LIKE,
    // vì trong LIKE dấu _ là ký tự đại diện: "tb_nv" sẽ khớp cả "tbXnv").
    const CString strSql =
        _T("SELECT COUNT(*) FROM information_schema.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = ")
        + SqlUtil::QuoteString(strTableName);

    CDBManager& db = CDBManager::GetInstance();
    MySqlResultPtr result = db.Query(strSql);
    if (!result)
    {
        m_strLastError = db.GetLastErrorText();
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result.get());
    bExists = (row != nullptr && row[0] != nullptr && atoi(row[0]) > 0);
    return true;
}

bool CSchemaRepository::LoadColumns(const CString& strTableName, std::vector<FieldInfo>& outFields)
{
    outFields.clear();

    CDBManager& db = CDBManager::GetInstance();
    MySqlResultPtr result = db.Query(_T("SHOW COLUMNS FROM ") + SqlUtil::QuoteIdentifier(strTableName));
    if (!result)
    {
        m_strLastError = db.GetLastErrorText();
        return false;
    }

    // Các cột kết quả: 0 Field | 1 Type | 2 Null | 3 Key | 4 Default | 5 Extra
    MYSQL_ROW row = nullptr;
    while ((row = mysql_fetch_row(result.get())) != nullptr)
    {
        FieldInfo f;
        f.strName = FromUtf8(row[0]);
        f.strOldName = f.strName;
        f.strDataType = FromUtf8(row[1]);
        f.strOldDataType = f.strDataType;
        f.bNotNull = FromUtf8(row[2]).CompareNoCase(_T("NO")) == 0;
        f.bPrimaryKey = FromUtf8(row[3]).CompareNoCase(_T("PRI")) == 0;
        f.op = FieldOp::Existing;
        outFields.push_back(f);
    }
    return true;
}

// Chặn lần cuối trước khi ghép SQL: tên cột mới / đổi tên phải hợp lệ,
// kiểu dữ liệu phải nằm trong danh sách cho phép (hoặc giữ nguyên kiểu cũ trong DB).
bool CSchemaRepository::ValidateFields(const std::vector<FieldInfo>& fields)
{
    for (const auto& f : fields)
    {
        if (f.op == FieldOp::Existing || f.op == FieldOp::Delete) continue;

        const bool bNameChanged = (f.op == FieldOp::Add) || (f.strName != f.strOldName);
        if (bNameChanged && !SqlUtil::IsValidIdentifier(f.strName))
        {
            m_strLastError = _T("Invalid column name: ") + f.strName;
            return false;
        }

        const bool bTypeOk = IsAllowedDataType(f.strDataType)
            || (f.op == FieldOp::Modify && f.strDataType.CompareNoCase(f.strOldDataType) == 0);
        if (!bTypeOk)
        {
            m_strLastError = _T("Invalid data type: ") + f.strDataType;
            return false;
        }
    }
    return true;
}

CString CSchemaRepository::BuildColumnDefinition(const FieldInfo& field)
{
    CString strDef = SqlUtil::QuoteIdentifier(field.strName) + _T(" ") + field.strDataType;
    if (field.bNotNull || field.bPrimaryKey) strDef += _T(" NOT NULL");
    if (field.bPrimaryKey) strDef += _T(" AUTO_INCREMENT PRIMARY KEY");
    return strDef;
}

bool CSchemaRepository::CreateTable(const CString& strTableName, const std::vector<FieldInfo>& fields)
{
    if (!SqlUtil::IsValidIdentifier(strTableName))
    {
        m_strLastError = _T("Invalid table name: ") + strTableName;
        return false;
    }
    if (!ValidateFields(fields)) return false;

    std::vector<CString> definitions;
    for (const auto& f : fields)
    {
        if (f.op != FieldOp::Delete) definitions.push_back(BuildColumnDefinition(f));
    }
    if (definitions.empty())
    {
        m_strLastError = _T("The table has no columns.");
        return false;
    }

    return Run(_T("CREATE TABLE ") + SqlUtil::QuoteIdentifier(strTableName)
        + _T(" (") + JoinStrings(definitions, _T(", ")) + _T(")")
        + _T(" ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci"));
}

bool CSchemaRepository::HasChanges(const std::vector<FieldInfo>& fields)
{
    for (const auto& f : fields)
    {
        if (f.op != FieldOp::Existing) return true;
    }
    return false;
}

bool CSchemaRepository::ApplyChanges(const CString& strTableName, const std::vector<FieldInfo>& fields)
{
    if (!ValidateFields(fields)) return false;

    // Thứ tự: xóa -> sửa -> thêm (để xóa cột "A" rồi thêm lại cột "A" vẫn hợp lệ)
    std::vector<CString> drops, changes, adds;
    for (const auto& f : fields)
    {
        switch (f.op)
        {
        case FieldOp::Delete:
            drops.push_back(_T("DROP COLUMN ") + SqlUtil::QuoteIdentifier(f.strOldName));
            break;
        case FieldOp::Modify:
            changes.push_back(_T("CHANGE COLUMN ") + SqlUtil::QuoteIdentifier(f.strOldName)
                + _T(" ") + BuildColumnDefinition(f));
            break;
        case FieldOp::Add:
            adds.push_back(_T("ADD COLUMN ") + BuildColumnDefinition(f));
            break;
        default:
            break;
        }
    }

    std::vector<CString> clauses;
    clauses.insert(clauses.end(), drops.begin(), drops.end());
    clauses.insert(clauses.end(), changes.begin(), changes.end());
    clauses.insert(clauses.end(), adds.begin(), adds.end());
    if (clauses.empty()) return true;

    return Run(_T("ALTER TABLE ") + SqlUtil::QuoteIdentifier(strTableName)
        + _T(" ") + JoinStrings(clauses, _T(", ")));
}