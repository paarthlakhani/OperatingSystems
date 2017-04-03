#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>
#include<unistd.h>

// command line arguments
int n_cats = 0;
int n_dogs = 0;
int n_birds = 0;

// in-order to run the threads again and again
int cats_running = 1;
int dogs_running = 1;
int birds_running = 1;

// current cats, dogs and birds in the critical section
int cats = 0;
int dogs = 0;
int birds = 0;

// condition variables
pthread_cond_t cats_cond;
pthread_cond_t dogs_cond;
pthread_cond_t birds_cond;

// mutex lock
pthread_mutex_t moniter_lock;

// Frequency of cats, dogs and birds
int cat_freq = 0;
int dog_freq = 0;
int bird_freq = 0;

// Seconds the main thread should sleep
int num_seconds = 10;

void play(void);
void cat_enter(void); // Changed this declaration
void cat_exit(void);
void dog_enter(void);
void dog_exit(void);
void bird_enter(void);
void bird_exit(void);
void* cat_func(void *);
void* dog_func(void *);
void* bird_func(void *);

void* cat_func(void *t)
{

  while(cats_running)
    {
      cat_enter();
      cat_exit();
    }
  return 0;
}

void* dog_func(void *t)
{
  while(dogs_running)
    {
      dog_enter();
      dog_exit();
    }
  return 0;
}

void* bird_func(void *t)
{
  while(birds_running)
    {
      bird_enter();
      bird_exit();
    }
  return 0;
}

void cat_enter(void)
{
    pthread_mutex_lock(&moniter_lock);
    while(dogs!=0 || birds!=0) 
    {
      pthread_cond_wait(&cats_cond,&moniter_lock);
    }

    cat_freq++;
    cats++;
    play();
}

void cat_exit(void)
{
  pthread_mutex_lock(&moniter_lock);
  cats--; 
  pthread_cond_broadcast(&dogs_cond);
  pthread_cond_broadcast(&birds_cond);
  pthread_mutex_unlock(&moniter_lock);
}

void dog_enter(void)
{
    pthread_mutex_lock(&moniter_lock);
    //while(cats!=0 && birds!=0)
    while(cats!=0)
    { 
      pthread_cond_wait(&dogs_cond, &moniter_lock);
    }
    dog_freq++;
    dogs++;
    play();
}

void dog_exit(void) 
{
  pthread_mutex_lock(&moniter_lock);
  dogs--;
 pthread_cond_broadcast(&cats_cond);
  pthread_mutex_unlock(&moniter_lock);
}


void bird_enter(void)
{
    pthread_mutex_lock(&moniter_lock);
    while(cats!=0)
    {
      pthread_cond_wait(&birds_cond, &moniter_lock);
    }

    bird_freq++;
    birds++;
    play();


    //  pthread_mutex_unlock(&moniter_lock);
}

void bird_exit(void)
{
  pthread_mutex_lock(&moniter_lock);
  birds--;
  pthread_cond_broadcast(&cats_cond);
  pthread_mutex_unlock(&moniter_lock);
}

void play(void) 
{

  pthread_mutex_unlock(&moniter_lock);

  for (int i=0; i<100; i++) 
  {
        assert(cats >= 0 && cats <= n_cats);
        assert(dogs >= 0 && dogs <= n_dogs);
        assert(birds >= 0 && birds <= n_birds);
        assert(cats == 0 || dogs == 0);
        assert(cats == 0 || birds == 0);
  }
}

int main(int argc, char *argv[])
{
  if(argc !=  4)
    {
      fprintf(stderr,"Three Arguments not given");
      exit(-1);
    }

  // We have to be able detect the errors when converting the string into an integer
  n_cats = atoi(argv[1]);
  n_dogs = atoi(argv[2]);
  n_birds = atoi(argv[3]);

  if(n_cats < 0 || n_cats > 99 || n_dogs < 0 || n_dogs > 99 || n_birds < 0 || n_birds > 99)
    {
      fprintf(stderr,"Number of animals not in the correct range");
      exit(-1);
    }
  else
    {
      pthread_t cat_threads[n_cats];
      pthread_t dog_threads[n_dogs];
      pthread_t bird_threads[n_birds];
      // initializing the mutex lock
      pthread_mutex_init(&moniter_lock, NULL);

      // initializing the condition variables
      pthread_cond_init(&cats_cond, NULL);
      pthread_cond_init(&dogs_cond, NULL);
      pthread_cond_init(&birds_cond,NULL);

      void *status;      
      
      // create the cat threads
      int i;
      for(i = 0; i < n_cats ; i++)
      {
        int return_value = pthread_create(&cat_threads[i], NULL, cat_func, NULL);
        if(return_value!=0)
        {
          fprintf(stderr,"Thread was not created properly");
          exit(-1);
        }
      }

      // creating the dog threads
      for(i = 0; i < n_dogs ; i++)
      {
        int return_value = pthread_create(&dog_threads[i], NULL, dog_func, NULL);
        if(return_value!=0)
        {
          fprintf(stderr,"Thread was not created properly");
          exit(-1);
        }
      }

      // creating the bird threads
      
      for(i = 0; i < n_birds ; i++)
      {
        int return_value = pthread_create(&bird_threads[i], NULL, bird_func, NULL);
        if(return_value!=0)
        {
          fprintf(stderr,"Thread was not created properly");
          exit(-1);
        }
      }

      // Sleep main thread;
      sleep(num_seconds);

      cats_running = 0;
      dogs_running = 0;
      birds_running = 0;

      for( i = 0 ; i < n_cats; i++)
      {
        pthread_join(cat_threads[i], &status);
      }

      for( i = 0 ; i < n_dogs; i++)
      {
        pthread_join(dog_threads[i], &status);
      }

      
      for( i = 0 ; i < n_birds; i++)
      {
        pthread_join(bird_threads[i], &status);
      }

      printf("cat play = %d, dog play = %d, bird play = %d\n", cat_freq, dog_freq, bird_freq);

      pthread_cond_destroy(&cats_cond);
      pthread_cond_destroy(&dogs_cond);
      pthread_cond_destroy(&birds_cond);
      pthread_mutex_destroy(&moniter_lock);
      pthread_exit(NULL);
    }
}
