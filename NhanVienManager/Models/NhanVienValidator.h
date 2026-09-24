// NhanVienValidator.h
// Luật nghiệp vụ kiểm tra dữ liệu nhân viên. Tách khỏi Dialog để:
//   - Dialog nào cũng dùng lại được (thêm, sửa, sau này import từ Excel...)
//   - Không phụ thuộc giao diện / ngôn ngữ: chỉ trả về "lỗi gì, ở trường nào".
#pragma once
#include "Models/NhanVienSchema.h"

enum class ValidationError
{
    NoError,
    Empty,              // bỏ trống trường bắt buộc
    TooLong,            // dài quá nMaxLength
    InvalidHoTen,       // họ tên chứa chữ số
    InvalidAccount,     // tài khoản có ký tự không cho phép
    InvalidNgaySinh     // ngày sinh sai / ngoài khoảng cho phép
};

struct ValidationResult
{
    bool bValid = true;
    ValidationError error = ValidationError::NoError;
    NvField field = NvField::ID;   // trường bị lỗi -> UI đặt con trỏ vào đó
};

ValidationResult ValidateNhanVien(const NhanVienInfo& nv);