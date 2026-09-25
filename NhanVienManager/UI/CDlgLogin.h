// CDlgLogin.h - Màn hình đăng nhập MySQL
#pragma once
#include "UI/BaseDialog.h"

class CDlgLogin : public CBaseDialog
{
    DECLARE_DYNAMIC(CDlgLogin)

public:
    explicit CDlgLogin(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLG_LOGIN };
#endif

    void ApplyLanguage() override;

protected:
    void DoDataExchange(CDataExchange* pDX) override;
    BOOL OnInitControls() override;
    void SetupLayout() override;

    afx_msg void OnClickedBtnConnect();
    afx_msg void OnSelchangeCmbLanguage();
    DECLARE_MESSAGE_MAP()

private:
    CComboBox m_cmbLanguage;
};