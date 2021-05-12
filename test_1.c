#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    
    if(strcmp(error_server ,server) == 0){ //記録したサーバからping応答があれば故障期間を出力
      printf("restored \n");
      printf("broken server : %s ,故障期間：%ld〜%ld\n", server, error_time, time); //故障状態のサーバアドレスとサーバの故障期間を出力
      break;
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
