/************main.c*******************/
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "main.h"
#include "nyuukai.h"
#include "keisoku.h"
#include "sakujyo.h"

/******************************/
/*du lieu chung               */
/******************************/

/*ma trong**/
int akicode_tbl[MEMBER_MAX + 1];

/* doi chieu du lieu + ma code */
int codedata_tbl[MEMBER_MAX];

/* hien thi su do luong cua moi ca nhan*/
struct KEISOKU_TBL kojin_keisoku_tbl;

/* hien thi du lieu do luong sap xep */
struct KEISOKU_TBL sort_keisoku_tbl[MEMBER_MAX];

/******************************/
/* kiem tra hoi vien          */
/******************************/

int main(void) {
  int loop = TRUE;
  char work[128];
  FILE *fp;
  char *fname1 = AKICODE_TBL_NAME;
  char *fname2 = CODEDATA_TBL_NAME;
  int i;

  /* Open akicode */

  if((fp = fopen(fname1,"rb")) == NULL) {
    printf("\nCan't open file akicode");
    akicode_tbl_create();
  } else {
    fclose(fp);
  }
  
  /* Open code data */

  if((fp = fopen(fname2,"rb")) == NULL) {
    printf("\nCan't open file code data");
    codedata_tbl_create();
  } else {
    fclose(fp);
  }

  while(loop) {
    akicode_tbl[0]=0;
    for(i=0; i<MEMBER_MAX; i++) {
      akicode_tbl[i+1]=0;
      codedata_tbl[i]=0;
      sort_keisoku_tbl[i] = init_kojin_keisoku_tbl();
    }
    kojin_keisoku_tbl = init_kojin_keisoku_tbl();
    
    printf("\n");
    printf("\n *******************************************");
    printf("\n Chuong trinh quan ly cau lac bo the thao");
    printf("\n *******************************************");
    printf("\n Chon thao tac muon xu ly ");
    printf("\n 1: Dang ky ");
    printf("\n 2: Nhap record ");
    printf("\n 3: Xoa dang ky");
    printf("\n E: ket thuc");
    printf("\n ? ");

    work[0] = '\0';
    scanf("%s", work);
    
    if(strlen(work) != 1) {
      printf("Input again!");
      continue;
    }

    switch(work[0]) {
    case '1':
      nyuukai_touroku();
      break;
    case '2':
      keisoku_input();
      break;
    case '3':
      touroku_sakujyo();
      break;
    case 'e':
    case 'E':
      loop = FALSE;
      break;
    default:
      printf("Input miss!");
      break;
    }
  }
  return OK;
}

static int akicode_tbl_create(void){
  int  ret;
  int i;
  FILE *fp;
  char *fname = AKICODE_TBL_NAME;

  akicode_tbl[0] = MEMBER_MAX;
  for(i = 1; i< MEMBER_MAX; i++) {
    akicode_tbl[i] = i;
  }

  if((fp = fopen(fname, "w+b")) == NULL) {
    printf("Can't open file /n ");
    return NG;
  }

  if(( ret = fwrite((char *)akicode_tbl, sizeof(akicode_tbl), 1 ,fp )) != 1) {
    printf("/n Write error! ");
    ret = NG;
  } else {
    ret = OK;
  }
  fclose(fp);
  return ret;
}

static int codedata_tbl_create(void){
  int  ret;
  int i;
  FILE *fp;
  char *fname = CODEDATA_TBL_NAME;
  
  for(i = 1; i< MEMBER_MAX; i++) {
    akicode_tbl[i] = 0;
  }

  if((fp = fopen(fname, "w+b")) == NULL) {
    printf("Can't open file /n ");
    return NG;
  }

  if(( ret = fwrite((char *)codedata_tbl, sizeof(codedata_tbl), 1 ,fp )) != 1) {
    printf("/n Write error! ");
    ret = NG;
  } else {
    ret = OK;
  }
  fclose(fp);
  return ret;
}

int akicode_tbl_read(void) {
  int ret;
  int i;
  FILE *fp;
  char *fname = AKICODE_TBL_NAME;
 
  if((fp= fopen(fname, "rb"))== NULL) {
    printf("/n Open file akicode error!");
    return NG;
  }
  
  for( i = 0; i< MEMBER_MAX + 1; i++) {
    if((ret = fread((char *)&akicode_tbl[i], sizeof(int), 1, fp)) != 1) {
      if(ferror(fp) != 0) {
	printf("/n READ error!");
	ret = NG; 
      } else {
	if(feof(fp) == 0) {
	  printf("\n READ error");
          ret = NG;
	} else {
	  ret = OK;
	}
      }
      break;
    }
  }
  
  fclose(fp);
  return ret;
}
/* Xac nhan viec xu ly*/
int kakunin_input(char *msg){
  int ret;
  int loop = TRUE;
  char work[128];
  
  while(loop) {
    printf("\n%s", msg);
    printf("\n ?");
    
    work[0] = '\n';
    scanf("%s", work);

    if(strlen(work) != 1) {
      printf("/n Input error");
      continue;
    }
    
    switch(work[0]) {
    case 'y':
    case 'Y':
      ret = OK; 
      loop = FALSE;
      break;
    case 'n':
    case 'N':
      ret = NG;
      loop = FALSE;
      break;
    default: 
      printf("\n Input miss! ");
      break;
    }
  }
  return ret;
}

/* Doc du lieu tinh toan ca nhan */

int kojin_data_read(int kaiin_code) {
  int ret;
  FILE *fp;
  long fptr;
  char *fname = KEISOKU_TBL_NAME;
  
  if((fp = fopen(fname, "rb")) == NULL) {
    printf("/n Can't open file keisoku !");
    return NG;
  }

  fptr = (codedata_tbl[kaiin_code -1]) * sizeof(struct KEISOKU_TBL);
  
  if((ret = fseek(fp, fptr, SEEK_SET)) != OK) {
    printf("\n SEEK error!");
    fclose(fp);
    return NG;
  }
  if((ret = fread((char *)&kojin_keisoku_tbl, sizeof(kojin_keisoku_tbl),1,fp)) !=1) {
    printf("\n Open error! ");
    ret = NG;
  } else {
    ret = OK;
  }
  
  fclose(fp);

  return ret;
}

/*Xu ly hien thi tinh toan ca nhan */
void kojin_data_disp(int kaiin_code, char *msg) {
  printf("\n%s", msg);
  printf("\n Ma hoi vien %3d", kaiin_code);
  
  if(kojin_keisoku_tbl.count != 0){
    printf("\n\n so lan tinh toan    du lieu ngay gan nhat");
    printf("Du lieu ngay cao nhat    du lieu ngay moi nhat");
    
    printf("\n %3d", kojin_keisoku_tbl.count);
    
    printf("\n %4.4s-%2.2s-%2.2s",
	   &kojin_keisoku_tbl.first_date[0],
	   &kojin_keisoku_tbl.first_date[4],
	   &kojin_keisoku_tbl.first_date[6]);

    printf("\n %4d", kojin_keisoku_tbl.first_data);
    
    printf("\n %4.4s-%2.2s-%2.2s",
	   &kojin_keisoku_tbl.max_date[0],
	   &kojin_keisoku_tbl.max_date[4],
	   &kojin_keisoku_tbl.max_date[6]);
  
    printf("\n %3d", kojin_keisoku_tbl.max_data);
    
    printf("\n %4.4s-%2.2s-%2.2s",
	   &kojin_keisoku_tbl.soku_date[0],
	   &kojin_keisoku_tbl.soku_date[4],
	   &kojin_keisoku_tbl.soku_date[6]);

    printf("\n %3d", kojin_keisoku_tbl.soku_data[0]);
    
    printf("\n\n Lan1 Lan2 Lan3 Lan4 ");
    printf("Lan5 Lan6 Lan7 Lan8 Lan9\n");
    
    printf("%4d", kojin_keisoku_tbl.soku_data[1]);
    printf("%4d", kojin_keisoku_tbl.soku_data[2]);
    printf("%4d", kojin_keisoku_tbl.soku_data[3]);
    printf("%4d", kojin_keisoku_tbl.soku_data[4]);
    printf("%4d", kojin_keisoku_tbl.soku_data[5]);
    printf("%4d", kojin_keisoku_tbl.soku_data[6]);
    printf("%4d", kojin_keisoku_tbl.soku_data[7]);
    printf("%4d", kojin_keisoku_tbl.soku_data[8]);
    printf("%4d", kojin_keisoku_tbl.soku_data[9]);
  } else {
    printf(" Khong co data");
    return;
  }
  return;
}

struct KEISOKU_TBL init_kojin_keisoku_tbl(void) {
  static struct KEISOKU_TBL tbl = {
    0, 0, "  ", 0, "  ", 0, " ", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  return tbl;
}
