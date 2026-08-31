// CDlgConnectDB.cpp : implementation file
//

#include "pch.h"
#include "NhanVienManager.h"
#include "afxdialogex.h"
#include "CDlgConnectDB.h"
#include "DBManager.h"
#include "CDlgCreateTable.h"
#include "CDlgEditTable.h"
#include "LanguageManager.h"


// CDlgConnectDB dialog

IMPLEMENT_DYNAMIC(CDlgConnectDB, CDialogEx)

CDlgConnectDB::CDlgConnectDB(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CONNECT_DB, pParent)
	, m_strDBName(_T(""))
	, m_strTableName(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	EnableAutomation();

}

CDlgConnectDB::~CDlgConnectDB()
{
}

void CDlgConnectDB::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialogEx::OnFinalRelease();
}

void CDlgConnectDB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DBNAME, m_strDBName);
	DDX_Text(pDX, IDC_EDIT_TABLENAME, m_strTableName);
}


BEGIN_MESSAGE_MAP(CDlgConnectDB, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CDlgConnectDB::OnClickedBtnCheck)
//	ON_COMMAND(IDC_BTN_CONNECT, &CDlgConnectDB::OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CREATE_TABLE, &CDlgConnectDB::OnClickedBtnCreateTable)
	ON_BN_CLICKED(IDC_BTN_EDIT_TABLE, &CDlgConnectDB::OnClickedBtnEditTable)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDlgConnectDB, CDialogEx)
END_DISPATCH_MAP()

// Note: we add support for IID_IDlgConnectDB to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {bcad22b9-ea5c-4d75-884e-3746639c0363}
static const IID IID_IDlgConnectDB =
{0xbcad22b9,0xea5c,0x4d75,{0x88,0x4e,0x37,0x46,0x63,0x9c,0x03,0x63}};

BEGIN_INTERFACE_MAP(CDlgConnectDB, CDialogEx)
	INTERFACE_PART(CDlgConnectDB, IID_IDlgConnectDB, Dispatch)
END_INTERFACE_MAP()


// CDlgConnectDB message handlers

void CDlgConnectDB::OnClickedBtnCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CLanguageManager& langMgr = CLanguageManager::GetInstance();

	if (m_strDBName.IsEmpty() || m_strTableName.IsEmpty()) {
		AfxMessageBox(langMgr.GetString(_T("ERR_DB_TABLE_EMPTY")), MB_ICONERROR);
		return;
	}

	if (!CheckDatabaseExists(m_strDBName)) {
		CString strCreateDB;
		strCreateDB.Format(_T("CREATE DATABASE IF NOT EXISTS %s CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci"), m_strDBName);
		CDBManager::GetInstance().ExecuteQuery(strCreateDB);
	}
	CDBManager::GetInstance().ExecuteQuery(_T("USE ") + m_strDBName);

	if (!CheckTableExists(m_strTableName)) {
		AfxMessageBox(langMgr.GetString(_T("MSG_TABLE_NOT_EXISTS")), MB_ICONINFORMATION);
		GetDlgItem(IDC_BTN_CREATE_TABLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_EDIT_TABLE)->ShowWindow(SW_HIDE);
	}
	else {
		AfxMessageBox(langMgr.GetString(_T("MSG_TABLE_EXISTS")), MB_ICONINFORMATION);
		GetDlgItem(IDC_BTN_EDIT_TABLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_CREATE_TABLE)->ShowWindow(SW_HIDE);
	}
}

//void CDlgConnectDB::OnBtnConnect()
//{
	// TODO: Add your command handler code here
//}

void CDlgConnectDB::OnClickedBtnCreateTable()
{
	// TODO: Add your control notification handler code here
	CDlgCreateTable dlg(m_strTableName);
	if (dlg.DoModal() == IDOK) CDialogEx::OnOK();

}

void CDlgConnectDB::OnClickedBtnEditTable()
{
	// TODO: Add your control notification handler code here
	CDlgEditTable dlg(m_strTableName);
	if (dlg.DoModal() == IDOK) CDialogEx::OnOK();

}
bool CDlgConnectDB::CheckDatabaseExists(const CString& strDBName) {
	CString strSQL;
	strSQL.Format(_T("SHOW DATABASES LIKE '%s'"), strDBName);
	MYSQL_RES* res = CDBManager::GetInstance().FetchResult(strSQL);
	bool bExists = (res && mysql_num_rows(res) > 0);
	if (res) mysql_free_result(res);
	return bExists;
}
bool CDlgConnectDB::CheckTableExists(const CString& strTableName) {
	CString strSQL;
	strSQL.Format(_T("SHOW TABLES LIKE '%s'"), strTableName);
	MYSQL_RES* res = CDBManager::GetInstance().FetchResult(strSQL);
	bool bExists = (res && mysql_num_rows(res) > 0);
	if (res) mysql_free_result(res);
	return bExists;
}