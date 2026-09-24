// DateUtil.h
// Xử lý ngày tháng. Quy ước của TOÀN BỘ chương trình:
//   - Lưu trữ / truyền dữ liệu: luôn dùng dạng ISO "YYYY-MM-DD".
//   - Hiển thị: theo định dạng FMT_DATE_DISPLAY trong file ngôn ngữ (vd "dd/MM/yyyy").
#pragma once
#include "Common/StringUtil.h"

inline CString SystemTimeToIso(const SYSTEMTIME& st)
{
    CString str;
    str.Format(_T("%04u-%02u-%02u"), st.wYear, st.wMonth, st.wDay);
    return str;
}

inline bool IsoToSystemTime(const CString& strIso, SYSTEMTIME& st)
{
    int nYear = 0, nMonth = 0, nDay = 0;
    if (_stscanf_s(strIso, _T("%d-%d-%d"), &nYear, &nMonth, &nDay) != 3) return false;
    st = {};
    st.wYear = static_cast<WORD>(nYear);
    st.wMonth = static_cast<WORD>(nMonth);
    st.wDay = static_cast<WORD>(nDay);
    return true;
}

inline CString TodayIso()
{
    SYSTEMTIME st = {};
    ::GetLocalTime(&st);
    return SystemTimeToIso(st);
}

// Ngày hợp lệ: đúng dạng YYYY-MM-DD, ngày có thật, từ năm nMinYear đến hôm nay.
inline bool IsValidIsoDate(const CString& strDate, int nMinYear)
{
    if (strDate.GetLength() != 10 || strDate[4] != _T('-') || strDate[7] != _T('-')) return false;
    for (int i = 0; i < 10; ++i)
    {
        if (i == 4 || i == 7) continue;
        if (!IsAsciiDigit(strDate[i])) return false;
    }

    const int nYear = _ttoi(strDate.Left(4));
    const int nMonth = _ttoi(strDate.Mid(5, 2));
    const int nDay = _ttoi(strDate.Mid(8, 2));
    if (nYear < nMinYear || nMonth < 1 || nMonth > 12) return false;

    static const int kDaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    const bool bLeap = (nYear % 4 == 0 && nYear % 100 != 0) || (nYear % 400 == 0);
    const int nMaxDay = kDaysInMonth[nMonth - 1] + ((nMonth == 2 && bLeap) ? 1 : 0);
    if (nDay < 1 || nDay > nMaxDay) return false;

    return strDate <= TodayIso();   // với dạng ISO, so sánh chuỗi = so sánh ngày
}

// Đổi "1990-05-20" + "dd/MM/yyyy" -> "20/05/1990".
// Định dạng chỉ hỗ trợ 3 ký hiệu: yyyy, MM, dd (giống cú pháp của Date Time Picker).
inline CString FormatIsoDate(const CString& strIso, const CString& strFormat)
{
    SYSTEMTIME st = {};
    if (!IsoToSystemTime(strIso, st)) return strIso;

    CString strYear, strMonth, strDay;
    strYear.Format(_T("%04u"), st.wYear);
    strMonth.Format(_T("%02u"), st.wMonth);
    strDay.Format(_T("%02u"), st.wDay);

    CString strResult(strFormat);
    strResult.Replace(_T("yyyy"), strYear);
    strResult.Replace(_T("MM"), strMonth);
    strResult.Replace(_T("dd"), strDay);
    return strResult;
}