// UiHelpers.cpp
#include "pch.h"
#include "UI/UiHelpers.h"
#include "Localization/LanguageManager.h"

namespace Ui
{
    void ShowInfo(LPCTSTR pszKey)
    {
        AfxMessageBox(Tr(pszKey), MB_OK | MB_ICONINFORMATION);
    }

    void ShowWarning(LPCTSTR pszKey)
    {
        AfxMessageBox(Tr(pszKey), MB_OK | MB_ICONWARNING);
    }

    void ShowError(LPCTSTR pszKey, const CString& strDetail)
    {
        const CString strMain = Tr(pszKey);
        if (strDetail.IsEmpty())
        {
            AfxMessageBox(strMain, MB_OK | MB_ICONERROR);
            return;
        }
        const CString strMessage = CLanguageManager::GetInstance().Format(K::MSG_ERROR_WITH_DETAIL,
            static_cast<LPCTSTR>(strMain), static_cast<LPCTSTR>(strDetail));
        AfxMessageBox(strMessage, MB_OK | MB_ICONERROR);
    }

    bool Confirm(const CString& strMessage)
    {
        return AfxMessageBox(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES;
    }

    void FillLanguageCombo(CComboBox& cmb)
    {
        const CLanguageManager& lang = CLanguageManager::GetInstance();
        const auto& languages = lang.GetLanguages();

        cmb.ResetContent();
        for (size_t i = 0; i < languages.size(); ++i)
        {
            // Lưu VỊ TRÍ trong danh sách vào ItemData -> đúng ngôn ngữ kể cả khi combobox tự sắp xếp.
            const int nIndex = cmb.AddString(languages[i].strName);
            cmb.SetItemData(nIndex, static_cast<DWORD_PTR>(i));
            if (languages[i].strCode.CompareNoCase(lang.GetCurrentCode()) == 0)
            {
                cmb.SetCurSel(nIndex);
            }
        }
    }

    bool ChangeLanguageFromCombo(CComboBox& cmb)
    {
        const int nSel = cmb.GetCurSel();
        if (nSel == CB_ERR) return false;

        CLanguageManager& lang = CLanguageManager::GetInstance();
        const size_t nLangIndex = static_cast<size_t>(cmb.GetItemData(nSel));
        if (nLangIndex >= lang.GetLanguages().size()) return false;

        return lang.SetLanguage(lang.GetLanguages()[nLangIndex].strCode);
    }

    void SetListColumnText(CListCtrl& list, int nColumn, const CString& strText)
    {
        LVCOLUMN column = {};
        column.mask = LVCF_TEXT;
        column.pszText = const_cast<LPTSTR>(strText.GetString());
        list.SetColumn(nColumn, &column);
    }

    CString MakeLabel(LPCTSTR pszKey)
    {
        return CLanguageManager::GetInstance().Format(K::FMT_LABEL, static_cast<LPCTSTR>(Tr(pszKey)));
    }
}