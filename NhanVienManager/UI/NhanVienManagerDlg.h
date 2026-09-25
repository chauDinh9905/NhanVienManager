// NhanVienManagerDlg.h - Màn hình chính: danh sách nhân viên
#pragma once
#include "UI/BaseDialog.h"
#include "Data/NhanVienRepository.h"

class CNhanVienManagerDlg : public CBaseDialog
{
    DECLARE_DYNAMIC(CNhanVienManagerDlg)

public:
    // strTableName: bảng đã chọn ở CDlgConnectDB (KHÔNG hardcode tên bảng).
    explicit CNhanVienManagerDlg(const CString& strTableName, CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_NHANVIENMANAGER_DIALOG };
#endif

    void ApplyLanguage() override;

protected:
    void DoDataExchange(CDataExchange* pDX) override;
    BOOL OnInitControls() override;
    void SetupLayout() override;
    void OnOK() override;       // chặn phím Enter đóng chương trình
    void OnCancel() override;   // chặn phím Esc đóng chương trình

    afx_msg void OnClose();
    afx_msg void OnClickedBtnAdd();
    afx_msg void OnClickedBtnEdit();
    afx_msg void OnClickedBtnDelete();
    afx_msg void OnSelchangeCmbLanguage();
    afx_msg void OnDblclkListNhanVien(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

private:
    void                ReloadData();        // đọc lại từ DB rồi vẽ lại
    void                RefreshListView();   // chỉ vẽ lại từ m_data (không đọc DB)
    CString             GetDisplayText(const NhanVienInfo& nv, const NvColumnDef& col) const;
    std::vector<size_t> GetSelectedIndexes() const;

    HICON                     m_hIcon;
    CListCtrl                 m_listNhanVien;
    CComboBox                 m_cmbLanguage;
    CNhanVienRepository       m_repo;
    std::vector<NhanVienInfo> m_data;        // dữ liệu gốc; ItemData của mỗi dòng = vị trí trong m_data
};