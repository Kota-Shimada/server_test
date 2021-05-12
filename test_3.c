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
  long int start_time = 0;
  
  int m = strtod(argv[1],NULL); //直近m回
  int t = strtod(argv[2],NULL); //過負荷状態とみなす応答時間tミリ秒
  int data[10] = {};
  bool count_flag;
  int count = 0;
  int sum = 0;
  double ave = 0;

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
    
    if(strcmp(server, error_server) == 0 && result > 0 && count_flag == true){ //連続して応答時間を記録
      if(count < m) data[count] = result;
      else{
        for(int i=0;i<m-1;i++){
          data[i] = data[i+1];
        }
        data[m-1] = result;
      }
      count++;
    }

    if(count >= m){
      for(int i=0;i<m;i++){
        sum += data[i];
      }

      ave = sum/m;

      if(ave > t){
        printf("server:%s  過負荷状態期間%ld〜%ld", server, start_time, time);
        break;
      }
    }

    if(result > t){ //最初にtを超えたとき
      for(int i=0;i<20;i++){
        error_server[i] = server[i];
      }
      start_time = time; 
      data[count] = result;
      count ++;
      count_flag = true;
    }
  }

  fclose( fp );
}
