// LanguageManager.cpp
#include "pch.h"
#include "LanguageManager.h"

void CLanguageManager::InitDictionary() {
    auto add = [this](const TCHAR* key, const TCHAR* vi, const TCHAR* en) {
        m_dictionary[key][LANG_VIETNAMESE] = vi;
        m_dictionary[key][LANG_ENGLISH] = en;
        };

    add(_T("TITLE_MAIN"), _T("Quản Lý Nhân Viên - MFC & MySQL"), _T("Employee Management - MFC & MySQL"));

    add(_T("BTN_ADD"), _T("Thêm"), _T("Add"));
    add(_T("BTN_EDIT"), _T("Sửa"), _T("Edit"));
    add(_T("BTN_DELETE"), _T("Xóa"), _T("Delete"));

    add(_T("COL_ID"), _T("ID"), _T("ID"));
    add(_T("COL_ACCOUNT"), _T("Tài Khoản"), _T("Account"));
    add(_T("COL_HOTEN"), _T("Họ và Tên"), _T("Full Name"));
    add(_T("COL_QUEQUAN"), _T("Quê Quán"), _T("Hometown"));
    add(_T("COL_NGAYSINH"), _T("Ngày Sinh"), _T("Date of Birth"));
    add(_T("COL_GIOITINH"), _T("Giới Tính"), _T("Gender"));
    add(_T("COL_TRUONGHOC"), _T("Trường Học"), _T("School"));

    add(_T("GENDER_MALE"), _T("Nam"), _T("Male"));
    add(_T("GENDER_FEMALE"), _T("Nữ"), _T("Female"));

    add(_T("DLG_TITLE_ADD"), _T("Thêm Nhân Viên"), _T("Add Employee"));
    add(_T("DLG_TITLE_EDIT"), _T("Sửa Thông Tin Nhân Viên"), _T("Edit Employee"));

    add(_T("LBL_ACCOUNT"), _T("Tài Khoản:"), _T("Account:"));
    add(_T("LBL_HOTEN"), _T("Họ và Tên:"), _T("Full Name:"));
    add(_T("LBL_QUEQUAN"), _T("Quê Quán:"), _T("Hometown:"));
    add(_T("LBL_NGAYSINH"), _T("Ngày Sinh (YYYY-MM-DD):"), _T("Date of Birth (YYYY-MM-DD):"));
    add(_T("LBL_TRUONGHOC"), _T("Trường Học:"), _T("School:"));

    add(_T("ERR_SELECT_ONE"), _T("Vui lòng chọn ĐÚNG 1 dòng dữ liệu để thực hiện chức năng Sửa!"),
        _T("Please select EXACTLY ONE row to edit!"));
    add(_T("ERR_SELECT_NONE_DELETE"), _T("Chưa chọn dòng nào để xóa! Vui lòng chọn ít nhất một bản ghi."),
        _T("No row selected! Please select at least one record."));
    add(_T("CONFIRM_DELETE"), _T("Bạn có chắc chắn muốn xóa %d bản ghi đã chọn khỏi Database hay không?"),
        _T("Are you sure you want to delete %d selected record(s)?"));

    add(_T("ERR_EMPTY_FIELD"), _T("Vui lòng điền đầy đủ các trường thông tin, không được để trống!"),
        _T("Please fill in all fields, none can be left empty!"));
    add(_T("ERR_INVALID_HOTEN"), _T("Họ và tên không hợp lệ! Không được chứa ký tự số."),
        _T("Invalid full name! It must not contain digits."));
    add(_T("ERR_INVALID_ACCOUNT"), _T("Tài khoản không được chứa ký tự đặc biệt hoặc khoảng trắng!"),
        _T("Account must not contain special characters or spaces!"));
    add(_T("ERR_INVALID_NGAYSINH"), _T("Ngày sinh không hợp lệ! Định dạng đúng: YYYY-MM-DD."),
        _T("Invalid date of birth! Correct format: YYYY-MM-DD."));

    add(_T("MSG_ADD_SUCCESS"), _T("Thêm nhân viên thành công!"), _T("Employee added successfully!"));
    add(_T("MSG_ADD_FAIL"), _T("Thêm nhân viên thất bại!"), _T("Failed to add employee!"));
    add(_T("MSG_EDIT_SUCCESS"), _T("Cập nhật dữ liệu nhân viên thành công!"), _T("Employee updated successfully!"));
    add(_T("MSG_EDIT_FAIL"), _T("Cập nhật dữ liệu thất bại!"), _T("Update failed!"));
    add(_T("MSG_DELETE_SUCCESS"), _T("Xóa thành công các bản ghi đã chọn!"), _T("Selected records deleted successfully!"));

    add(_T("ERR_LOGIN_EMPTY"), _T("Vui lòng nhập đầy đủ Host, Port và User!"), _T("Please fill in Host, Port and User!"));
    add(_T("ERR_PORT_INVALID"), _T("Port phải là số!"), _T("Port must be numeric!"));
    add(_T("ERR_CONNECT_FAIL"), _T("Kết nối MySQL thất bại! Vui lòng kiểm tra lại thông tin."),
        _T("MySQL connection failed! Please check your credentials."));
    add(_T("ERR_DB_TABLE_EMPTY"), _T("Vui lòng nhập tên Database và tên Bảng!"), _T("Please enter Database name and Table name!"));
    add(_T("MSG_TABLE_NOT_EXISTS"), _T("Bảng chưa tồn tại. Vui lòng bấm 'Tạo Bảng' để khởi tạo."),
        _T("Table does not exist yet. Click 'Create Table' to initialize."));
    add(_T("MSG_TABLE_EXISTS"), _T("Bảng đã tồn tại. Bấm 'Sửa Bảng' nếu cần bổ sung cột."),
        _T("Table already exists. Click 'Edit Table' if you need to add columns."));
}