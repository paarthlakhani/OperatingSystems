#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>

/*

    gcc foo.c -O -static -o foo
    objdump -d foo
*/

void draw_me()
{
   // puts("\n");
   // puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    // int file_des = syscall(SYS_open,"draw_me.txt",O_RDWR | O_APPEND, O_RDWR);
    //int file_des = open("draw_me.txt", O_RDWR | O_CREAT | O_APPEND );
    //int file_des = syscall(SYS_open,"draw_me.txt",O_RDWR | O_CREAT | O_APPEND);//, O_RDWR);
    int file_des = syscall(SYS_open,"draw_me.txt",3 | 6 | 4);//, O_RDWR);
    const char* name = "paarth";
    long result = syscall(SYS_write, file_des, name, 6);
}

int main()
{
    draw_me();
    /*puts("\n");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYY               YYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");
    puts("    YYYYYYYYYYYYYYYYYYYYYYY    ");*/
    //puts("\n");
}