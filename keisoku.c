/********keisoku.c*********/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "main.h"
#include "keisoku.h"

extern int akicode_tbl[MEMBER_MAX + 1];
extern int codedata_tbl[MEMBER_MAX];
extern struct KEISOKU_TBL kojin_keisoku_tbl;
extern struct KEISOKU_TBL sort_keisoku_tbl[MEMBER_MAX];

static struct KEISOKU_INPUT keisoku_indata[ 5 ];

int keisoku_input(void){
  int ret;
  int i;
  int kaiin_code;
  int undou_sisuu;
  char msg[ 64 ];
  char keisoku_date[ 9 ];
  
  if(( ret = akicode_tbl_read())== NG ) {
    return ret;
  }
  
  if( akicode_tbl[ 0 ] >= MEMBER_MAX ) {
    printf("Ima, kaiin ga inai!");
    return NG;
  }
  if((ret = codedata_tbl_read()) == NG){
    return ret;
  }
  kaiin_code_input(&kaiin_code);
  keisoku_date_input(keisoku_date);

  for( i = 0; i < 5; i++){
    keisoku_data_input(i);
  }
  
  keisoku_data_disp(kaiin_code, keisoku_date);
  strcpy(msg, "\n Input oki(Y/N)" );
  
  if((ret = kakunin_input( msg )) != OK){
    return NG;
  }

  if((ret = kojin_data_read(kaiin_code)) == NG){
    return ret;
  }

  undou_sisuu_keisan(&undou_sisuu);
  kojin_data_update(kaiin_code, keisoku_date, undou_sisuu);
  kojin_data_disp(kaiin_code, "\n du lieu ket qua");

  while(getchar() != '\n');
  printf("\n AN bat ky tu nao de tro ve! !");
  getchar();

  ret = keisoku_rank();
  
  return ret;
}

static int codedata_tbl_read(void){
  int  ret;
  FILE *fp;
  char *fname = CODEDATA_TBL_NAME;
  
  if((fp = fopen(fname, "rb")) == NULL){
    printf("\n Can't open file code data");
    return NG;
  }
  
  if((ret = fread((char*)codedata_tbl, 
		  sizeof(codedata_tbl), 1, fp)) != 1){
    printf("\n Can't read file code data");
    ret = NG;
  }else{
    ret = OK;
  }
  fclose(fp);
  return ret;
}

static void kaiin_code_input(int *kaiin_code){
  int loop = TRUE;
  char work[128];
  
  while( loop ){
    printf("\n InPut kaiin code ");
    printf("\n ?");
    work[0] = '\0';
    scanf("%s", work);

    if(strspn(work,"1234567890")<strlen(work)){
      printf("\n Value error!");
      continue;
    }
    *kaiin_code = atoi(work);
    if(*kaiin_code > MEMBER_MAX || *kaiin_code <= 0){
      printf("\n kaiin code miss!");
      continue;
    }
    if(codedata_tbl[ *kaiin_code -1] == 0){
      printf("\n kaiin code chua duoc dang ky");
      continue;
    }
    break;
  }
  return;
}

static void keisoku_date_input(char *keisoku_date){
  int loop = TRUE;
  int chk_date;
  char conv[ 3 ];
  char work[ 128 ];
  
  while( loop ){
    printf("\n Nhap ngay de dinh vao (YYYYMMDD)");
    printf("\n ?");
    
    work[ 0 ] = '\0';
    scanf("%s", work);
    if(strlen(work) != 8){
      printf("\n Iuput day miss");
      continue;
    }
    if(strspn(work, "1234567890") < strlen(work)){
      printf("\n value input error! ");
      continue;
    }
    conv[0] = work[4];
    conv[1] = work[5];
    conv[2] = '\0';
    
    chk_date = atoi(conv);
    if(chk_date >12|| chk_date <1){
      printf("\n thang nhap sai");
      continue;
    }
    conv[0] = work[6];
    conv[1] = work[7];
    conv[2] = '\0';
    
    chk_date = atoi(conv);
    if(chk_date >31 || chk_date < 1){
      printf("\n ngay nhap sai");
      continue;
    } 
    break;
  }
  strcpy(keisoku_date, work);
  return;
}

static void keisoku_data_input(int idx){
  int i;
  int loop = TRUE;
  char work[3][128];

  while(loop){
    printf("\n nhap vao do luong %d van dong", idx +1);
    printf("\n So tai(999) set (100) so lan(100)");
    printf("\n ? ");
    
    work[1][0]= '\0';
    work[2][0]= '\0';
    work[0][0]= '\0';
    
    scanf("%s%s%s", work[0],work[1],work[2]);

    for(i = 0; i < 3; i++){

      if(strlen(work[i]) > 3){
	printf("\n Input miss");
	break;
      }

      if(strspn(work[ i ], "1234567890") < strlen(work[ i ] ) ){
	printf("\n Value input miss!");
	break;
      }
      
      if(i != 0) {
	if(atoi(work[i]) >100){
	  printf("\n Value input error");
	  break;
	}
      }

    }
    if(i<3) continue;
    break;
  }
  
  keisoku_indata[ idx ].huka = atoi( work[ 0 ]);
  keisoku_indata[ idx ].set = atoi(work[1]); 
  keisoku_indata[ idx ].kaisuu = atoi(work[2]);
}

static void keisoku_data_disp(int kaiin_code, char *keisoku_date) {
  int i;
  printf("\n nhap du lieu can tinh toan");
  printf("\n kaiin code %3d", kaiin_code);
  printf("\n Ngay in %4.4s-%2.2s-%2.2s",
	 (keisoku_date + 0), (keisoku_date + 4), (keisoku_date + 6));
  printf("\n\n hoat dong luong tai set so lan");
  for( i = 0; i < 5 ; i++) {
    printf("\n %d  %3d %3d %3d", i+1,
	   keisoku_indata[ i ].huka,
	   keisoku_indata[ i ].set,
	   keisoku_indata[ i ].kaisuu);
  }
  return;
}

static void undou_sisuu_keisan(int *undou_sisuu) {
  int i;
  double sisuu;
  double sisuu_total;
  static double haku_sisuu[5]= {0.24, 0.36, 0.52, 1.05, 2.13};
  
  sisuu_total = 0.0;
  for(i = 0; i < 5; i++ ) {
    if(keisoku_indata[i].kaisuu <= 1){
      continue;
    }
    sisuu = haku_sisuu[ i ]* (double) keisoku_indata[i].huka *
      (double)keisoku_indata[ i ].set * 
      ((pow( (double)keisoku_indata[ i ].kaisuu, 2.0))/
       ((double)keisoku_indata[ i ].kaisuu - 1.0));
    sisuu_total += sisuu;
  }
  *undou_sisuu = (int)sqrt(sisuu_total);
  return;
}
static int kojin_data_update(int kaiin_code, char *keisoku_date, 
			     int undou_sisuu){
  int ret;
  long fptr;
  FILE *fp;
  char *fname = KEISOKU_TBL_NAME;
  int i;

  if (kojin_keisoku_tbl.count <= 0 ) {
    strcpy(kojin_keisoku_tbl.first_date, keisoku_date);
    kojin_keisoku_tbl.first_data = undou_sisuu;
    strcpy(kojin_keisoku_tbl.max_date, keisoku_date);
    kojin_keisoku_tbl.max_data = undou_sisuu;
  }
  else{
    if (kojin_keisoku_tbl.max_data < undou_sisuu){
      strcpy(kojin_keisoku_tbl.max_date, keisoku_date);
      kojin_keisoku_tbl.max_data = undou_sisuu;
    }
    for (i = sizeof kojin_keisoku_tbl.soku_data / sizeof(int) -1; i  >  0; i--){
      kojin_keisoku_tbl.soku_data[ i ] = kojin_keisoku_tbl.soku_data[ i - 1 ];
    }
  }

  strcpy(kojin_keisoku_tbl.soku_date, keisoku_date);
  kojin_keisoku_tbl.soku_data[0] = undou_sisuu;
  kojin_keisoku_tbl.count++;
  
  if ((fp = fopen(fname, "r+b")) == NULL){
    printf("\n Can't open file Keisoku");
    return NG;
  }

  fptr = (codedata_tbl[kaiin_code -1] -1) * sizeof(struct KEISOKU_TBL);

  if ( (ret = fseek(fp, fptr, SEEK_SET)))  {
    printf("\n SEEK error");
    fclose(fp);
    return NG;
  }

  if ((ret = fwrite((char *)&kojin_keisoku_tbl, sizeof(kojin_keisoku_tbl),
                    1,fp )) != 1) {
    printf("\n wwrite error! ");
    ret = NG;
  }
  else{
    ret = OK;
  }
  fclose( fp );

  return ret;
}

static int keisoku_rank(void){
  int ret;
  int i;
  FILE *fp;
  char *fname = KEISOKU_TBL_NAME;

  if ((fp = fopen(fname, "rb")) == NULL) {
    printf("\n Can't open keisoku");
    return NG;
  }

  i = 0;
  for (; ; ) {
    if ((ret = fread((char *)&sort_keisoku_tbl[ i ],
		     sizeof(struct KEISOKU_TBL), 1,fp)) != 1){
      if (ferror(fp) != 0 ){
        printf("\n Read error ");
        ret = NG;
      }
      else{
        if (feof( fp ) == 0){
          printf("\n Read error ");
          ret = NG;
        }
        else{
          ret = OK;
        }
      }
    }
    if (ret == NG) {
      break;
    }
    if (sort_keisoku_tbl[i].count != 0)  {
      i++;
    }

    if (ret == OK){
      break;
    }

  }
  fclose(fp);
  if (ret == OK) {
    keisoku_data_sort( i );
    keisoku_sort_disp(i);
  }
  return ret;
}

static void keisoku_data_sort(int cnt){
  int  i;
  int  j;
  struct KEISOKU_TBL work;

  for (i = 0; i < cnt - 1; i++ ){
    for (j = 0; j <cnt ; j++){
      if (sort_keisoku_tbl[ i ].max_data <
          sort_keisoku_tbl[ j ].max_data ){
        work = sort_keisoku_tbl[ i ];
        sort_keisoku_tbl[ i ] = sort_keisoku_tbl[ j ];
        sort_keisoku_tbl[j] = work;
      }
    }
  }
  return;
}

static void keisoku_sort_disp(int cnt) {
  int i;
  printf("\n\nThu tu bang ");
  printf("\n\nThu tu    Ma nhan vien     Data cao nhat   Ngay dinh kem");
  for (i = 0; i < cnt; i++){
    if (i >= 10 ) {
      break;
    }
    printf("\n\n%2d     %3d              %4d             %4.4s-%2.2s-%2.2s", i + 1, 
	   sort_keisoku_tbl[ i ].kaiin_code,
	   sort_keisoku_tbl[ i ].max_data,
	   &sort_keisoku_tbl[ i ].max_date[0],
	   &sort_keisoku_tbl[ i ].max_date[4],
	   &sort_keisoku_tbl[ i ].max_date[6]);
  }
  return;
}
