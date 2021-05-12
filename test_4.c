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
  char error_server1[20];
  char error_server2[20];
  int result;

  int error = -1;//'-"の時の条件分岐の方法が分からなかったため'-1'で代用
  long int error_time1 = 0;
  long int error_time2 = 0;
  bool error_flag1 = false;
  bool error_flag2 = false;
  
  int N=strtod(argv[1],NULL); //故障と判定する連続回数
  int count1=0; //連続した故障数(server1)
  int count2=0; //連続した故障数(server2)

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
    
    if(strcmp(server, error_server1) == 0 && result == error){ //連続したエラーを記録(server1)
      count1++;
    }  

    if(strcmp(server, error_server2) == 0 && result == error){ //連続したエラーを記録(server2)
      count2++;
    } 

    if(result == error && count1 == 0){ //最初のエラーを記録(server1)
      printf("first server:first timeout \n");
      for(int i=0;i<20;i++){
        error_server1[i] = server[i];
      }
      error_time1 = time;
      count1++;
      error_flag1 = true;
    }

    if(strcmp(server, error_server1) && result == error && count2 == 0){ //最初のエラーを記録(server2)
      printf("second server:first timeout \n");
      for(int i=0;i<20;i++){
        error_server2[i] = server[i];
      }
      error_time2 = time;
      count2++;
      error_flag2 = true;
    }

    if(strcmp(server, error_server1) == 0 && result != error && error_flag1 == true){ //ping応答が復活したのでcountリセット(server1)
      printf("reset\n");
      count1 = 0;
      error_flag1 = false;
    }
     
    if(strcmp(server, error_server2) == 0 && result != error && error_flag2 == true){ //ping応答が復活したのでcountリセット(server2)
      printf("reset\n");
      count2 = 0;
      error_flag2 = false;
    }
   
    if(count1 >= N && count2 >= N){ //同サブネットのサーバがN回以上連続でタイムアウトしたら故障期間を出力
      printf("broken \n");
      printf("server:%s 故障期間：%ld〜%ld \n", error_server1, error_time1, time);
      printf("server:%s 故障期間：%ld〜%ld \n", error_server2, error_time2, time);
      break;
    }

  }

  fclose( fp );
}
