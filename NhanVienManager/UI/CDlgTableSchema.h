// CDlgTableSchema.h
// MỘT dialog dùng cho cả "Tạo bảng" và "Sửa cấu trúc bảng" (thay cho 2 dialog cũ
// CDlgCreateTable và CDlgEditTable vốn bị lặp code gần như hoàn toàn).
#pragma once
#include "UI/BaseDialog.h"
#include "Data/SchemaRepository.h"

enum class SchemaMode
{
    Create,
    Edit
};

class CDlgTableSchema : public CBaseDialog
{
    DECLARE_DYNAMIC(CDlgTableSchema)

public:
    CDlgTableSchema(SchemaMode mode, const CString& strTableName, CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLG_TABLE_SCHEMA };
#endif

    void ApplyLanguage() override;

protected:
    void DoDataExchange(CDataExchange* pDX) override;
    BOOL OnInitControls() override;
    void SetupLayout() override;
    void OnOK() override;

    afx_msg void OnClickedBtnAddField();
    afx_msg void OnClickedBtnEditField();
    afx_msg void OnClickedBtnDelField();
    afx_msg void OnItemChangedListFields(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

private:
    bool    LoadFields();
    void    RefreshList();
    int     GetSelectedFieldIndex() const;   // vị trí trong m_fields, -1 nếu chưa chọn
    bool    ReadEditor(CString& strName, CString& strType, bool& bNotNull, const FieldInfo* pCurrent);
    bool    IsDuplicateName(const CString& strName, int nIgnoreIndex) const;
    CString GetStatusText(const FieldInfo& field) const;

    SchemaMode             m_mode;
    CString                m_strTableName;
    std::vector<FieldInfo> m_fields;
    CListCtrl              m_listFields;
    CComboBox              m_cmbDataType;
    CSchemaRepository      m_schemaRepo;
};