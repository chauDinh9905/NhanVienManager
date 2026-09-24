// DBManager.h
// Lớp DUY NHẤT nói chuyện trực tiếp với thư viện MySQL (libmysql).
// Không chứa bất kỳ câu SQL nghiệp vụ nào - SQL nằm trong các Repository.
#pragma once
#include "pch.h"
#include <mysql.h>
struct DbConnectionInfo
{
    CString strHost;
    UINT    nPort = 3306;
    CString strUser;
    CString strPassword;
};

// Tự động gọi mysql_free_result khi biến ra khỏi phạm vi -> không bao giờ quên giải phóng.
struct MySqlResultDeleter
{
    void operator()(MYSQL_RES* pResult) const
    {
        if (pResult != nullptr) mysql_free_result(pResult);
    }
};
using MySqlResultPtr = std::unique_ptr<MYSQL_RES, MySqlResultDeleter>;

class CDBManager
{
public:
    static CDBManager& GetInstance();

    bool Connect(const DbConnectionInfo& info);
    void Disconnect();
    bool IsConnected() const { return m_pConn != nullptr; }

    bool SelectDatabase(const CString& strDbName);

    // Chạy câu lệnh không trả dữ liệu (INSERT, UPDATE, DELETE, CREATE, ALTER...)
    bool Execute(const CString& strSql);

    // Chạy câu SELECT / SHOW. Trả về nullptr nếu lỗi (xem GetLastErrorText()).
    MySqlResultPtr Query(const CString& strSql);

    // Escape GIÁ TRỊ dữ liệu (không dùng cho tên bảng / tên cột - xem SqlUtil).
    CString Escape(const CString& strValue) const;

    // Nội dung lỗi thật từ MySQL của lệnh gần nhất (để hiển thị phần "Chi tiết").
    const CString& GetLastErrorText() const { return m_strLastError; }

private:
    CDBManager() = default;
    ~CDBManager();
    CDBManager(const CDBManager&) = delete;
    CDBManager& operator=(const CDBManager&) = delete;

    bool EnsureConnected();
    void CaptureError(const CString& strSql);

    MYSQL* m_pConn = nullptr;
    CString m_strLastError;
};