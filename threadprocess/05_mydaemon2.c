#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void touchfile( int num ) {
    //创建文件
    char *homedir = getenv( "HOME" );
    char  strFileName[256] = {0};
    sprintf( strFileName, "%s/log/yourdaemon.%ld", homedir, time( NULL ) );
    int fd = open( strFileName, O_RDONLY | O_CREAT, 0666 );
    if ( fd < 0 ) {
        perror( "open err" );
    }
    if ( fd > 0 ) {
        close( fd );
    }
}

int main() {
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset( &act.sa_mask );
    act.sa_handler = touchfile;
    sigaction( SIGALRM, &act, NULL ); //注册捕捉
    struct itimerval it = {{60, 0}, {5, 0}};
    setitimer( ITIMER_REAL, &it, NULL );
    while ( 1 ) {
        sleep( 1 );
    }
    return 0;
}
