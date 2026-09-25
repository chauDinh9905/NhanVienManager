// LanguageManager.cpp
#include "pch.h"
#include "Localization/LanguageManager.h"
#include "Common/AppConstants.h"
#include "Common/AppSettings.h"

CLanguageManager& CLanguageManager::GetInstance()
{
    static CLanguageManager instance;
    return instance;
}

// Thư mục lang\ nằm cạnh file .exe (KHÔNG dùng thư mục làm việc hiện tại,
// vì khi chạy từ Visual Studio thư mục làm việc là thư mục project).
CString CLanguageManager::GetLanguageFolder()
{
    TCHAR szExePath[MAX_PATH] = {};
    ::GetModuleFileName(nullptr, szExePath, MAX_PATH);

    CString strFolder(szExePath);
    const int nPos = strFolder.ReverseFind(_T('\\'));
    if (nPos >= 0) strFolder = strFolder.Left(nPos);
    return strFolder + _T("\\") + AppConst::LANG_FOLDER;
}

bool CLanguageManager::Initialize()
{
    m_languages.clear();

    // 1) Quét tất cả file *.lang
    CFileFind finder;
    BOOL bFound = finder.FindFile(GetLanguageFolder() + _T("\\") + AppConst::LANG_FILE_PATTERN);
    while (bFound)
    {
        bFound = finder.FindNextFile();
        if (finder.IsDirectory()) continue;

        Dictionary dict;
        if (!LoadFile(finder.GetFilePath(), dict)) continue;

        LanguageInfo info;
        const CString strFileName = finder.GetFileName();
        const int nDot = strFileName.ReverseFind(_T('.'));
        info.strCode = (nDot > 0) ? strFileName.Left(nDot) : strFileName;      // "vi.lang" -> "vi"
        info.strPath = finder.GetFilePath();
        auto it = dict.find(K::META_LANG_NAME);
        info.strName = (it != dict.end()) ? it->second : info.strCode;
        m_languages.push_back(info);
    }
    finder.Close();

    if (m_languages.empty()) return false;

    std::sort(m_languages.begin(), m_languages.end(),
        [](const LanguageInfo& a, const LanguageInfo& b) { return a.strName.CompareNoCase(b.strName) < 0; });

    // 2) Nạp ngôn ngữ dự phòng
    const LanguageInfo* pDefault = FindLanguage(AppConst::DEFAULT_LANGUAGE);
    if (pDefault == nullptr) pDefault = &m_languages.front();
    LoadFile(pDefault->strPath, m_fallback);
    ReportProblems(m_fallback, pDefault->strCode);

    // 3) Nạp ngôn ngữ người dùng đã chọn lần trước
    const CString strSaved = AppSettings::GetString(SettingKey::LANGUAGE, pDefault->strCode);
    if (!SetLanguage(strSaved))
    {
        SetLanguage(pDefault->strCode);
    }
    return true;
}

const LanguageInfo* CLanguageManager::FindLanguage(const CString& strCode) const
{
    for (const auto& lang : m_languages)
    {
        if (lang.strCode.CompareNoCase(strCode) == 0) return &lang;
    }
    return nullptr;
}

bool CLanguageManager::SetLanguage(const CString& strCode)
{
    const LanguageInfo* pLang = FindLanguage(strCode);
    if (pLang == nullptr) return false;

    Dictionary dict;
    if (!LoadFile(pLang->strPath, dict)) return false;
    ReportProblems(dict, pLang->strCode);

    m_current.swap(dict);
    m_strCurrentCode = pLang->strCode;
    AppSettings::SetString(SettingKey::LANGUAGE, m_strCurrentCode);
    return true;
}

CString CLanguageManager::Get(LPCTSTR pszKey) const
{
    auto it = m_current.find(pszKey);
    if (it != m_current.end()) return it->second;

    it = m_fallback.find(pszKey);
    if (it != m_fallback.end()) return it->second;

    // Thiếu key: hiện "[KEY]" để nhìn là thấy ngay trên giao diện
    CString strMissing;
    strMissing.Format(_T("[%s]"), pszKey);
    return strMissing;
}

CString CLanguageManager::Format(LPCTSTR pszKey, ...) const
{
    const CString strFormat = Get(pszKey);

    va_list args;
    va_start(args, pszKey);
    CString strResult;
    strResult.FormatV(strFormat, args);
    va_end(args);
    return strResult;
}

// Đọc file "KEY=Giá trị" dạng UTF-8.
bool CLanguageManager::LoadFile(const CString& strPath, Dictionary& outDict)
{
    outDict.clear();

    FILE* pFile = nullptr;
    if (_tfopen_s(&pFile, strPath, _T("rt, ccs=UTF-8")) != 0 || pFile == nullptr) return false;

    TCHAR szLine[4096];
    while (_fgetts(szLine, _countof(szLine), pFile) != nullptr)
    {
        CString strLine(szLine);
        strLine.Trim();
        if (strLine.IsEmpty() || strLine[0] == _T('#')) continue;

        const int nEqual = strLine.Find(_T('='));
        if (nEqual <= 0) continue;

        CString strKey = strLine.Left(nEqual);
        CString strValue = strLine.Mid(nEqual + 1);
        strKey.Trim();
        strValue.Trim();
        strValue.Replace(_T("\\n"), _T("\n"));
        outDict[strKey] = strValue;
    }
    fclose(pFile);
    return true;
}

// "Xóa %d bản ghi của %s" -> "ds". Dùng để phát hiện bản dịch sai tham số.
CString CLanguageManager::ExtractFormatSpecifiers(const CString& str)
{
    CString strResult;
    const int nLen = str.GetLength();
    for (int i = 0; i < nLen; ++i)
    {
        if (str[i] != _T('%')) continue;
        ++i;
        if (i >= nLen) break;
        if (str[i] == _T('%')) continue;                       // "%%" là dấu % bình thường
        while (i < nLen && !IsCharAlpha(str[i])) ++i;          // bỏ qua cờ, độ rộng
        if (i < nLen) strResult += str[i];
    }
    return strResult;
}

// Chỉ chạy ở bản Debug: báo cho lập trình viên biết file .lang thiếu key
// hoặc có số lượng/thứ tự %s %d khác với ngôn ngữ dự phòng (dễ gây crash khi Format).
void CLanguageManager::ReportProblems(const Dictionary& dict, const CString& strCode) const
{
#ifdef _DEBUG
    CString strProblems;
    for (LPCTSTR pszKey : kAllStringKeys)
    {
        auto it = dict.find(pszKey);
        if (it == dict.end())
        {
            strProblems += CString(_T("  - Thiếu key: ")) + pszKey + _T("\n");
            continue;
        }
        auto itRef = m_fallback.find(pszKey);
        if (itRef != m_fallback.end() &&
            ExtractFormatSpecifiers(it->second) != ExtractFormatSpecifiers(itRef->second))
        {
            strProblems += CString(_T("  - Sai tham số %s/%d: ")) + pszKey + _T("\n");
        }
    }
    if (!strProblems.IsEmpty())
    {
        CString strMessage;
        strMessage.Format(_T("[DEBUG] File ngôn ngữ \"%s.lang\" có vấn đề:\n%s"),
            static_cast<LPCTSTR>(strCode), static_cast<LPCTSTR>(strProblems));
        AfxMessageBox(strMessage, MB_ICONWARNING);
    }
#else
    UNREFERENCED_PARAMETER(dict);
    UNREFERENCED_PARAMETER(strCode);
#endif
}