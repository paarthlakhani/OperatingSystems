#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include "crc32.h"

// Moniter variables
// mutex lock
pthread_mutex_t moniter_lock;
volatile int file_no; // file selected by a thread
volatile int num_files; // total number of files
int is_running = 1; // threads are being run here.

// condition variables
pthread_cond_t main_condition;

struct file_checksum
{
  char* file_name;
  uint32_t checksum;
  int file_size;
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

void* runs_infinite(void *t)
{
  while(is_running)
  {
    pthread_mutex_lock(&moniter_lock);
   //  calculate_checksum(file_no, files, );
    file_no++; // no. of directories whose checksum has been calculated
    if(file_no >= num_files) // file_no has been greater than total number of files
      is_running = 0;
    pthread_mutex_unlock(&moniter_lock);
  }
  return 0;
}

void initialization(DIR *dir, DIR *counting_dir,char *directory_name, int threads)
{
 // size_t num_files = 0;
  pthread_t main_threads[threads];

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

  files = (struct file_checksum *)(malloc( sizeof(struct file_checksum)*num_files ));
  size_t i = 0;
  struct dirent* directory; 

  do
  {
    directory  = readdir(dir);
    if(directory!=NULL)
    {
      if(directory->d_type!=DT_DIR)
      {
          files[i].file_name = directory->d_name;
          files[i].checksum = 0;
          files[i].file_size = directory->d_reclen;
          i++;
      }
    }
  }while(directory!=NULL);

  // file_no -> current file_no
  int j;
  for(j = 0; j < threads; j++)
  {
    int return_value = pthread_create(&main_threads[j], NULL, runs_infinite, NULL);
    if(return_value!=0)
    {
      fprintf(stderr,"Thread was not created properly");
      exit(-1);
    }
  }

  qsort(files, num_files, sizeof(struct file_checksum),comparator_func );
  print_files(files,num_files);
  free(files);
  closedir(dir);
}

void calculate_checksum(int index, struct file_checksum *files, char *directory_name)
{
  struct file_checksum file = files[index];  
  char *new_string = (char *)malloc(sizeof(char)*( strlen(file.file_name) + strlen(directory_name) ));
  strcpy(new_string, directory_name);
  strcat(new_string, file.file_name);
  int size_file = file.file_size;
  void *buffer = malloc(sizeof(char)*size_file);

  if(buffer != NULL)
  {
    FILE *fp;
    if((fp = fopen(new_string, "r")) != NULL)
    {
      fread(buffer, 1, size_file, fp);
      uint32_t initialCheck = 0;
      files[index].checksum = crc32(initialCheck, buffer, size_file);;
      fclose(fp);
      free(buffer);
    }
    else
    {
      printf("ACCESS ERROR\n");
    }
  }
  else
  {
    printf("Could not allocate space");
  }

  free(new_string);
}

int main(int argc, char* argv[])
{
  if(argc < 3)
  {
    fprintf(stderr, "Not enough arguments");
    exit(-1);
  }

  char* directory_name = argv[1];
  int threads = atoi(argv[2]);
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

  // initializing mutex variables
  pthread_mutex_init(&moniter_lock, NULL);

  // initializating condition variables
  pthread_cond_init(&main_condition, NULL);

  file_no = 0;

  printf("FileName          Checksum\n");  
  initialization(dir, dir_copy, directory_name, threads);
  return 0;
}
