// BaseDialog.h
// Lớp cha của TẤT CẢ dialog trong chương trình. Gom các việc dialog nào cũng phải làm:
//   1) Khởi tạo theo đúng thứ tự: OnInitControls() -> SetupLayout() -> ApplyLanguage()
//   2) Co giãn giao diện (Dynamic Layout) khai báo bằng bảng LayoutItem
//   3) Kích thước tối thiểu = kích thước lúc thiết kế (không kéo nhỏ hơn làm vỡ giao diện)
//   4) Gán chữ cho control bằng bảng CtrlTextItem { ID control, key ngôn ngữ }
//
// Dialog con KHÔNG override OnInitDialog() (đã khóa bằng final) mà override:
//   OnInitControls()  : tạo cột ListView, nạp combobox, đổ dữ liệu ban đầu...
//   SetupLayout()     : khai báo control nào di chuyển / giãn khi kéo cửa sổ
//   ApplyLanguage()   : gán lại TOÀN BỘ chữ hiển thị (gọi lại được khi đổi ngôn ngữ)
#pragma once
#include "pch.h"
// 1 dòng = 1 control cần gán chữ
struct CtrlTextItem
{
    UINT    nID;
    LPCTSTR pszKey;
};

// 1 dòng = 1 control co giãn. Giá trị 0..100 = tỉ lệ % theo độ thay đổi kích thước cửa sổ.
//   nMoveX = 100 : bám cạnh phải      nMoveY = 100 : bám cạnh dưới
//   nSizeX = 100 : giãn hết chiều rộng nSizeY = 100 : giãn hết chiều cao
struct LayoutItem
{
    UINT nID;
    int  nMoveX;
    int  nMoveY;
    int  nSizeX;
    int  nSizeY;
};

class CBaseDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CBaseDialog)

public:
    CBaseDialog(UINT nIDTemplate, CWnd* pParent = nullptr);

    // Gán lại toàn bộ chữ trên dialog theo ngôn ngữ hiện tại.
    virtual void ApplyLanguage() {}

protected:
    BOOL OnInitDialog() final;

    // Trả về FALSE nếu đã tự SetFocus / GotoDlgCtrl vào 1 control.
    virtual BOOL OnInitControls() { return TRUE; }
    virtual void SetupLayout() {}

    template <size_t N>
    void ApplyTexts(const CtrlTextItem(&items)[N]) { ApplyTexts(items, N); }
    void ApplyTexts(const CtrlTextItem* pItems, size_t nCount);

    template <size_t N>
    void ApplyLayout(const LayoutItem(&items)[N]) { ApplyLayout(items, N); }
    void ApplyLayout(const LayoutItem* pItems, size_t nCount);

    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    DECLARE_MESSAGE_MAP()

private:
    CSize m_sizeMin{ 0, 0 };
};