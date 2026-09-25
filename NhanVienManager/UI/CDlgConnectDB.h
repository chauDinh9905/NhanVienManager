// CDlgConnectDB.h - Chọn / tạo Database và bảng làm việc
#pragma once
#include "UI/BaseDialog.h"
#include "Data/SchemaRepository.h"

class CDlgConnectDB : public CBaseDialog
{
    DECLARE_DYNAMIC(CDlgConnectDB)

public:
    explicit CDlgConnectDB(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DLG_CONNECT_DB };
#endif

    void ApplyLanguage() override;

    // Kết quả sau khi DoModal() == IDOK
    const CString& GetDatabaseName() const { return m_strDbName; }
    const CString& GetTableName() const { return m_strTableName; }

protected:
    BOOL OnInitControls() override;
    void SetupLayout() override;
    void OnOK() override;

    afx_msg void OnClickedBtnCheck();
    afx_msg void OnClickedBtnCreateTable();
    afx_msg void OnClickedBtnEditTable();
    afx_msg void OnChangeNames();
    DECLARE_MESSAGE_MAP()

private:
    enum class TableState
    {
        Unknown,        // chưa kiểm tra (hoặc vừa sửa tên)
        NotExists,      // bảng chưa có -> cho phép "Tạo bảng"
        Invalid,        // bảng có nhưng thiếu cột bắt buộc -> cho phép "Sửa bảng"
        Ready           // bảng dùng được -> cho phép "Mở"
    };

    bool ReadInputs();
    bool CheckTable(bool bShowReadyMessage);
    void SetState(TableState state);

    TableState        m_state = TableState::Unknown;
    CString           m_strDbName;
    CString           m_strTableName;
    CSchemaRepository m_schemaRepo;
};