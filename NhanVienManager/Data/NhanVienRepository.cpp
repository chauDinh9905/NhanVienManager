// NhanVienRepository.cpp
#include "pch.h"
#include "Data/NhanVienRepository.h"
#include "Data/DBManager.h"
#include "Data/SqlUtil.h"
#include "Common/StringUtil.h"

CNhanVienRepository::CNhanVienRepository(const CString& strTableName)
    : m_strQuotedTable(SqlUtil::QuoteIdentifier(strTableName))
{
}

bool CNhanVienRepository::EnsureConnected()
{
    if (CDBManager::GetInstance().IsConnected()) return true;
    m_strLastError = _T("Not connected to MySQL server.");
    return false;
}

bool CNhanVienRepository::Run(const CString& strSql)
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

// Giá trị của 1 cột dưới dạng SQL: số giữ nguyên, chuỗi / ngày được escape và bọc '...'
CString CNhanVienRepository::BuildValueSql(const NhanVienInfo& nv, const NvColumnDef& col) const
{
    const CString strValue = NvSchema::GetFieldText(nv, col.field);
    switch (col.kind)
    {
    case ColKind::Integer:
    case ColKind::Gender:
        return strValue;        // do chính chương trình format từ kiểu int -> an toàn
    default:
        return SqlUtil::QuoteString(strValue);
    }
}

bool CNhanVienRepository::GetAll(std::vector<NhanVienInfo>& outList)
{
    outList.clear();
    if (!EnsureConnected()) return false;

    // SELECT `ID`, `Account`, ..., DATE_FORMAT(`NgaySinh`, '%Y-%m-%d'), ... FROM `bang` ORDER BY `ID`
    // Chú ý: ghép bằng phép cộng, KHÔNG dùng Format vì chuỗi có chứa dấu %.
    CString strColumns;
    for (const auto& col : NvSchema::kColumns)
    {
        if (!strColumns.IsEmpty()) strColumns += _T(", ");
        const CString strQuoted = SqlUtil::QuoteIdentifier(col.dbName);
        if (col.kind == ColKind::Date)
            strColumns += _T("DATE_FORMAT(") + strQuoted + _T(", '%Y-%m-%d')");
        else
            strColumns += strQuoted;
    }

    const CString strSql = _T("SELECT ") + strColumns
        + _T(" FROM ") + m_strQuotedTable
        + _T(" ORDER BY ") + SqlUtil::QuoteIdentifier(NvSchema::GetPrimaryKey().dbName);

    CDBManager& db = CDBManager::GetInstance();
    MySqlResultPtr result = db.Query(strSql);
    if (!result)
    {
        m_strLastError = db.GetLastErrorText();
        return false;
    }

    MYSQL_ROW row = nullptr;
    while ((row = mysql_fetch_row(result.get())) != nullptr)
    {
        NhanVienInfo nv;
        for (int i = 0; i < NvSchema::kColumnCount; ++i)
        {
            // FromUtf8: bắt buộc, nếu không tiếng Việt sẽ lỗi font
            NvSchema::SetFieldText(nv, NvSchema::kColumns[i].field, FromUtf8(row[i]));
        }
        outList.push_back(nv);
    }
    m_strLastError.Empty();
    return true;
}

bool CNhanVienRepository::Insert(const NhanVienInfo& nv)
{
    if (!EnsureConnected()) return false;

    CString strColumns, strValues;
    for (const auto& col : NvSchema::kColumns)
    {
        if (col.bPrimaryKey) continue;          // ID do MySQL tự tăng
        if (!strColumns.IsEmpty())
        {
            strColumns += _T(", ");
            strValues += _T(", ");
        }
        strColumns += SqlUtil::QuoteIdentifier(col.dbName);
        strValues += BuildValueSql(nv, col);
    }

    return Run(_T("INSERT INTO ") + m_strQuotedTable
        + _T(" (") + strColumns + _T(") VALUES (") + strValues + _T(")"));
}

bool CNhanVienRepository::Update(const NhanVienInfo& nv)
{
    if (!EnsureConnected()) return false;

    CString strSet;
    for (const auto& col : NvSchema::kColumns)
    {
        if (col.bPrimaryKey) continue;
        if (!strSet.IsEmpty()) strSet += _T(", ");
        strSet += SqlUtil::QuoteIdentifier(col.dbName) + _T(" = ") + BuildValueSql(nv, col);
    }

    return Run(_T("UPDATE ") + m_strQuotedTable + _T(" SET ") + strSet
        + _T(" WHERE ") + SqlUtil::QuoteIdentifier(NvSchema::GetPrimaryKey().dbName)
        + _T(" = ") + IntToString(nv.nID));
}

bool CNhanVienRepository::DeleteByIds(const vector<int>& ids)
{
    if (ids.empty()) return true;
    if (!EnsureConnected()) return false;

    vector<CString> idTexts;
    for (int nID : ids) idTexts.push_back(IntToString(nID));

    return Run(_T("DELETE FROM ") + m_strQuotedTable
        + _T(" WHERE ") + SqlUtil::QuoteIdentifier(NvSchema::GetPrimaryKey().dbName)
        + _T(" IN (") + JoinStrings(idTexts, _T(", ")) + _T(")"));
}