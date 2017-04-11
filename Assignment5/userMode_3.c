#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int fd1;
    fd1 = open("/dev/sleepy0", O_RDWR);

    if(fd1 < 0)
    {
        printf("Problem loading the fd1\n");
        printf("The error in fd1 is: %d" , fd1);
        exit(-1);
    }
    printf("File fd1 has been loaded: %d\n" , fd1);

    int *buf = (int *)malloc(sizeof(int));
    *buf = 10;
    size_t length = sizeof(*buf);
    int bytes_written = write(fd1, buf, length);
    printf("User Code: Bytes written are: %d\n", bytes_written);
    printf("Error meaning is: %s \n", strerror(errno));
    printf("Error Code is: %d \n", errno);

    close(fd1);
}
