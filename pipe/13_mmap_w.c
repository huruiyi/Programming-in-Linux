#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct student {
    int  num;
    char sname[20];
};

int main( int argc, char *argv[] ) {
    if ( argc != 2 ) {
        printf( "./a.out filename\n" );
        return -1;
    }
    int fd = open( argv[1], O_RDWR );
    if ( fd < 0 ) {
        perror( "open err" );
        return -1;
    }
    struct student *pstu = mmap( NULL, sizeof( struct student ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if ( pstu == MAP_FAILED ) {
        perror( "mmap err" );
        return -1;
    }
    close( fd );
    int num = 1000;
    //对内存区域进行操作
    while ( 1 ) {
        pstu->num = num++;
        sprintf( pstu->sname, "xiaoming%04d", pstu->num );
        sleep( 1 );
    }

    if ( munmap( pstu, sizeof( struct student ) ) < 0 ) {
        perror( "munmap err" );
        return -1;
    }
    return 0;
}