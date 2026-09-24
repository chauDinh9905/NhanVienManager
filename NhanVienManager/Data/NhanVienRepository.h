// NhanVienRepository.h
// Mọi câu SQL thao tác DỮ LIỆU nhân viên (SELECT / INSERT / UPDATE / DELETE) nằm ở đây.
// Câu SQL được SINH TỰ ĐỘNG từ NvSchema::kColumns -> thêm cột không phải sửa SQL.
#pragma once
#include "Models/NhanVienSchema.h"

class CNhanVienRepository
{
public:
    // strTableName: tên bảng người dùng đã chọn ở màn hình CDlgConnectDB.
    explicit CNhanVienRepository(const CString& strTableName);

    bool GetAll(std::vector<NhanVienInfo>& outList);
    bool Insert(const NhanVienInfo& nv);
    bool Update(const NhanVienInfo& nv);
    bool DeleteByIds(const std::vector<int>& ids);

    // Chi tiết lỗi của lệnh gần nhất (nội dung từ MySQL)
    const CString& GetLastError() const { return m_strLastError; }

private:
    bool    EnsureConnected();
    bool    Run(const CString& strSql);
    CString BuildValueSql(const NhanVienInfo& nv, const NvColumnDef& col) const;

    CString m_strQuotedTable;   // tên bảng đã bọc `...`
    CString m_strLastError;
};