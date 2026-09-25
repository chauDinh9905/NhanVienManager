// CDlgNhanVienInfo.cpp
#include "pch.h"
#include "resource.h"
#include "UI/CDlgNhanVienInfo.h"
#include "UI/UiHelpers.h"
#include "Common/AppConstants.h"
#include "Common/DateUtil.h"
#include "Common/StringUtil.h"
#include "Localization/LanguageManager.h"

namespace
{
    // Bảng ghép "trường dữ liệu" <-> "nhãn" <-> "ô nhập" cho các trường dạng chữ.
    // Thêm 1 trường chữ mới = thêm 1 dòng ở đây (và 2 control trong .rc).
    struct TextFieldCtrl
    {
        NvField field;
        UINT    nLabelID;
        UINT    nEditID;
    };

    const TextFieldCtrl kTextFields[] =
    {
        { NvField::Account,   IDC_LBL_ACCOUNT,   IDC_EDIT_ACCOUNT   },
        { NvField::HoTen,     IDC_LBL_HOTEN,     IDC_EDIT_HOTEN     },
        { NvField::QueQuan,   IDC_LBL_QUEQUAN,   IDC_EDIT_QUEQUAN   },
        { NvField::TruongHoc, IDC_LBL_TRUONGHOC, IDC_EDIT_TRUONGHOC },
    };

    // Nhãn của các trường không phải ô chữ
    struct FieldLabel
    {
        NvField field;
        UINT    nLabelID;
    };

    const FieldLabel kOtherLabels[] =
    {
        { NvField::ID,       IDC_LBL_ID       },
        { NvField::NgaySinh, IDC_LBL_NGAYSINH },
        { NvField::GioiTinh, IDC_LBL_GIOITINH },
    };
}

IMPLEMENT_DYNAMIC(CDlgNhanVienInfo, CBaseDialog)

CDlgNhanVienInfo::CDlgNhanVienInfo(const NhanVienInfo& nv, bool bEditMode, CWnd* pParent)
    : CBaseDialog(IDD_DLG_NHANVIEN_INFO, pParent)
    , m_nv(nv)
    , m_bEditMode(bEditMode)
{
}

void CDlgNhanVienInfo::DoDataExchange(CDataExchange* pDX)
{
    CBaseDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DTP_NGAYSINH, m_dtpNgaySinh);
}

BEGIN_MESSAGE_MAP(CDlgNhanVienInfo, CBaseDialog)
END_MESSAGE_MAP()

BOOL CDlgNhanVienInfo::OnInitControls()
{
    // Ô chữ: giới hạn độ dài lấy từ bảng mô tả cột -> không bao giờ vượt quá VARCHAR(n) của DB
    for (const auto& ctrl : kTextFields)
    {
        const NvColumnDef& col = NvSchema::GetColumnDef(ctrl.field);
        CEdit* pEdit = static_cast<CEdit*>(GetDlgItem(ctrl.nEditID));
        if (col.nMaxLength > 0) pEdit->LimitText(col.nMaxLength);
        pEdit->SetWindowText(NvSchema::GetFieldText(m_nv, ctrl.field));
    }

    // Ngày sinh: chọn bằng lịch, chỉ cho phép từ MIN_BIRTH_YEAR đến hôm nay
    const COleDateTime dtMin(AppConst::MIN_BIRTH_YEAR, 1, 1, 0, 0, 0);
    const COleDateTime dtMax = COleDateTime::GetCurrentTime();
    m_dtpNgaySinh.SetRange(&dtMin, &dtMax);

    SYSTEMTIME st = {};
    if (IsoToSystemTime(m_nv.strNgaySinh, st))
        m_dtpNgaySinh.SetTime(&st);
    else
        m_dtpNgaySinh.SetTime(static_cast<LPSYSTEMTIME>(nullptr));   // chưa có ngày: ô bỏ tích (DTS_SHOWNONE)

    // Giới tính: chọn theo ID của nút, không phụ thuộc thứ tự nút trên dialog
    CheckRadioButton(IDC_RADIO_NAM, IDC_RADIO_NU,
        m_nv.gioiTinh == GioiTinh::Nam ? IDC_RADIO_NAM : IDC_RADIO_NU);

    GotoDlgCtrl(GetDlgItem(IDC_EDIT_ACCOUNT));
    return FALSE;
}

void CDlgNhanVienInfo::SetupLayout()
{
    static const LayoutItem kLayout[] =
    {
        //  ID                  MoveX MoveY SizeX SizeY
        { IDC_STATIC_ID,           0,    0,  100,    0 },
        { IDC_EDIT_ACCOUNT,        0,    0,  100,    0 },
        { IDC_EDIT_HOTEN,          0,    0,  100,    0 },
        { IDC_EDIT_QUEQUAN,        0,    0,  100,    0 },
        { IDC_EDIT_TRUONGHOC,      0,    0,  100,    0 },
        { IDOK,                  100,  100,    0,    0 },
        { IDCANCEL,              100,  100,    0,    0 },
    };
    ApplyLayout(kLayout);
}

void CDlgNhanVienInfo::ApplyLanguage()
{
    SetWindowText(Tr(m_bEditMode ? K::DLG_TITLE_EDIT : K::DLG_TITLE_ADD));

    // Nhãn lấy từ CÙNG key với tiêu đề cột ở màn hình chính -> 2 nơi luôn khớp nhau
    for (const auto& ctrl : kTextFields)
    {
        SetDlgItemText(ctrl.nLabelID, Ui::MakeLabel(NvSchema::GetColumnDef(ctrl.field).headerKey));
    }
    for (const auto& label : kOtherLabels)
    {
        SetDlgItemText(label.nLabelID, Ui::MakeLabel(NvSchema::GetColumnDef(label.field).headerKey));
    }

    SetDlgItemText(IDC_STATIC_ID, m_bEditMode ? IntToString(m_nv.nID) : Tr(K::LBL_ID_AUTO));

    static const CtrlTextItem kTexts[] =
    {
        { IDC_RADIO_NAM, K::GENDER_MALE   },
        { IDC_RADIO_NU,  K::GENDER_FEMALE },
        { IDOK,          K::BTN_OK        },
        { IDCANCEL,      K::BTN_CANCEL    },
    };
    ApplyTexts(kTexts);

    m_dtpNgaySinh.SetFormat(Tr(K::FMT_DATE_DISPLAY));
}

NhanVienInfo CDlgNhanVienInfo::ReadForm()
{
    NhanVienInfo nv = m_nv;     // giữ nguyên ID

    for (const auto& ctrl : kTextFields)
    {
        CString strValue;
        GetDlgItemText(ctrl.nEditID, strValue);
        strValue.Trim();
        NvSchema::SetFieldText(nv, ctrl.field, strValue);
    }

    SYSTEMTIME st = {};
    const DWORD dwResult = m_dtpNgaySinh.GetTime(&st);
    nv.strNgaySinh = (dwResult == GDT_VALID) ? SystemTimeToIso(st) : CString();

    nv.gioiTinh = (GetCheckedRadioButton(IDC_RADIO_NAM, IDC_RADIO_NU) == IDC_RADIO_NU)
        ? GioiTinh::Nu : GioiTinh::Nam;
    return nv;
}

CString CDlgNhanVienInfo::BuildErrorMessage(const ValidationResult& result) const
{
    const CLanguageManager& lang = CLanguageManager::GetInstance();
    const NvColumnDef& col = NvSchema::GetColumnDef(result.field);
    const CString strFieldName = Tr(col.headerKey);

    switch (result.error)
    {
    case ValidationError::Empty:
        return lang.Format(K::ERR_EMPTY_FIELD, static_cast<LPCTSTR>(strFieldName));
    case ValidationError::TooLong:
        return lang.Format(K::ERR_TOO_LONG, static_cast<LPCTSTR>(strFieldName), col.nMaxLength);
    case ValidationError::InvalidHoTen:
        return Tr(K::ERR_INVALID_HOTEN);
    case ValidationError::InvalidAccount:
        return Tr(K::ERR_INVALID_ACCOUNT);
    case ValidationError::InvalidNgaySinh:
        return lang.Format(K::ERR_INVALID_NGAYSINH, AppConst::MIN_BIRTH_YEAR);
    default:
        return CString();
    }
}

UINT CDlgNhanVienInfo::GetControlIdForField(NvField field) const
{
    for (const auto& ctrl : kTextFields)
    {
        if (ctrl.field == field) return ctrl.nEditID;
    }
    if (field == NvField::NgaySinh) return IDC_DTP_NGAYSINH;
    if (field == NvField::GioiTinh) return IDC_RADIO_NAM;
    return IDC_EDIT_ACCOUNT;
}

void CDlgNhanVienInfo::OnOK()
{
    const NhanVienInfo nv = ReadForm();

    const ValidationResult result = ValidateNhanVien(nv);
    if (!result.bValid)
    {
        AfxMessageBox(BuildErrorMessage(result), MB_OK | MB_ICONWARNING);
        GotoDlgCtrl(GetDlgItem(GetControlIdForField(result.field)));
        return;
    }

    m_nv = nv;
    EndDialog(IDOK);
}