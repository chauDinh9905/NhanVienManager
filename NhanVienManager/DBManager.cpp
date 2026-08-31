// DBManager.cpp
#include "pch.h"
#include "DBManager.h"

CDBManager::CDBManager() : m_pConn(nullptr), m_bConnected(false), m_nPort(3306) {}
CDBManager::~CDBManager() { Disconnect(); }

bool CDBManager::Connect(const CString& host, const CString& user, const CString& pass,
    const CString& db, unsigned int port) {
    Disconnect();
    m_pConn = mysql_init(nullptr);
    if (!m_pConn) return false;

    // Thiết lập utf8mb4 hỗ trợ tiếng Việt
    mysql_options(m_pConn, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    CT2A szHost(host), szUser(user), szPass(pass), szDB(db);
    if (mysql_real_connect(m_pConn, szHost, szUser, szPass,
        db.IsEmpty() ? nullptr : (const char*)szDB, port, nullptr, 0)) {
        m_bConnected = true;
        m_strHost = host; m_strUser = user; m_strPass = pass; m_strDB = db; m_nPort = port;
        return true;
    }
    return false;
}

void CDBManager::Disconnect() {
    if (m_pConn) {
        mysql_close(m_pConn);
        m_pConn = nullptr;
    }
    m_bConnected = false;
}

bool CDBManager::ExecuteQuery(const CString& strQuery) {
    if (!m_bConnected || !m_pConn) return false;
    CT2A szQuery(strQuery, CP_UTF8);
    return (mysql_query(m_pConn, szQuery) == 0);
}

MYSQL_RES* CDBManager::FetchResult(const CString& strQuery) {
    if (!ExecuteQuery(strQuery)) return nullptr;
    return mysql_store_result(m_pConn);
}

// Chống SQL Injection: escape ký tự đặc biệt (dấu nháy đơn, backslash...) trước khi nhúng vào câu SQL
CString CDBManager::EscapeString(const CString& strInput) {
    if (!m_bConnected || !m_pConn) return strInput;
    CT2A szInput(strInput, CP_UTF8);
    size_t nLen = strlen(szInput);
    std::vector<char> buffer(nLen * 2 + 1);
    mysql_real_escape_string(m_pConn, buffer.data(), szInput, (unsigned long)nLen);
    return CString(CA2T(buffer.data(), CP_UTF8));
}