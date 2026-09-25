// StringKeys.h
// DANH SÁCH DUY NHẤT các key ngôn ngữ của chương trình.
//
// Muốn thêm 1 chuỗi hiển thị mới:
//   1) Thêm 1 dòng X(TEN_KEY) vào danh sách bên dưới.
//   2) Thêm dòng TEN_KEY=... vào TẤT CẢ file trong thư mục lang\.
//   3) Trong code dùng: Tr(K::TEN_KEY)
// Gõ sai tên key (K::BTN_ADDD) -> compiler báo lỗi ngay, không đợi đến lúc chạy.
// Bản Debug sẽ tự báo nếu file .lang nào thiếu key hoặc sai %s/%d.
#pragma once
#include "pch.h"
#include "UI/CDlgLogin.h"
#define APP_STRING_KEYS(X) \
    X(META_LANG_NAME) \
    X(FMT_DATE_DISPLAY) \
    X(FMT_LABEL) \
    X(MSG_ERROR_WITH_DETAIL) \
    X(BTN_OK) \
    X(BTN_CANCEL) \
    X(LBL_LANGUAGE) \
    X(ERR_INVALID_IDENTIFIER) \
    X(TITLE_LOGIN) \
    X(LBL_HOST) \
    X(LBL_PORT) \
    X(LBL_USER) \
    X(LBL_PASSWORD) \
    X(BTN_CONNECT) \
    X(ERR_LOGIN_EMPTY) \
    X(ERR_PORT_INVALID) \
    X(ERR_CONNECT_FAIL) \
    X(TITLE_CONNECT_DB) \
    X(LBL_DBNAME) \
    X(LBL_TABLENAME) \
    X(BTN_CHECK) \
    X(BTN_CREATE_TABLE) \
    X(BTN_EDIT_TABLE) \
    X(BTN_OPEN) \
    X(ERR_DB_TABLE_EMPTY) \
    X(ERR_OPEN_DB_FAIL) \
    X(MSG_TABLE_NOT_EXISTS) \
    X(MSG_TABLE_READY) \
    X(MSG_TABLE_MISSING_COLUMNS) \
    X(TITLE_CREATE_TABLE) \
    X(TITLE_EDIT_TABLE) \
    X(LBL_FIELDNAME) \
    X(LBL_DATATYPE) \
    X(CHK_NOTNULL) \
    X(CHK_PK) \
    X(BTN_ADD_FIELD) \
    X(BTN_EDIT_FIELD) \
    X(BTN_DEL_FIELD) \
    X(COL_FIELDNAME) \
    X(COL_DATATYPE) \
    X(COL_NOTNULL) \
    X(COL_PRIMARYKEY) \
    X(COL_STATUS) \
    X(VAL_YES) \
    X(VAL_NO) \
    X(TAG_NEW) \
    X(TAG_MODIFIED) \
    X(TAG_REQUIRED) \
    X(ERR_SELECT_FIELD) \
    X(ERR_FIELDNAME_EMPTY) \
    X(ERR_FIELDNAME_DUP) \
    X(ERR_SELECT_TYPE) \
    X(ERR_FIELD_LOCKED) \
    X(CONFIRM_DELETE_FIELD) \
    X(MSG_CREATE_TABLE_SUCCESS) \
    X(MSG_CREATE_TABLE_FAIL) \
    X(MSG_ALTER_NOTHING) \
    X(MSG_ALTER_SUCCESS) \
    X(MSG_ALTER_FAIL) \
    X(ERR_LOAD_SCHEMA_FAIL) \
    X(TITLE_MAIN) \
    X(BTN_ADD) \
    X(BTN_EDIT) \
    X(BTN_DELETE) \
    X(COL_ID) \
    X(COL_ACCOUNT) \
    X(COL_HOTEN) \
    X(COL_QUEQUAN) \
    X(COL_NGAYSINH) \
    X(COL_GIOITINH) \
    X(COL_TRUONGHOC) \
    X(GENDER_MALE) \
    X(GENDER_FEMALE) \
    X(ERR_SELECT_ONE) \
    X(ERR_SELECT_NONE_DELETE) \
    X(CONFIRM_DELETE) \
    X(MSG_LOAD_FAIL) \
    X(MSG_ADD_SUCCESS) \
    X(MSG_ADD_FAIL) \
    X(MSG_EDIT_SUCCESS) \
    X(MSG_EDIT_FAIL) \
    X(MSG_DELETE_SUCCESS) \
    X(MSG_DELETE_FAIL) \
    X(DLG_TITLE_ADD) \
    X(DLG_TITLE_EDIT) \
    X(LBL_ID_AUTO) \
    X(ERR_EMPTY_FIELD) \
    X(ERR_TOO_LONG) \
    X(ERR_INVALID_HOTEN) \
    X(ERR_INVALID_ACCOUNT) \
    X(ERR_INVALID_NGAYSINH)

namespace K
{
#define DECLARE_STRING_KEY(name) constexpr LPCTSTR name = _T(#name);
    APP_STRING_KEYS(DECLARE_STRING_KEY)
#undef DECLARE_STRING_KEY
}

// Mảng chứa TẤT CẢ key - LanguageManager dùng để kiểm tra file .lang có đủ key không.
#define LIST_STRING_KEY(name) _T(#name),
constexpr LPCTSTR kAllStringKeys[] = { APP_STRING_KEYS(LIST_STRING_KEY) };
#undef LIST_STRING_KEY