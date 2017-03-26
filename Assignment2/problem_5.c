// Problem 5
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<unistd.h>

struct spin_lock_t
{
    volatile int turn; // current thread being served
    volatile int counter; // the next thread number
};

void spin_lock (struct spin_lock_t *s);
void spin_unlock (struct spin_lock_t *s);
static inline int atomic_xadd(volatile int *ptr);

volatile int in_cs;
int num_threads;
pthread_t *threads;
volatile long* frequency_threads;
int is_time = 1;
struct spin_lock_t *lock;

/*
 * atomic_xadd
 *
 * equivalent to atomic execution of this code:
 *
 * return (*ptr)++;
 * 
 */
static inline int atomic_xadd(volatile int *ptr) {
  register int val __asm__("eax") = 1;
  asm volatile("lock xaddl %0,%1"
  : "+r" (val)
  : "m" (*ptr)
  : "memory"
  );  
  return val;
}

void spin_lock (struct spin_lock_t *s)
{
    int me = atomic_xadd(&(s->counter));
    while(me != s->turn);
    //thread_num[thread_number] = atomic_xadd(&(s->counter));
    
    /*int i;
    for(i = 0; i < num_threads; i++)
    {
    }*/
}

void spin_unlock (struct spin_lock_t *s)
{
    atomic_xadd(&(s->turn));
}

void thread_function(void *thread_number)
{
    while(is_time)
    {
        long thread_num = (long)thread_number;
        spin_lock(lock);
        assert(in_cs==0);
        in_cs++;
        assert(in_cs==1);
        in_cs++;
        assert(in_cs==2);
        in_cs++;
        assert(in_cs==3);
        in_cs=0;
        frequency_threads[thread_num] = frequency_threads[thread_num] + 1;
        spin_unlock(lock);
    }
}

int main(int argc, char**argv)
{
    int i;

    if(argc < 3) // Insufficient arguments
    {
        printf("Insufficient arguments.\n");
        return -1;
    }

    // argument 1 = number of threads
    num_threads = atoi(argv[1]);

    // argument 2 = seconds to run the program
    int num_seconds = atoi(argv[2]);

    lock = (struct spin_lock_t *)malloc(sizeof(struct spin_lock_t)*1);
    threads = (pthread_t *)malloc(sizeof(pthread_t)*num_threads);
    frequency_threads = (long *)malloc(sizeof(long)*num_threads);
    //thread_num = (long *)malloc(sizeof(long)*num_threads);

    for(i = 0; i < num_threads; i++)
    {
        *(frequency_threads + i) = 0;
    }

    /*
    for(i = 0; i< num_threads; i++)
    {
        *(thread_num + i) = 0;
    } 
    */

    if(threads == NULL)
    {
        printf("Error in initializing threads.\n");
        return -1;
    }
    if(frequency_threads == NULL)
    {
        printf("Error in initializing frequency of threads\n");
        return -1;
    }
    /*
    if(thread_num == NULL)
    {
        printf("Error in initializing thread number array.\n");
        return -1;
    }
    */

    void *status;

    
    for(i = 0; i < num_threads; i++)
    {
        int return_value = pthread_create(threads + i, NULL, thread_function, (void *)(long)i);
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
      printf("Thread %d has entered critical section %ld times\n" ,i + 1, frequency_threads[i]);
    }

    return 0;
}
