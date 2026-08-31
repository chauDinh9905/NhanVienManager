// LanguageManager.h
#pragma once
#include <afxwin.h>
#include <map>
#undef LANG_VIETNAMESE  
#undef LANG_ENGLISH

enum Language { LANG_VIETNAMESE = 0, LANG_ENGLISH = 1 };

class CLanguageManager {
private:
    Language m_currentLang;
    std::map<CString, std::map<Language, CString>> m_dictionary;

    CLanguageManager() {
        m_currentLang = LoadLanguageSetting();
        InitDictionary();
    }

public:
    static CLanguageManager& GetInstance() {
        static CLanguageManager instance;
        return instance;
    }

    void InitDictionary();
    CString GetString(const CString& strKey) { return m_dictionary[strKey][m_currentLang]; }
    void SetLanguage(Language lang) { m_currentLang = lang; SaveLanguageSetting(lang); }
    Language GetLanguage() { return m_currentLang; }
    void SaveLanguageSetting(Language lang) { AfxGetApp()->WriteProfileInt(_T("Settings"), _T("Language"), (int)lang); }
    Language LoadLanguageSetting() { return (Language)AfxGetApp()->GetProfileInt(_T("Settings"), _T("Language"), 0); }
};