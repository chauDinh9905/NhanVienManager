// UiHelpers.h
// Các hàm giao diện dùng chung: hộp thoại thông báo, combobox ngôn ngữ, tiêu đề cột ListView.
#pragma once
#include "pch.h"
namespace Ui
{
    // Hộp thoại thông báo - tham số là KEY ngôn ngữ, không phải chữ.
    void ShowInfo(LPCTSTR pszKey);
    void ShowWarning(LPCTSTR pszKey);

    // Báo lỗi kèm "Chi tiết: <lỗi thật từ MySQL>" để dễ tìm nguyên nhân.
    void ShowError(LPCTSTR pszKey, const CString& strDetail = CString());

    // Hỏi Có / Không. strMessage là chữ đã dịch sẵn (thường tạo bằng Format).
    bool Confirm(const CString& strMessage);

    // Nạp danh sách ngôn ngữ vào combobox (chọn sẵn ngôn ngữ hiện tại).
    void FillLanguageCombo(CComboBox& cmb);

    // Đổi ngôn ngữ theo mục đang chọn. Trả về true nếu đổi thành công.
    bool ChangeLanguageFromCombo(CComboBox& cmb);

    void SetListColumnText(CListCtrl& list, int nColumn, const CString& strText);

    // Nhãn ô nhập: Tr(key) + định dạng FMT_LABEL, vd "Họ và tên" -> "Họ và tên:"
    CString MakeLabel(LPCTSTR pszKey);
}