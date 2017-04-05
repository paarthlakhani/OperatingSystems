#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

// int open(const char *path, int oflags);
// ssize_t read(int fildes, void *buf, size_t nbytes);


int main(int argc, char* argv[])
{
    int read_fd;
    read_fd = open("/dev/sleepy1", O_WRONLY);

    if(read_fd < 0)
    {
        printf("Problem loading the file\n");
        printf("The error is: %d" , read_fd);
        exit(-1);
    }
    printf("File has been loaded: %d" , read_fd);
}