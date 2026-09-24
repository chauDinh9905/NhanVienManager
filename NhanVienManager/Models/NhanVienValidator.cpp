// NhanVienValidator.cpp
#include "pch.h"
#include "Models/NhanVienValidator.h"
#include "Common/AppConstants.h"
#include "Common/DateUtil.h"
#include "Common/StringUtil.h"

ValidationResult ValidateNhanVien(const NhanVienInfo& nv)
{
    //Luật chung lấy từ bảng mô tả cột: bắt buộc nhập, độ dài tối đa
    for (const auto& col : NvSchema::kColumns)
    {
        if (col.kind != ColKind::Text && col.kind != ColKind::Date) continue;

        const CString strValue = NvSchema::GetFieldText(nv, col.field);
        if (col.bRequired && strValue.IsEmpty())
            return { false, ValidationError::Empty, col.field };
        if (col.nMaxLength > 0 && strValue.GetLength() > col.nMaxLength)
            return { false, ValidationError::TooLong, col.field };
    }

    // Luật riêng từng trường
    for (int i = 0; i < nv.strHoTen.GetLength(); ++i)
    {
        if (IsAsciiDigit(nv.strHoTen[i]))
            return { false, ValidationError::InvalidHoTen, NvField::HoTen };
    }

    for (int i = 0; i < nv.strAccount.GetLength(); ++i)
    {
        const TCHAR ch = nv.strAccount[i];
        if (!IsAsciiLetter(ch) && !IsAsciiDigit(ch) && ch != _T('_'))
            return { false, ValidationError::InvalidAccount, NvField::Account };
    }

    if (!IsValidIsoDate(nv.strNgaySinh, AppConst::MIN_BIRTH_YEAR))
        return { false, ValidationError::InvalidNgaySinh, NvField::NgaySinh };

    return {};
}