#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

// int open(const char *path, int oflags);
// ssize_t read(int fildes, void *buf, size_t nbytes);


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
    ssize_t bytes_written = write(fd1, buf, length);

    void *read_buf = malloc(length);
    ssize_t bytes_read = read(fd1, read_buf, length);
    printf("User Code: Bytes read are: %zu\n", bytes_read);
    printf("User Code: String is: %d\n", *(int *)read_buf);

    close(fd1);
}