// NhanVienInfo.h
// Dữ liệu của 1 nhân viên. Mọi lớp (UI, Repository, Validator) trao đổi với nhau
// bằng struct này, KHÔNG truyền từng biến CString rời rạc.
#pragma once
#include "pch.h"
// Giá trị số trùng với giá trị lưu trong cột GioiTinh của Database.
enum class GioiTinh : int
{
    Nu = 0,
    Nam = 1
};

// Hai hàm DUY NHẤT quy đổi giữa GioiTinh và giá trị trong DB.
inline int GioiTinhToDbValue(GioiTinh gioiTinh)
{
    return static_cast<int>(gioiTinh);
}

inline GioiTinh GioiTinhFromDbValue(int nValue)
{
    return (nValue == static_cast<int>(GioiTinh::Nam)) ? GioiTinh::Nam : GioiTinh::Nu;
}

struct NhanVienInfo
{
    int      nID = 0;                    // 0 = nhân viên mới, chưa có trong DB
    CString  strAccount;
    CString  strHoTen;
    CString  strQueQuan;
    CString  strNgaySinh;                // luôn dạng ISO "YYYY-MM-DD" (rỗng = chưa nhập)
    GioiTinh gioiTinh = GioiTinh::Nam;
    CString  strTruongHoc;
};