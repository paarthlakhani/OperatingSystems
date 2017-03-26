#include<stdio.h>
#include<stdlib.h>

int n_cats = 0;
int n_dogs = 0;
int n_birds = 0;

int main(int argc, char *argv[])
{

  if(argc !=  4)
    {
      fprintf(stderr,"Three Arguments not given");
      exit(0);
    }

  // We have to be able detect the errors when converting the string into an integer
  n_cats = atoi(argv[1]);
  n_dogs = atoi(argv[2]);
  n_birds = atoi(argv[3]);
  

  if(n_cats < 0 || n_cats > 99 || n_dogs < 0 || n_dogs > 99 || n_birds < 0 || n_birds > 99)
    {
      fprintf(stderr,"Number of animals not in the correct range");
    }
  else
    {
      // DO the monitor and other stuff here
    }
}

void play(void) {
  /*for (int i=0; i<10; i++) 
    {
    assert(cats >= 0 && cats <= n_cats);
    assert(dogs >= 0 && dogs <= n_dogs);
    assert(birds >= 0 && birds <= n_birds);
    assert(cats == 0 || dogs == 0);
    assert(cats == 0 || birds == 0);
    }*/
}
