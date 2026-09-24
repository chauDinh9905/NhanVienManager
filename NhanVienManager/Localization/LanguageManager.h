// LanguageManager.h
// Quản lý đa ngôn ngữ. Chữ hiển thị KHÔNG nằm trong code mà nằm trong các file
// lang\<mã>.lang (UTF-8) cạnh file .exe. Thêm ngôn ngữ mới = thả thêm 1 file .lang,
// KHÔNG cần sửa code, KHÔNG cần build lại.
//
// Cách dùng trong code:
//     Tr(K::BTN_ADD)                                        -> "Thêm" / "Add"
//     CLanguageManager::GetInstance().Format(K::CONFIRM_DELETE, 3)
#pragma once
#include "Common/StringKeys.h"
using namespace std;
struct LanguageInfo
{
    CString strCode;   // "vi", "en"... = tên file không có đuôi
    CString strName;   // tên hiển thị trong combobox (key META_LANG_NAME trong file)
    CString strPath;   // đường dẫn đầy đủ tới file .lang
};

class CLanguageManager
{
public:
    static CLanguageManager& GetInstance();

    // Quét thư mục lang\, nạp ngôn ngữ đã lưu. Gọi 1 lần trong InitInstance().
    // Trả về false nếu không tìm thấy file ngôn ngữ nào.
    bool Initialize();

    const vector<LanguageInfo>& GetLanguages() const { return m_languages; }
    const CString& GetCurrentCode() const { return m_strCurrentCode; }

    // Đổi ngôn ngữ và lưu lựa chọn vào Registry. Trả về false nếu mã không tồn tại.
    bool SetLanguage(const CString& strCode);

    // Lấy chuỗi theo key. Thiếu key -> lấy từ ngôn ngữ dự phòng -> vẫn thiếu thì trả "[KEY]".
    CString Get(LPCTSTR pszKey) const;

    // Lấy chuỗi rồi Format. Tham số CString phải ép kiểu: (LPCTSTR)str
    CString Format(LPCTSTR pszKey, ...) const;

private:
    CLanguageManager() = default; //Cho phép compiler tự tạo constructor mặc định
    CLanguageManager(const CLanguageManager&) = delete; // Không được phép sao chép constructor
    CLanguageManager& operator=(const CLanguageManager&) = delete; // Không được phép gán constructor này cho constructor khác

    using Dictionary = map<CString, CString>;

    static CString GetLanguageFolder();
    static bool LoadFile(const CString& strPath, Dictionary& outDict);
    static CString ExtractFormatSpecifiers(const CString& str);
    const LanguageInfo* FindLanguage(const CString& strCode) const;
    void ReportProblems(const Dictionary& dict, const CString& strCode) const;

    vector<LanguageInfo> m_languages;
    Dictionary m_current;        // ngôn ngữ đang dùng
    Dictionary m_fallback;       // ngôn ngữ dự phòng (AppConst::DEFAULT_LANGUAGE)
    CString    m_strCurrentCode;
};

// Viết tắt cho gọn: Tr(K::BTN_ADD)
inline CString Tr(LPCTSTR pszKey)
{
    return CLanguageManager::GetInstance().Get(pszKey);
}