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
  char buf[1024];

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
      printf("変換前 %s",buf);
      for (i=0; i< numrcv; i++){ // bufの中の小文字を大文字に変換
	if(isalpha(buf[i])) buf[i] = toupper(buf[i]);
      }
      // パケットの送信
      write(dstSocket, buf, 1024);
      fprintf(stdout,"→ 変換後 %s \n",buf);
    }
  }
  return(0);
}
