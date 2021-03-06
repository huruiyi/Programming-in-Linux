#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

void catchsig( int num ) {
    printf( "cat %d sig\n", num );
}

int main() {
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = catchsig;
    sigemptyset( &act.sa_mask );
    sigaction( SIGALRM, &act, NULL ); //捕获闹钟的信号

    //设置定时器
    struct itimerval it = {{3, 0}, {4, 0}}; //4s后第一次闹钟，之后每隔3s一次闹钟
    setitimer( ITIMER_REAL, &it, NULL );    //设置闹钟
    while ( 1 ) {
        printf( "lai da wo ya!\n" );
        sleep( 1 );
    }
    return 0;
}
