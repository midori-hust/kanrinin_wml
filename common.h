/***********commmon.h ***************/

#define MEMBER_MAX 200
#define AKICODE_TBL_NAME "akicode.tbl"
#define CODEDATA_TBL_NAME "codedata.tbl"
#define KEISOKU_TBL_NAME "keisoku.tbl"

#define TRUE   1
#define FALSE  0
#define OK     0
#define CANCEL 1
#define NG    -1

/* tinh toan table data */
struct KEISOKU_TBL{
  int kaiin_code; /* ma code cua hoi vien */
  int count; /* so lan tinh toan */
  char first_date[9]; /* ngay dau tien */
  int first_data; /* du lieu lan dau*/
  char max_date[9]; /* ngay ghi lai ky luc cao nhat */
  int max_data; /* du lieu ngay ghi lai ky luc cao nhat*/
  char soku_date[9]; /*ngay do moi nhat*/
  int soku_data[10]; /*du lieu do*/
};

/* du lieu do luong nhap vao*/
struct KEISOKU_INPUT{
  int huka; /* trach nhiem*/
  int set; 
  int kaisuu; /*so lan*/
};
