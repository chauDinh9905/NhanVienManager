//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by NhanVienManager.rc
//
// QUY ƯỚC ĐÁNH SỐ (mỗi nhóm cách nhau để còn chỗ thêm control):
//   1000-1009 : control dùng chung nhiều dialog (combobox ngôn ngữ)
//   1010-1019 : màn hình chính
//   1020-1039 : đăng nhập
//   1040-1059 : chọn database / bảng
//   1060-1079 : tạo / sửa cấu trúc bảng
//   1080-1099 : thêm / sửa nhân viên
// Một ID có thể dùng ở NHIỀU dialog (vd IDC_CMB_LANGUAGE), nhưng MỘT SỐ chỉ được gán cho MỘT tên.
//
#define IDD_NHANVIENMANAGER_DIALOG      102
#define IDR_MAINFRAME                   128
#define IDD_DLG_LOGIN                   130
#define IDD_DLG_CONNECT_DB              132
#define IDD_DLG_TABLE_SCHEMA            134
#define IDD_DLG_NHANVIEN_INFO           136

// ----- Dùng chung -----
#define IDC_LBL_LANGUAGE                1000
#define IDC_CMB_LANGUAGE                1001

// ----- Màn hình chính -----
#define IDC_LIST_NHANVIEN               1010
#define IDC_BTN_ADD                     1011
#define IDC_BTN_EDIT                    1012
#define IDC_BTN_DELETE                  1013

// ----- Đăng nhập -----
#define IDC_LBL_HOST                    1020
#define IDC_EDIT_HOST                   1021
#define IDC_LBL_PORT                    1022
#define IDC_EDIT_PORT                   1023
#define IDC_LBL_USER                    1024
#define IDC_EDIT_USER                   1025
#define IDC_LBL_PASS                    1026
#define IDC_EDIT_PASS                   1027
#define IDC_BTN_CONNECT                 1028

// ----- Chọn database / bảng (IDC_LBL_TABLENAME, IDC_EDIT_TABLENAME dùng chung với dialog cấu trúc bảng) -----
#define IDC_LBL_DBNAME                  1040
#define IDC_EDIT_DBNAME                 1041
#define IDC_LBL_TABLENAME               1042
#define IDC_EDIT_TABLENAME              1043
#define IDC_BTN_CHECK                   1044
#define IDC_BTN_CREATE_TABLE            1045
#define IDC_BTN_EDIT_TABLE              1046

// ----- Tạo / sửa cấu trúc bảng -----
#define IDC_LIST_FIELDS                 1060
#define IDC_LBL_FIELDNAME               1061
#define IDC_EDIT_FIELDNAME              1062
#define IDC_LBL_DATATYPE                1063
#define IDC_CMB_DATATYPE                1064
#define IDC_CHK_NOTNULL                 1065
#define IDC_CHK_PK                      1066
#define IDC_BTN_ADD_FIELD               1067
#define IDC_BTN_EDIT_FIELD              1068
#define IDC_BTN_DEL_FIELD               1069

// ----- Thêm / sửa nhân viên -----
#define IDC_LBL_ID                      1080
#define IDC_STATIC_ID                   1081
#define IDC_LBL_ACCOUNT                 1082
#define IDC_EDIT_ACCOUNT                1083
#define IDC_LBL_HOTEN                   1084
#define IDC_EDIT_HOTEN                  1085
#define IDC_LBL_QUEQUAN                 1086
#define IDC_EDIT_QUEQUAN                1087
#define IDC_LBL_NGAYSINH                1088
#define IDC_DTP_NGAYSINH                1089
#define IDC_LBL_GIOITINH                1090
#define IDC_RADIO_NAM                   1091
#define IDC_RADIO_NU                    1092
#define IDC_LBL_TRUONGHOC               1093
#define IDC_EDIT_TRUONGHOC              1094

// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        138
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1100
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
