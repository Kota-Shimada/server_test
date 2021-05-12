#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void cal_time(long int e_time, long int s_time)
{
  
  int day,hour,min,sec;
  int it = e_time - s_time;

  // 詳細時間計算(未完成）
  sec  = it % 60; it /= 60;
  min  = it % 60; it /= 60;
  hour = it % 24;
  
  // 出力
  printf("故障時間：%02d時間%02d分%02d秒\n",hour,min,sec);
}

int main(){
  FILE *fp;
  char *fname = "log.csv";

  char buf[3][10];
  long int time;
  char server[20];
  char error_server[20];
  int result;

  int error = -1;//'-"の時の条件分岐の方法が分からなかったため'-1'で代用
  long int error_time = 0;

  fp = fopen( fname, "r" );
  if( fp == NULL ){
    printf( "%sファイルが開けません\n", fname );
    return -1;
  }

  printf("\n");

  fscanf(fp, "%[^,],%[^,],%s", buf[0], buf[1], buf[2]);
  printf("%s %s %s\n",buf[0], buf[1], buf[2]);

  while(fscanf(fp, "%ld,%[^,],%d", &time, server, &result) != EOF){
    printf("%ld %s %d\n", time, server, result);
    
    if(strcmp(error_server ,server) == 0){ //記録したエラーサーバからping応答があればエラー時間を計算
      printf("restored \n");
      printf("broken server : %s ", server); //故障状態のサーバアドレスを出力
      cal_time(time, error_time); //サーバの故障時間を出力
    }

    if(result == error){ //エラーを記録
      printf("broken \n");
      for(int i=0;i<20;i++){
        error_server[i] = server[i];
      }
      error_time = time;
    }
  }

  fclose( fp );
}
