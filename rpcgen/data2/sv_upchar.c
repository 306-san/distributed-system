#include <string.h>
#include <ctype.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <unistd.h>
#include "strcap.h"
#define BUFF_SIZE 1024

char ** strdate_1_svc(char **arg, struct svc_req *req) {

  static char *result;
  int i;
  int fd[2]; //pipe用
  char buff[BUFF_SIZE]; //pipe用
  sigignore( SIGCLD );
  pipe(fd);    //パイプの作成
  if( fork()==0 ){
    /* 子プロセスの標準出力をパイプに関連づける */
    close( STDOUT_FILENO );
    dup2( fd[1] , STDOUT_FILENO );
    close( fd[0] );
    execlp("ruby","ruby","./haihuri.rb",arg[0],NULL);
  }
  printf("受信文字: %s\n", arg[0]);
  read(fd[0],buff,BUFF_SIZE);    /* パイプに出力されたデータをbuffに 読み込む */
  // arg[0]にexecの値を入れてあげる
  strcpy(arg[0], buff);
  printf("→送信文字: %s\n", arg[0]);
  return(&arg[0]);
}
