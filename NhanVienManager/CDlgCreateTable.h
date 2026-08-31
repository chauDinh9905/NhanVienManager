// DlgCreateTable.h
#pragma once
#include "FieldInfo.h"
#include <afxcmn.h>
#include <vector>

class CDlgCreateTable : public CDialogEx {
public:
    CDlgCreateTable(const CString& strTableName, CWnd* pParent = nullptr);
    CString GetGeneratedSQL() const { return m_strGeneratedSQL; }
    ~CDlgCreateTable();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnClickedBtnAddField();
    afx_msg void OnClickedBtnEditField();
    afx_msg void OnClickedBtnDelField();
    afx_msg void OnClickedOk();
    void RefreshListControl();
    bool ValidateFieldName(const CString& strName, int nIgnoreIndex = -1);
    CString BuildCreateTableSQL();
    DECLARE_MESSAGE_MAP()

private:
    CListCtrl  m_listFields;
    CComboBox   m_cmbDataType;
    CString    m_strTableName;
    std::vector<FieldInfo>   m_vecFields;
    CString  m_strGeneratedSQL;
};