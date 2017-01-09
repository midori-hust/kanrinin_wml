/********************************************/
/*             sakujyo.c                    */
/*       Quan ly xoa dang ky                */
/********************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "main.h"
#include "sakujyo.h"

extern int akicode_tbl[MEMBER_MAX + 1];
extern int codedata_tbl[MEMBER_MAX];
extern struct KEISOKU_TBL kojin_keisoku_tbl;

/********************************************/
/*          Main routine                    */
/*    Parameter: none                       */
/*    Return: 0: OK                         */
/*           -1: NG                         */
/********************************************/
int touroku_sakujyo(void) {
  int ret;
  int loop = TRUE;
  int kaiin_code;
  char work[28];

  while(loop) {
    printf("\n Hay nhap ma nhan vien");
    printf("\n ?");

    work[0] = '\0';
    scanf("%s", work);

    if(strspn(work, "1234567890") < strlen(work)) {
      printf("\nHay nhap so khac so nay");
      continue;
    }

    kaiin_code = atoi(work);
    if(kaiin_code > MEMBER_MAX || kaiin_code <= 0) {
      printf("\n Nhap sai roi");
      continue;
    }

    if((ret = codedata_tbl_delete(kaiin_code)) == OK) {
      if((ret = akicode_tbl_add(kaiin_code)) == OK) {
	loop = FALSE;
      }
    } else {
      loop = FALSE;
    }
  }

  if(ret == OK) {
    printf("\nDa xoa xong");
  }
    
  return ret;
}

/********************************************/
/*          Code data doi chieu             */
/*    Parameter: none                       */
/*    Return: 0: OK                         */
/*            1: CANCEL                     */
/*           -1: NG                         */
/********************************************/
static int codedata_tbl_delete(int kaiin_code) {
  int ret;
  int i;
  char msg[64];
  FILE *fp;
  char *fname = CODEDATA_TBL_NAME;

  if((fp = fopen(fname, "r+b")) == NULL) {
    printf("\nMo file doi chieu code data loi");
    return NG;
  }

  if((ret = fread((char*) codedata_tbl, sizeof(codedata_tbl), 1, fp)) != 1) {
    printf("\nMo file doc doi chieu code data loi");
    fclose(fp);
    return NG;
  }

  if(codedata_tbl[kaiin_code - 1] == 0) {
    printf("\nMa nhan vien nay chua duoc dang ky");
    fclose(fp);
    return CANCEL;
  }

  if((ret = kojin_data_read(kaiin_code)) == NG) {
    fclose(fp);
    return ret;
  }

  kojin_data_disp(kaiin_code, "\n ** Xoa du lieu **");
  sprintf(msg, "\n\nXoa du lieu o tren co duoc khong? (Y/N)");

  if((ret = kakunin_input(msg)) == OK) {
    if((ret = kojin_data_delete(kaiin_code)) == OK) {
      for(i = 0; i < MEMBER_MAX; i++) {
	if(codedata_tbl[i] > codedata_tbl[kaiin_code - 1]) {
	  codedata_tbl[i]--;
	}
      }

      codedata_tbl[kaiin_code - 1] = 0;

      if((ret = fseek(fp, 0L, SEEK_SET)) == OK) {
	if((ret = fwrite((char*) codedata_tbl, sizeof(codedata_tbl), 1, fp)) != 1) {
	  printf("\nViet file doi chieu code data loi");
	  ret = NG;
	} else {
	  ret = OK;
	}
      } else {
	printf("\nSEEK file doi chieu code data loi");
	ret = NG;
      }
    }
  } else {
    ret = CANCEL;
  }
  
  fclose(fp);
  return ret;
}

static int kojin_data_delete(int kaiin_code) {
  int ret;
  int i;
  FILE *fp;
  FILE *tmp;
  char *fname = KEISOKU_TBL_NAME;
  char *tmpfl = "keisoku.tmp";

  if((fp = fopen(fname, "rb")) == NULL) {
    printf("\nMo file doi chieu du lieu loi");
    return NG;
  }

  if((tmp = fopen(tmpfl, "w+b")) == NULL) {
    printf("\nMo file tam loi");
    return NG;
  }

  i = 0;
  while(1) {
    if((ret = fread((char*) &kojin_keisoku_tbl, sizeof(kojin_keisoku_tbl), 1, fp)) != 1) {
      if(ferror(fp) != 0) {
	printf("\nDoc file loi");
	ret = NG;
      } else {
	if(feof(fp) == 0) {
	  printf("\nDoc file loi");
	  ret = NG;
	} else {
	  ret = OK;
	}
      }
      break;
    }

    if(kaiin_code == kojin_keisoku_tbl.kaiin_code) {
      continue;
    }

    if((ret = fwrite((char*) &kojin_keisoku_tbl, sizeof(kojin_keisoku_tbl), 1, tmp)) != 1) {
      printf("\nViet file loi");
      ret = NG;
      break;
    }
    i++;
  }

  fclose(tmp);
  fclose(fp);

  if(ret == OK) {
    if((ret = remove(fname)) != 0) {
      printf("\nXoa file moi");
      ret = NG;
    } else {
      if(i > 0) {
	if((ret = rename(tmpfl, fname)) != 0) {
	  printf("\nRename file loi");
	  ret = NG;
	}
      } else {
	remove(tmpfl);
      }
    }
  } else {
    remove(tmpfl);
  }
  return ret;
}

static int akicode_tbl_add(int kaiin_code) {
  int ret;
  int cnt;
  FILE *fp;
  char *fname = AKICODE_TBL_NAME;

  if((ret = akicode_tbl_read()) == NG) {
    return ret;
  }

  cnt = akicode_tbl[0];
  akicode_tbl[cnt + 1] = kaiin_code;
  akicode_tbl[0] = cnt + 1;

  if((fp = fopen(fname, "w+b")) == NULL) {
    printf("\nMo file code trong loi");
    return NG;
  }

  if((ret = fwrite((char*) akicode_tbl, sizeof(int) * (akicode_tbl[0] + 1), 1, fp)) != 1) {
    printf("\nGhi file loi");
    ret = NG;
  } else {
    ret = OK;
  }

  fclose(fp);
  return ret;
}
