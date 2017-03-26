// Problem 6
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<unistd.h>
#include<math.h>

int num_threads;
pthread_t *threads;
int is_time = 1;
pthread_mutex_t mutex_lock;
volatile unsigned long long total;
volatile unsigned long long inside;

void* calculate_radius()
{
  while(is_time)
    {
      pthread_mutex_lock(&mutex_lock);
      double x = (double)rand()/RAND_MAX;
      double y = (double)rand()/RAND_MAX;
      double area = x*x + y*y;
      if(area <= 1)
	{
	  inside = inside + 1;
	  total = total + 1;
	}
      else
	total = total + 1;
      pthread_mutex_unlock(&mutex_lock);
    }
  return 0;
}

int main(int argc, char**argv)
{
    int i;
    
    if(argc < 3) // Insufficient arguments
    {
        printf("Insufficient arguments.\n");
        return -1;
    }

    total = 0;
    inside = 0;

    //number of threads
    num_threads = atoi(argv[1]);

    //seconds to run threads
    int num_seconds = atoi(argv[2]);

    threads = (pthread_t *)malloc(sizeof(pthread_t)*num_threads);

    if(threads == NULL)
    {
        printf("Error in initializing threads.\n");
        return -1;
    }

    void *status;

    // initializing mutex lock
    pthread_mutex_init(&mutex_lock, NULL);

    for(i = 0; i < num_threads; i++)
      {
	int return_value = pthread_create(threads + i, NULL, calculate_radius,NULL);
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

    pthread_mutex_destroy(&mutex_lock);
    double pi = 4 * inside / (total * 1.0);    
    printf("Number of points inside the circle: %llu\n", inside);
    printf("Total number of points : %llu\n", total);
    printf("The value of pi is: %.6f\n", pi);

    return 0;
}
