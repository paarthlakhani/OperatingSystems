#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include "crc32.h"

struct file_checksum
{
  char* file_name;
  uint32_t checksum;
};


  int comparator_func(const void *first_name, const void *second_name)
  {
   const char *first_file = ((const struct file_checksum *)first_name)->file_name;
   const char *second_file = ((const struct file_checksum *)second_name)->file_name;
    return(  strcmp( first_file, second_file ) );
  }
  
void print_files(struct file_checksum *files, int n)
{
  int i;
  for(i = 0; i < n ; i++)
  {
    printf("%s        " , files[i].file_name);
    printf("%08x\n" , files[i].checksum);
  }
}

void calculate_checksum(DIR *dir, DIR *counting_dir,char *directory_name)
{
  size_t num_files = 0;
  
  struct dirent* directory_copy;
  do
  {
    directory_copy = readdir(counting_dir);
    if(directory_copy!=NULL)
    {
      if(directory_copy->d_type!=DT_DIR)
      {
        num_files++;
      }
    }
  }while(directory_copy!=NULL);

  closedir(counting_dir);
  struct file_checksum *files;

  files = (struct file_checksum *)(malloc( sizeof(struct file_checksum)*num_files      ));
  if(files == NULL)
  {
    printf("Files has failed");
  }
  
  size_t i = 0;
  struct dirent* directory;
  
  char c = directory_name[strlen(directory_name)-1];
  //char *str2 = NULL;
  if(c!='/')
  {
    //char character = '/';
    //size_t length = strlen(directory_name);
    //str2 = malloc(length + 1 + 1);
    //strcpy(str2, directory_name);
    //directory_name[length] = character;
     strcat(directory_name, "/");
   // directory_name[length + 1] = '\0';
  }
  //printf("directory name: %s\n" , directory_name);

  do
  {
    directory  = readdir(dir);
    if(directory!=NULL) 
    {
      if(directory->d_type!=DT_DIR)
      {
        //printf("File name: %s       " ,directory->d_name );
        char *new_string = (char *)malloc(sizeof(char)*( strlen(directory->d_name) + strlen(directory_name) + 1 ));
        //char *new_string = (char *)malloc(sizeof(char)*( strlen(directory->d_name) + strlen(directory_name)  ));
        if(new_string == NULL)
        {
          printf("New String has failed");
        }     

        strcpy(new_string, directory_name);
        strcat(new_string, directory->d_name);
        //strcat(new_string, "\0");
        FILE *fp;
        //printf("Opening file %s\n" , new_string);
        if((fp = fopen(new_string, "rb")) != NULL)
        {
          fseek(fp, 0, SEEK_END);
          long size_file = ftell(fp);

          void *buffer = malloc(sizeof(char)*size_file);
          if(buffer == NULL)
          {
            printf("Buffer has failed");
          }

          //printf("currentsize is: %ld",size);
          //printf("\n");
          fseek(fp, -1*ftell(fp), SEEK_CUR);
          //printf("The file position is: %ld   ", ftell(fp));
          fread(buffer, 1, size_file, fp);

          uint32_t initialCheck = 0;
          uint32_t checksum_file = crc32(initialCheck, buffer, size_file);
          fclose(fp);
          free(buffer);

          //files[i].file_name = directory->d_name;
          files[i].file_name = (char *)malloc(strlen(directory->d_name)*sizeof(char));
          strcpy(files[i].file_name ,  directory->d_name);
          //files[i].file_name = directory->d_name;
          // printf("File added: %s", files[i].file_name );
          files[i].checksum = checksum_file;
        }
        else
        {
          printf("New path is: %s",new_string);
          printf("Name is: %s",directory->d_name);
          printf("ACCESS ERROR\n");
          exit(-1);
        }

        free(new_string);
        i++;
      }
    }
  }while(directory!=NULL);

  qsort(files, num_files, sizeof(struct file_checksum),comparator_func );
  printf("Number of files %zu", num_files);
  print_files(files,num_files);
  free(files);
  closedir(dir);
}

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    fprintf(stderr, "the directory name is not given");
    exit(-1);
  }

  char* directory_name = argv[1];
  DIR *dir = NULL;
  DIR *dir_copy = NULL;

  if((dir = opendir(directory_name)) == NULL)
  {
    fprintf(stderr,"Error, in opening the file");
    exit(-1);
  }

  if((dir_copy = opendir(directory_name)) == NULL)
  {
    fprintf(stderr,"Error, in opening the file again");
    exit(-1);
  }

  printf("FileName          Checksum\n");  
  calculate_checksum(dir, dir_copy,  directory_name);
  return 0;
}