// DBManager.h
#pragma once
#include <mysql.h>
#include <afxwin.h>
#include <string>
#include <vector>

class CDBManager {
private:
    MYSQL* m_pConn;
    bool m_bConnected;
    CString m_strHost, m_strUser, m_strPass, m_strDB;
    unsigned int m_nPort;

    CDBManager();
    ~CDBManager();

public:
    static CDBManager& GetInstance() {
        static CDBManager instance;
        return instance;
    }

    bool Connect(const CString& host, const CString& user, const CString& pass,
        const CString& db, unsigned int port = 3306); // Lưu ý: 3306, không phải 306
    void Disconnect();
    MYSQL* GetConnection() { return m_pConn; }
    bool IsConnected() { return m_bConnected; }
    bool ExecuteQuery(const CString& strQuery);
    MYSQL_RES* FetchResult(const CString& strQuery);
    CString EscapeString(const CString& strInput); // Chống SQL Injection, dùng ở Lab 3
};