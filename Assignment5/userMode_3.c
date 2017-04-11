#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int fd1;
    fd1 = open("/dev/sleepy1", O_RDWR);

    if(fd1 < 0)
    {
        printf("Problem loading the fd1\n");
        printf("The error in fd1 is: %d" , fd1);
        exit(-1);
    }
    printf("File fd1 has been loaded: %d\n" , fd1);

    int *buf = (int *)malloc(sizeof(int));
    *buf = 20;
    size_t length = sizeof(*buf);
    ssize_t bytes_written = write(fd1, buf, length);
    printf("User Code: Bytes written are: %zu\n", bytes_written);
    printf("Error Code is: %s \n", strerror(errno));
    printf("Error Code is: %d \n", errno);

    /*void *read_buf = malloc(length);
    ssize_t bytes_read = read(fd1, read_buf, length);
    printf("User Code: Bytes read are: %zu\n", bytes_read);
    printf("User Code: String is: %d\n", *(int *)read_buf);*/

    close(fd1);
}