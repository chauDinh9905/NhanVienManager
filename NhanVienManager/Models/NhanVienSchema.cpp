// NhanVienSchema.cpp
#include "pch.h"
#include "Models/NhanVienSchema.h"
#include "Common/StringUtil.h"
using namespace std;
namespace NvSchema
{
    const NvColumnDef& GetPrimaryKey()
    {
        for (const auto& col : kColumns)
        {
            if (col.bPrimaryKey) return col;
        }
        ASSERT(FALSE);          // không thể xảy ra nhờ static_assert CountPrimaryKeys()
        return kColumns[0];
    }

    // Đây là 2 hàm DUY NHẤT biết trường nào của NhanVienInfo ứng với NvField nào.
    CString GetFieldText(const NhanVienInfo& nv, NvField field)
    {
        switch (field)
        {
        case NvField::ID:        return IntToString(nv.nID);
        case NvField::Account:   return nv.strAccount;
        case NvField::HoTen:     return nv.strHoTen;
        case NvField::QueQuan:   return nv.strQueQuan;
        case NvField::NgaySinh:  return nv.strNgaySinh;
        case NvField::GioiTinh:  return IntToString(GioiTinhToDbValue(nv.gioiTinh));
        case NvField::TruongHoc: return nv.strTruongHoc;
        default:
            ASSERT(FALSE);      // quên thêm case khi thêm trường mới
            return CString();
        }
    }

    void SetFieldText(NhanVienInfo& nv, NvField field, const CString& strValue)
    {
        switch (field)
        {
        case NvField::ID:        nv.nID = _ttoi(strValue);                           break;
        case NvField::Account:   nv.strAccount = strValue;                           break;
        case NvField::HoTen:     nv.strHoTen = strValue;                             break;
        case NvField::QueQuan:   nv.strQueQuan = strValue;                           break;
        case NvField::NgaySinh:  nv.strNgaySinh = strValue;                          break;
        case NvField::GioiTinh:  nv.gioiTinh = GioiTinhFromDbValue(_ttoi(strValue)); break;
        case NvField::TruongHoc: nv.strTruongHoc = strValue;                         break;
        default:
            ASSERT(FALSE);
            break;
        }
    }

    vector<FieldInfo> BuildRequiredFields()
    {
        vector<FieldInfo> fields;
        for (const auto& col : kColumns)
        {
            FieldInfo f;
            f.strName = col.dbName;
            f.strDataType = col.sqlType;
            f.bPrimaryKey = col.bPrimaryKey;
            f.bNotNull = col.bRequired || col.bPrimaryKey;
            f.bLocked = true;
            f.op = FieldOp::Add;
            fields.push_back(f);
        }
        return fields;
    }

    bool IsRequiredColumn(const CString& strColumnName)
    {
        for (const auto& col : kColumns)
        {
            if (strColumnName.CompareNoCase(col.dbName) == 0) return true;
        }
        return false;
    }

    vector<CString> FindMissingColumns(const std::vector<FieldInfo>& actualFields)
    {
        vector<CString> missing;
        for (const auto& col : kColumns)
        {
            bool bFound = false;
            for (const auto& f : actualFields)
            {
                if (f.op != FieldOp::Delete && f.strName.CompareNoCase(col.dbName) == 0)
                {
                    bFound = true;
                    break;
                }
            }
            if (!bFound) missing.push_back(CString(col.dbName));
        }
        return missing;
    }
}