#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  FILE *fp;
  char *fname = "log.csv";

  char buf[3][10];
  long int time;
  char server[20];
  char error_server[20];
  int result;

  int error = -1;//'-"の時の条件分岐の方法が分からなかったため'-1'で代用
  long int error_time = 0;
  bool error_flag = false;
  
  int N=strtod(argv[1],NULL); //故障と判定する連続回数
  int count=0; //連続した故障数

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
    
     if(strcmp(server, error_server) == 0 && result == error){ //連続したエラーを記録
      count++;
    }   

    if(result == error && count == 0){ //最初のエラーを記録
      printf("first timeout \n");
      for(int i=0;i<20;i++){
        error_server[i] = server[i];
      }
      error_time = time;
      count++;
      error_flag = true;
    }

    if(strcmp(server, error_server) == 0 && result != error && error_flag == true){ //ping応答が復活したのでcountリセット
      printf("reset\n");
      count = 0;
      error_flag = false;
    }
   
    if(count == N){ //特定のサーバがN回以上連続でタイムアウトしたらエラー時間を計算
      printf("broken \n");
      break;
    }

  }

  fclose( fp );
}
