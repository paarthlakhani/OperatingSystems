#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<unistd.h>

volatile int in_cs;
volatile int* number;
volatile int* choosing;

// Number of times each thread enters the critical section
volatile int* frequency_threads;
volatile pthread_t* threads;
volatile int is_time = 1;
pthread_mutex_t mutex_lock;

// Finds the maximum in the array and then increments by 1 to get the 
// new index
int new_index(volatile int* number,int num_threads) 
{
  int max = 0;
  int i;
  for(i = 0 ; i < num_threads; i++)
  {
      if( *(number + i) > max)
	    {
	      max = *(number + i);
	    }
  }
  return (max + 1);
}

void lock()
{
  
}

void unlock()
{
  
}

// worker function
void thread_function(void *thread_number)
{
  while(is_time)
  {
    pthread_mutex_lock(&mutex_lock);
    int thread_num = (int)thread_number;
    // printf("Thread number is : %d\n" , thread_num );
    assert(in_cs==0);
    in_cs++;
    assert(in_cs==1);
    in_cs++;
    assert(in_cs==2);
    in_cs++;
    assert(in_cs==3);
    in_cs=0;
    //*(frequency_threads + thread_num) = *(frequency_threads + thread_num) + 1;
    //printf("The thread number is: %d\n", thread_num);
    frequency_threads[thread_num] = frequency_threads[thread_num] + 1;
    pthread_mutex_unlock(&mutex_lock);
  }
}

int main(int argc, char**argv)
{
  if(argc < 3) // Sufficient threads are not provided
    {
      printf("Insufficient arguments.\n");
      return -1;
    }

  // argument 1 = number of threads
  int num_threads = atoi(argv[1]);

  // argument 2 = seconds to run the program
  int num_seconds = atoi(argv[2]);
  
  // initializing two arrays of size num_threads
  number = (int *)malloc(sizeof(int)*num_threads);
  choosing = (int *)malloc(sizeof(int)*num_threads);
  threads = (pthread_t *)malloc(sizeof(pthread_t)*num_threads);
  frequency_threads = (int *)malloc(sizeof(int)*num_threads);

  int i;
  for(i = 0 ; i < num_threads ; i++)
  {
    frequency_threads[i] = 0;
  }

  if(number == NULL)
    {
      printf("Error in initializing the number array.\n");
      return -1;
    }
  if(choosing == NULL)
    {
      printf("Error in initializing the choosing array.\n");
      return -1;
    }
  if(threads == NULL)
    {
      printf("Error in initializing the threads.\n");
      return -1;
    }
  if(frequency_threads == NULL)
    {
      printf("Error in initializing the threads.\n");
      return -1;
    }

  void *status;

  // initializing mutex locks
 pthread_mutex_init(&mutex_lock, NULL);

  // Creating threads
  for( i = 0 ; i < num_threads; i++)
    {
      // getting an error
      int return_value = pthread_create(threads + i,NULL,thread_function,(void *)i);
      if(return_value != 0)
      {
        printf("Error in creating threads");
        return -1;
      }
    }

    sleep(num_seconds);
    is_time = 0;

    for( i = 0 ; i < num_threads; i++)
    {
      pthread_join(*(threads + i), &status);
    }

    for( i = 0 ; i < num_threads; i++)
    {
      printf("The frequency is: %d\n" , frequency_threads[i]);
    }

  return 0;
}


  /*
  int  i;
  int j = 20;
  for( i = 0 ; i < num_threads; i++)
    {
      *(number + i) = rand() % j;
      j = j + (rand() % 67);
    }
  printf("Hello Everyone\n");

  for( i = 0 ; i < num_threads; i++)
    {
      printf("%d\n",*(number + i));
    }

  printf("The maximum is: %d",new_index(number, num_threads) );
  */