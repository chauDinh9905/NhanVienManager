// NhanVienSchema.h
// ===========================================================================
// BẢNG MÔ TẢ CỘT DUY NHẤT của bảng nhân viên.
// Từ bảng này, chương trình TỰ ĐỘNG sinh ra:
//   - Các cột của ListView ở màn hình chính (tiêu đề, độ rộng, căn lề)
//   - Câu SELECT / INSERT / UPDATE trong CNhanVienRepository
//   - Cấu trúc bảng mặc định khi "Tạo bảng" và danh sách cột bắt buộc khi kiểm tra bảng
//   - Nhãn và giới hạn độ dài ô nhập trong CDlgNhanVienInfo, luật "bắt buộc nhập"
// => Đổi tên cột DB, độ rộng cột, độ dài tối đa... chỉ sửa 1 dòng ở đây.
// ===========================================================================
#pragma once
#include "Common/StringKeys.h"
#include "Models/NhanVienInfo.h"
#include "Models/FieldInfo.h"
using namespace std;

// Thứ tự trong enum PHẢI trùng thứ tự các dòng trong NvSchema::kColumns (có static_assert kiểm tra).
enum class NvField
{
    ID,
    Account,
    HoTen,
    QueQuan,
    NgaySinh,
    GioiTinh,
    TruongHoc,
    Count       // luôn để cuối cùng
};

enum class ColKind
{
    Integer,    // số nguyên, không có dấu nháy trong SQL
    Text,       // chuỗi, được escape và đặt trong '...'
    Date,       // ngày, lưu "YYYY-MM-DD"
    Gender      // giới tính, lưu 0/1, hiển thị Nam/Nữ theo ngôn ngữ
};

struct NvColumnDef
{
    NvField field;
    LPCTSTR dbName;       // tên cột trong MySQL
    ColKind kind;
    LPCTSTR sqlType;      // kiểu dữ liệu khi tạo bảng (phải có trong DataTypes.h)
    bool    bPrimaryKey;  // khóa chính tự tăng
    bool    bRequired;    // bắt buộc nhập (NOT NULL)
    int     nMaxLength;   // độ dài tối đa của ô nhập, 0 = không áp dụng
    LPCTSTR headerKey;    // key ngôn ngữ cho tiêu đề cột và nhãn
    int     nWidth;       // độ rộng cột trên ListView (pixel)
    int     nAlign;       // LVCFMT_LEFT / LVCFMT_CENTER / LVCFMT_RIGHT
};

namespace NvSchema
{
    constexpr NvColumnDef kColumns[] =
    {
        //  field               dbName           kind              sqlType            PK     Required  MaxLen  headerKey          Width  Align
        { NvField::ID,        _T("ID"),        ColKind::Integer, _T("INT"),          true,  true,       0,   K::COL_ID,           60, LVCFMT_LEFT   },
        { NvField::Account,   _T("Account"),   ColKind::Text,    _T("VARCHAR(100)"), false, true,     100,   K::COL_ACCOUNT,     120, LVCFMT_LEFT   },
        { NvField::HoTen,     _T("HoTen"),     ColKind::Text,    _T("VARCHAR(150)"), false, true,     150,   K::COL_HOTEN,       170, LVCFMT_LEFT   },
        { NvField::QueQuan,   _T("QueQuan"),   ColKind::Text,    _T("VARCHAR(200)"), false, true,     200,   K::COL_QUEQUAN,     140, LVCFMT_LEFT   },
        { NvField::NgaySinh,  _T("NgaySinh"),  ColKind::Date,    _T("DATE"),         false, true,       0,   K::COL_NGAYSINH,    100, LVCFMT_CENTER },
        { NvField::GioiTinh,  _T("GioiTinh"),  ColKind::Gender,  _T("TINYINT"),      false, true,       0,   K::COL_GIOITINH,     80, LVCFMT_CENTER },
        { NvField::TruongHoc, _T("TruongHoc"), ColKind::Text,    _T("VARCHAR(200)"), false, true,     200,   K::COL_TRUONGHOC,   170, LVCFMT_LEFT   },
    };

    constexpr int kColumnCount = static_cast<int>(size(kColumns));

    // ----- Kiểm tra lúc biên dịch: sai là build lỗi ngay, không đợi lúc chạy -----
    static_assert(size(kColumns) == static_cast<size_t>(NvField::Count),
        "So dong trong kColumns phai bang so phan tu cua NvField");

    constexpr bool IsColumnOrderValid()
    {
        int size_ = size(kColumns);
        for (size_t i = 0; i < size_; ++i)
        {
            if (static_cast<size_t>(kColumns[i].field) != i) return false;
        }
        return true;
    }
    static_assert(IsColumnOrderValid(), "Thu tu dong trong kColumns phai trung thu tu enum NvField");

    constexpr int CountPrimaryKeys()
    {
        int nCount = 0;
        for (const auto& col : kColumns)
        {
            if (col.bPrimaryKey) ++nCount;
        }
        return nCount;
    }
    static_assert(CountPrimaryKeys() == 1, "Bang nhan vien phai co dung 1 cot khoa chinh");

    // ----- Tra cứu -----
    inline const NvColumnDef& GetColumnDef(NvField field)
    {
        return kColumns[static_cast<int>(field)];
    }

    const NvColumnDef& GetPrimaryKey();

    // Giá trị "thô" của 1 trường dưới dạng chuỗi: ID "12", ngày "1990-05-20", giới tính "0"/"1".
    CString GetFieldText(const NhanVienInfo& nv, NvField field);
    void    SetFieldText(NhanVienInfo& nv, NvField field, const CString& strValue);

    // Dùng cho màn hình tạo / sửa bảng
    vector<FieldInfo> BuildRequiredFields();
    bool IsRequiredColumn(const CString& strColumnName);
    vector<CString> FindMissingColumns(const vector<FieldInfo>& actualFields);
}