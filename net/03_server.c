//实现大小写转换
#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    //1. 创建套接字
    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    //2. 绑定ip和端口
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons( 8889 );
    //inet_pton( AF_INET, "127.0.0.1", &serv.sin_addr.s_addr );
    serv.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) );

    if ( bind( sockfd, (struct sockaddr *) &serv, sizeof( serv ) ) < 0 ) {
        perror( "bind err" );
        exit( EXIT_FAILURE );
    }

    //3. 侦听
    listen( sockfd, 128 );
    //4. 等待连接
    struct sockaddr_in client;
    socklen_t          len = sizeof( client );
    //accept 会阻塞等待客户端连接
    int  cfd = accept( sockfd, (struct sockaddr *) &client, &len );
    char strIP[32];
    printf( "new conn %s:%d\n", inet_ntop( AF_INET, &client.sin_addr.s_addr, strIP, sizeof( strIP ) ), ntohs( client.sin_port ) );
    //5. 通信 read write
    char buf[256] = {0};
    int  i;
    int  ret;
    while ( 1 ) {
        ret = read( cfd, buf, sizeof( buf ) ); //读数据
        if ( ret < 0 ) {
            perror( "read err" );
            return -1;
        } else if ( ret == 0 ) {
            //代表客户端关闭
            printf( "client closed\n" );
            break;
        } else {
            //代表读到数据
            for ( i = 0; i < ret; i++ ) {
                buf[i] = toupper( buf[i] ); //大小写
            }
            write( cfd, buf, ret ); //写回给对端
        }
    }
    //6. 关闭退出
    close( cfd );
    close( sockfd );
    return 0;
}