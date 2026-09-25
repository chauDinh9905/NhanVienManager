// BaseDialog.cpp
#include "pch.h"
#include <afxlayout.h>
#include "UI/BaseDialog.h"
#include "Localization/LanguageManager.h"

IMPLEMENT_DYNAMIC(CBaseDialog, CDialogEx)

CBaseDialog::CBaseDialog(UINT nIDTemplate, CWnd* pParent)
    : CDialogEx(nIDTemplate, pParent)
{
}

BEGIN_MESSAGE_MAP(CBaseDialog, CDialogEx)
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

BOOL CBaseDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Kích thước lúc thiết kế trong .rc chính là kích thước nhỏ nhất cho phép
    CRect rcWindow;
    GetWindowRect(&rcWindow);
    m_sizeMin = rcWindow.Size();

    const BOOL bResult = OnInitControls();
    SetupLayout();
    ApplyLanguage();
    return bResult;
}

void CBaseDialog::ApplyTexts(const CtrlTextItem* pItems, size_t nCount)
{
    for (size_t i = 0; i < nCount; ++i)
    {
        CWnd* pCtrl = GetDlgItem(pItems[i].nID);
        ASSERT(pCtrl != nullptr);       // ID không có trên dialog -> dừng ngay khi chạy Debug
        if (pCtrl != nullptr) pCtrl->SetWindowText(Tr(pItems[i].pszKey));
    }
}

void CBaseDialog::ApplyLayout(const LayoutItem* pItems, size_t nCount)
{
    EnableDynamicLayout(TRUE);
    CMFCDynamicLayout* pLayout = GetDynamicLayout();
    if (pLayout == nullptr) return;
    pLayout->Create(this);

    for (size_t i = 0; i < nCount; ++i)
    {
        const LayoutItem& item = pItems[i];
        ASSERT(GetDlgItem(item.nID) != nullptr);
        if (GetDlgItem(item.nID) == nullptr) continue;

        pLayout->AddItem(item.nID,
            CMFCDynamicLayout::MoveHorizontalAndVertical(item.nMoveX, item.nMoveY),
            CMFCDynamicLayout::SizeHorizontalAndVertical(item.nSizeX, item.nSizeY));
    }
}

void CBaseDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    CDialogEx::OnGetMinMaxInfo(lpMMI);
    if (m_sizeMin.cx > 0 && m_sizeMin.cy > 0)
    {
        lpMMI->ptMinTrackSize.x = m_sizeMin.cx;
        lpMMI->ptMinTrackSize.y = m_sizeMin.cy;
    }
}