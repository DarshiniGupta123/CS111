#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
 #include <errno.h>

#define THREADS 't'
#define ITERATIONS 'i'
#define YIELD 'y'
#define SYNC 'a'

long long sum = 0;
int opt_yield = 0;
char lock = 'x';

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long spin_lock = 0;

long long old;
long long new;

void add(long long *pointer, long long value) {
  long long sum = *pointer + value;
  if(opt_yield)
    sched_yield();
  *pointer = sum;
}

void * thread_worker(void *iteration){
  unsigned long iter = *((unsigned long*) iteration);  
  for(unsigned long i = 0; i < iter; i++){
    switch(lock){
    case 'm':
      pthread_mutex_lock(&mutex);
      add(&sum, 1);
      pthread_mutex_unlock(&mutex);
      break;

    case 's':
      while(__sync_lock_test_and_set(&spin_lock, 1));
      add(&sum, 1);
      __sync_lock_release(&spin_lock);
      break;
      
    case 'c':
      ;
      long long prev, next;
      do {
	   prev = sum;
	   next = prev + 1;
      } while((__sync_val_compare_and_swap(&sum, prev, next)) != prev);

      break;
      
    default:
      add(&sum, 1);
      break;
    }
  }
  for(unsigned long i = 0; i < iter; i++){
    switch(lock){
    case 'm':
      pthread_mutex_lock(&mutex);
      add(&sum,- 1);
      pthread_mutex_unlock(&mutex);
      break;

    case 's':
      while(__sync_lock_test_and_set(&spin_lock, 1));
      add(&sum, -1);
      __sync_lock_release(&spin_lock);
      break;

    case 'c':
      ;
      long long prev, next;
      do {
           prev = sum;
           next = prev - 1;
      } while((__sync_val_compare_and_swap(&sum, prev, next)) != prev);

      break;

    default:
      add(&sum, -1);
      break;
    }

     //add(&sum, -1);
  }
  return NULL;
}

int main(int argc, char* argv[]){

  struct option long_options[] =
  {
   {"threads", 1, NULL, THREADS},
   {"iterations", 1, NULL, ITERATIONS},
   {"yield", 0, NULL, YIELD},
   {"sync", 1, NULL, SYNC},
   {0, 0, 0, 0}
  };
  
  int threads = 1;
  unsigned long iterations = 1;

  int ret = 0;

  while((ret = getopt_long(argc, argv, "", long_options, NULL)) != -1){

    switch(ret) {
    case THREADS:
      threads = atoi(optarg);
      break;

    case ITERATIONS:
      iterations = (unsigned long)(atoi(optarg));
      break;

    case YIELD:
      opt_yield = 1;
      break;

    case SYNC:
      if(strcmp(optarg, "s") == 0)
	lock = 's';
      else if(strcmp(optarg, "c") == 0)
	lock = 'c';
      else if(strcmp(optarg, "m") == 0)
	lock = 'm';
      else{
	fprintf(stderr, "Sync Error");
	exit(1);
      }
      break;
    default:
      fprintf(stderr, "Unrecognized argument");
      exit(1);
      break;
    }

  }

  char tag[15];

  strcat(tag, "add");

  if(opt_yield)
    strcat(tag, "-yield");
  if(lock != 'x'){
    char str[2];
    str[0] = lock;
    str[1] = '\0';
    strcat(tag, "-");
    strcat(tag, str);
  }
  if(lock == 'x')
    strcat(tag, "-none");     
    
  pthread_t thread[threads];
  
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  for(int i = 0; i < threads; i++){
    int ret = pthread_create(&thread[i], NULL, thread_worker, &iterations);
    if(ret == 1){
      perror("Error creating threads");
      exit(2);
    }
  }
  for(int i = 0; i < threads; i++){
    int ret = pthread_join(thread[i], NULL);
     if(ret == 1){
      perror("Error in joing threads");
      exit(2);
     }
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  long long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000;
  elapsed_time += end.tv_nsec;
  elapsed_time -= start.tv_nsec;

  int operations =  threads*iterations*2;

  printf("%s,%d,%ld,%d,%lld,%lld,%lld\n", tag, threads, iterations, operations, elapsed_time, elapsed_time/operations, sum);
  fflush(stdout);
  //exit(0);
}

