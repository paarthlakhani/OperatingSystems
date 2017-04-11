#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

// int open(const char *path, int oflags);
// ssize_t read(int fildes, void *buf, size_t nbytes);


int main(int argc, char* argv[])
{
    int fd2;
    fd2 = open("/dev/sleepy0", O_RDONLY);

    if(fd2 < 0)
    {
        printf("Problem loading the fd2\n");
        printf("The error in fd2 is: %d" , fd2);
        exit(-1);
    }
    printf("File fd2 has been loaded: %d\n" , fd2);

    int length = sizeof(int);
    void *read_buf = malloc(length);
    ssize_t bytes_read = read(fd2, read_buf, length);
    close(fd2);
}