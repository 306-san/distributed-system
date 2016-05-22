//Linux gcc での　TCP/IP サンプルプログラム（ここからサーバー）
//クライアントから送られてきた文字列を大文字に変換して送り返す
//サーバープログラムを実行してからクライアントプログラムを実行して下さい
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9876 //クライアントプログラムとポート番号を合わせてください
#define BUFF_SIZE 1024

int main(){
  int i;
  int srcSocket; //自分
  int dstSocket; //相手
  // sockaddr_in 構造体
  struct sockaddr_in srcAddr;
  struct sockaddr_in dstAddr;
  int dstAddrSize = sizeof(dstAddr);
  // 各種パラメータ
  int status;
  int numrcv;
  int fd[2]; //pipe用
  char buff[BUFF_SIZE]; //pipe用
  char buf[1024];

  pipe( fd );    //パイプの作成
  while(1){//ループで回すことによって何度でもクライアントからつなぐことができる
    // sockaddr_in 構造体のセット
    bzero((char *)&srcAddr, sizeof(srcAddr));
    srcAddr.sin_port = htons(PORT);
    srcAddr.sin_family = AF_INET;
    srcAddr.sin_addr.s_addr = INADDR_ANY;

    // ソケットの生成（ストリーム型）
    srcSocket = socket(AF_INET, SOCK_STREAM, 0);
    // ソケットのバインド
    bind(srcSocket, (struct sockaddr *)&srcAddr, sizeof(srcAddr));
    // 接続の許可
    listen(srcSocket, 1);

    // 接続の受付け
    printf("接続を待っています\nクライアントプログラムを動かして下さい\n");
    dstSocket = accept(srcSocket, (struct sockaddr *)&dstAddr, &dstAddrSize);
    printf("%s から接続を受けました\n",(char *)inet_ntoa(dstAddr.sin_addr));
    close(srcSocket);

    while(1){
      //パケットの受信
      numrcv = read(dstSocket, buf, 1024);
      if(numrcv ==0 || numrcv ==-1 ){
	close(dstSocket); break;
      }
      if(fork() == 0){
        if( fork()==0 ){
          /* 子プロセスの標準出力をパイプに関連づける */
          close( STDOUT_FILENO );
          dup2( fd[1] , STDOUT_FILENO );
          close( fd[0] );
          execlp("ruby","ruby","./haihuri.rb",buf,NULL);
        }
        /* buffを0で初期化 */
        for( i=0; i<256 ;i++ ){
            buff[i] = 0;
        }
        read(fd[0],buff,BUFF_SIZE);    /* パイプに出力されたデータをbuffに 読み込む */
        // パケットの送信
        write(dstSocket, buff, 1024);
        fprintf(stdout,"→ %s \n",buff);
      }
    }
  }
  return(0);
}
