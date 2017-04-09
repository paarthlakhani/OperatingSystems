#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

// int open(const char *path, int oflags);
// ssize_t read(int fildes, void *buf, size_t nbytes);


int main(int argc, char* argv[])
{
    int fd;
    fd = open("/dev/sleepy0", O_RDWR);
    //fd = open("/dev/sleepy0", O_WRONLY);   
    //fd = open("/dev/sleepy0", O_RDONLY);

    if(fd < 0)
    {
        printf("Problem loading the file\n");
        printf("The error is: %d" , fd);
        exit(-1);
    }
    printf("File has been loaded: %d\n" , fd);
    // At this point file has been opened.

    // Write to the sleepy driver
    //const void *buf = (const void*)("First line written. Okay now, let's move on to the  next thing"); // String to write into the driver
    short *buf = (short *)malloc(sizeof(short));
    *buf = 5;
    // int buf = 2143;
    //long buf = 2143;
    //const void* buf = (const void*)sleep_time;
    //printf("Length of string is: %lu\n", strlen(buf));
    // unsigned long length = strlen(buf);
    size_t length = sizeof(*buf);
    //printf("Size of buffer is: %zu\n",length);
    ssize_t bytes_written = write(fd, buf, length);  
    printf("User Code: Bytes written are: %zu\n",bytes_written);
   //printf("User Code: Hello. Written to the file\n");

    // Read from the sleepy driver
    /* char read_buf[length];
    ssize_t bytes_read = read(fd, read_buf, length);
    printf("User Code: Bytes read are: %zu\n", bytes_read);
    printf("User Code: String is: %s\n", read_buf);*/
}