#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

// int open(const char *path, int oflags);
// ssize_t read(int fildes, void *buf, size_t nbytes);


int main(int argc, char* argv[])
{
    int fd0;
    fd0 = open("/dev/sleepy0", O_RDWR);

    if(fd0 < 0)
    {
        printf("Problem loading the fd0\n");
        printf("The error in fd0 is: %d" , fd0);
        exit(-1);
    }
    printf("File fd0 has been loaded: %d\n" , fd0);

    short *buf = (short *)malloc(sizeof(short));
    *buf = 5;
    size_t length = sizeof(*buf);
    ssize_t bytes_written = write(fd0, buf, length);
    printf("User Code: Bytes written are: %zu\n", bytes_written);
    printf("Error Code is: %s \n", strerror(errno));

    /*
    void *read_buf = malloc(length);
    ssize_t bytes_read = read(fd0, read_buf, length);
    printf("User Code: Bytes read are: %zu\n", bytes_read);
    printf("User Code: String is: %d\n", *(int *)read_buf);
    */
    close(fd0);
}