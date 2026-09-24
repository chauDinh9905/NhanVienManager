// SqlUtil.cpp
#include "pch.h"
#include "Data/SqlUtil.h"
#include "Data/DBManager.h"
#include "Common/AppConstants.h"
#include "Common/StringUtil.h"

namespace SqlUtil
{
    bool IsValidIdentifier(const CString& strName)
    {
        const int nLen = strName.GetLength();
        if (nLen == 0 || nLen > AppConst::MAX_IDENTIFIER_LENGTH) return false;
        if (!IsAsciiLetter(strName[0]) && strName[0] != _T('_')) return false;

        for (int i = 1; i < nLen; ++i)
        {
            const TCHAR ch = strName[i];
            if (!IsAsciiLetter(ch) && !IsAsciiDigit(ch) && ch != _T('_')) return false;
        }
        return true;
    }

    CString QuoteIdentifier(const CString& strName) // tên cột, tên bảng
    {
        CString strEscaped(strName);
        strEscaped.Replace(_T("`"), _T("``"));
        return _T("`") + strEscaped + _T("`");
    }

    CString QuoteString(const CString& strValue) // giá trị dữ liệu trong sql
    {
        return _T("'") + CDBManager::GetInstance().Escape(strValue) + _T("'");
    }
}