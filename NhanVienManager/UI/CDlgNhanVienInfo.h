// CDlgNhanVienInfo.h - Thêm / sửa 1 nhân viên
#pragma once
#include "UI/BaseDialog.h"
#include "Models/NhanVienValidator.h"

class CDlgNhanVienInfo : public CBaseDialog
{
    DECLARE_DYNAMIC(CDlgNhanVienInfo)

public:
    // bEditMode = false: thêm mới (nv thường là NhanVienInfo() rỗng)
    // bEditMode = true : sửa nhân viên nv có sẵn
    CDlgNhanVienInfo(const NhanVienInfo& nv, bool bEditMode, CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLG_NHANVIEN_INFO };
#endif

    void ApplyLanguage() override;

    // Kết quả sau khi DoModal() == IDOK (đã được kiểm tra hợp lệ)
    const NhanVienInfo& GetNhanVien() const { return m_nv; }

protected:
    void DoDataExchange(CDataExchange* pDX) override;
    BOOL OnInitControls() override;
    void SetupLayout() override;
    void OnOK() override;
    DECLARE_MESSAGE_MAP()

private:
    NhanVienInfo ReadForm();
    CString      BuildErrorMessage(const ValidationResult& result) const;
    UINT         GetControlIdForField(NvField field) const;

    NhanVienInfo  m_nv;
    bool          m_bEditMode;
    CDateTimeCtrl m_dtpNgaySinh;
};