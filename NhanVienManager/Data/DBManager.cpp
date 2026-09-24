// DBManager.cpp
#include "pch.h"
#include "Data/DBManager.h"
#include "Common/AppConstants.h"
#include "Common/StringUtil.h"

CDBManager& CDBManager::GetInstance()
{
    static CDBManager instance;
    return instance;
}

CDBManager::~CDBManager()
{
    Disconnect();
}

bool CDBManager::Connect(const DbConnectionInfo& info)
{
    Disconnect();
    m_strLastError.Empty();

    MYSQL* pConn = mysql_init(nullptr);
    if (pConn == nullptr)
    {
        m_strLastError = _T("mysql_init() failed.");
        return false;
    }

    // Không để chương trình "treo" quá lâu khi máy chủ không phản hồi
    unsigned int nTimeout = AppConst::CONNECT_TIMEOUT_SECONDS;
    mysql_options(pConn, MYSQL_OPT_CONNECT_TIMEOUT, &nTimeout);
    // Bắt buộc để lưu / đọc tiếng Việt có dấu
    mysql_options(pConn, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    const CStringA strHost = ToUtf8(info.strHost);
    const CStringA strUser = ToUtf8(info.strUser);
    const CStringA strPass = ToUtf8(info.strPassword);

    if (mysql_real_connect(pConn, strHost, strUser, strPass, nullptr, info.nPort, nullptr, 0) == nullptr)
    {
        m_strLastError = FromUtf8(mysql_error(pConn));
        mysql_close(pConn);
        return false;
    }

    // Chỉ giữ con trỏ kết nối, KHÔNG lưu mật khẩu lại trong bộ nhớ.
    m_pConn = pConn;
    return true;
}

void CDBManager::Disconnect()
{
    if (m_pConn != nullptr)
    {
        mysql_close(m_pConn);
        m_pConn = nullptr;
    }
}

bool CDBManager::EnsureConnected()
{
    if (IsConnected()) return true;
    m_strLastError = _T("Not connected to MySQL server.");
    return false;
}

void CDBManager::CaptureError(const CString& strSql)
{
    m_strLastError = FromUtf8(mysql_error(m_pConn));
#ifdef _DEBUG
    // Xem trong cửa sổ Output của Visual Studio khi chạy Debug (F5)
    ::OutputDebugString(CString(_T("[DB] SQL loi: ")) + strSql + _T("\n[DB] ") + m_strLastError + _T("\n"));
#else
    UNREFERENCED_PARAMETER(strSql);
#endif
}

bool CDBManager::SelectDatabase(const CString& strDbName)
{
    if (!EnsureConnected()) return false;
    if (mysql_select_db(m_pConn, ToUtf8(strDbName)) != 0)
    {
        CaptureError(_T("mysql_select_db ") + strDbName);
        return false;
    }
    return true;
}

bool CDBManager::Execute(const CString& strSql)
{
    if (!EnsureConnected()) return false;

    const CStringA strUtf8 = ToUtf8(strSql);
    if (mysql_real_query(m_pConn, strUtf8, static_cast<unsigned long>(strUtf8.GetLength())) != 0)
    {
        CaptureError(strSql);
        return false;
    }
    m_strLastError.Empty();
    return true;
}

MySqlResultPtr CDBManager::Query(const CString& strSql)
{
    if (!Execute(strSql)) return nullptr;

    MYSQL_RES* pResult = mysql_store_result(m_pConn);
    if (pResult == nullptr && mysql_field_count(m_pConn) != 0)
    {
        CaptureError(strSql);   // câu lệnh lẽ ra phải trả dữ liệu nhưng đọc thất bại
    }
    return MySqlResultPtr(pResult);
}

CString CDBManager::Escape(const CString& strValue) const
{
    // Không có kết nối thì KHÔNG trả lại chuỗi gốc (tránh lọt SQL Injection).
    ASSERT(IsConnected());
    if (!IsConnected()) return CString();

    const CStringA strUtf8 = ToUtf8(strValue);
    std::vector<char> buffer(static_cast<size_t>(strUtf8.GetLength()) * 2 + 1);
    mysql_real_escape_string(m_pConn, buffer.data(), strUtf8, static_cast<unsigned long>(strUtf8.GetLength()));
    return FromUtf8(buffer.data());
}