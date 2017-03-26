#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

static const int THREADS = 1;

#define NPROCS 10
#define REPS (100 * 1000)

static int pipe_fds[NPROCS][2];
static pthread_t thrds[NPROCS];

static void *pass_the_token(void *arg) {
  intptr_t n = (intptr_t)arg;
  for (int count = 0; count < REPS; ++count) {
    char c;
    int res = read(pipe_fds[n][0], &c, 1);
    assert(res == 1);
    assert(c == 7);
    res = write(pipe_fds[(n + 1) % NPROCS][1], &c, 1);
    assert(res == 1);
  }
  if (THREADS) {
    return 0;
  } else {
    exit(0);
  }
}

static double elapsed(struct timeval a, struct timeval b) {
  double ad = (1000000.0 * a.tv_sec) + a.tv_usec;
  double bd = (1000000.0 * b.tv_sec) + b.tv_usec;
  return bd - ad;
}

int main(void) {
  for (int i = 0; i < NPROCS; ++i) {
    int res = pipe(pipe_fds[i]);
    assert(res == 0);
  }
  if (THREADS) {
    for (intptr_t i = 0; i < NPROCS; ++i) {
      int res = pthread_create(&thrds[i], NULL, pass_the_token, (void *)i);
      assert(res == 0);
    }
  } else {
    for (intptr_t i = 0; i < NPROCS; ++i) {
      int pid = fork();
      assert(pid != -1);
      if (pid == 0) {
        // never returns
        pass_the_token((void *)i);
      }
    }
  }
  struct timeval start;
  int res = gettimeofday(&start, NULL);
  assert(res == 0);
  char c = 7;
  res = write(pipe_fds[0][1], &c, 1);
  assert(res == 1);
  if (THREADS) {
    for (int i = 0; i < NPROCS; ++i) {
      res = pthread_join(thrds[i], NULL);
      assert(res == 0);
    }
  } else {
    for (int i = 0; i < NPROCS; ++i) {
      res = wait(NULL);
      assert(res != -1);
    }
  }
  struct timeval stop;
  res = gettimeofday(&stop, NULL);
  assert(res == 0);
  double usecs = elapsed(start, stop);
  printf("elapsed time = %lf microseconds\n", usecs);
  printf("context switch takes %lf microseconds\n", usecs / (NPROCS * REPS));
  return 0;
}
