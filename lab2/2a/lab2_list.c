#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include <errno.h>

#include "SortedList.h"

#define THREADS 't'
#define ITERATIONS 'i'
#define YIELD 'y'
#define SYNC 'k'

#define LENGTH 5

//long long sum = 0;
int threads = 1;
int iterations = 1;

int opt_yield;
char yieldopts[20];
char lock = 'x';

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long spin_lock = 0;


SortedList_t *listhead;
SortedListElement_t *listArray;

SortedList_t* initializeHead()
{
  SortedList_t *listhead2 = malloc(sizeof(SortedList_t));
  listhead2->next = listhead2;
  listhead2->prev = listhead2;
  listhead2->key = NULL;

  return listhead2;
}

void init_elem(){
  srand(time(NULL));
  for(int i = 0; i < (threads*iterations); i++){
    char *key = malloc(sizeof(char) * (LENGTH));
    for(int j = 0; j < LENGTH-1; j++){
      key[j] = (char)(rand() % 26 + 'A');
    }
    key[LENGTH] = '\0';
    listArray[i].key = key;
  }
  
}

void thread_worker(void *threadNum){
  int iter = *((int*) threadNum);
  //int iter = tNum * iterations;
  //INSERT
  for(int i = iter; i < iterations + iter; i++){
    switch(lock){
    case 'm':
      pthread_mutex_lock(&mutex);
      SortedList_insert(listhead, &listArray[i]);
      pthread_mutex_unlock(&mutex);
      break;

    case 's':
      while(__sync_lock_test_and_set(&spin_lock, 1))
	continue;
      SortedList_insert(listhead, &listArray[i]);
      __sync_lock_release(&spin_lock);
      break;

    default:
      SortedList_insert(listhead, &listArray[i]);
      break;
    
    }
  }
  //LENGTH
  
  switch(lock){
   case 'm':
     pthread_mutex_lock(&mutex);
     if(SortedList_length(listhead) < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
     }
     pthread_mutex_unlock(&mutex);
     break;

   case 's':
     while(__sync_lock_test_and_set(&spin_lock, 1))
       continue;
     if(SortedList_length(listhead) < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
     }
     __sync_lock_release(&spin_lock);
     break;

  default:
    if(SortedList_length(listhead) < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
     }
     break;
   
  }

  for(int i = iter; i < iterations + iter; i++){
    int response;
    SortedListElement_t* e;
    switch(lock){
    case 'm':
      pthread_mutex_lock(&mutex);
      e = SortedList_lookup(listhead, listArray[i].key);
      if(e == NULL){
	fprintf(stderr, "Element not found\n");
	exit(2);
      }
      response = SortedList_delete(e);
      if(response != 0){
	fprintf(stderr, "Error with delete\n");
	exit(2);
      }
      pthread_mutex_unlock(&mutex);
      break;

    case 's':
      while(__sync_lock_test_and_set(&spin_lock, 1))
	continue;
      e = SortedList_lookup(listhead, listArray[i].key);
      if(e == NULL){
        fprintf(stderr, "Element not found\n");
        exit(2);
      }
      response = SortedList_delete(e);
      if(response != 0){
	 fprintf(stderr, "Element not found\n");
	 exit(2);
      }

      __sync_lock_release(&spin_lock);
      break;

    default:
      e = SortedList_lookup(listhead, listArray[i].key);
      if(e == NULL){
        fprintf(stderr, "Element not found\n");
        exit(2);
      }
      response = SortedList_delete(e);
      if(response != 0){
        fprintf(stderr, "Element not found\n");
        exit(2);
      }

      break;
    }
  }
  //return NULL;
}

int main(int argc, char* argv[]){
   struct option long_options[] =
  {
   {"threads", 1, NULL, THREADS},
   {"iterations", 1, NULL, ITERATIONS},
   {"yield", 1, NULL, YIELD},
   {"sync", 1, NULL, SYNC},
   {0, 0, 0, 0}
  };
  
  //int threads = 1;
  //unsigned long iterations = 1;

  int ret = 0;

  while((ret = getopt_long(argc, argv, "", long_options, NULL)) != -1){

    switch(ret) {
    case THREADS:
      threads = atoi(optarg);
      break;

    case ITERATIONS:
      iterations = (atoi(optarg));
      break;

    case YIELD:
      //opt_yield = 1;
      ;
      int len = strlen(optarg);
      for(int i = 0; i < len; i++){
	yieldopts[i] += optarg[i];
	if(optarg[i] == 'i')
	  opt_yield |= INSERT_YIELD;
	if(optarg[i] == 'd')
	  opt_yield |= DELETE_YIELD;
	if(optarg[i] == 'l')
	  opt_yield |= LOOKUP_YIELD;
      }
      break;

    case SYNC:
      if(strcmp(optarg, "s") == 0)
	lock = 's';
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

  char tag[24];

  strcat(tag, "list");
  
  if(opt_yield){
    strcat(tag, "-");
    strcat(tag, yieldopts);
  }
  else{
    strcat(tag, "-none");
  }
  if(lock != 'x'){
    char str[2];
    str[0] = lock;
    str[1] = '\0';
    strcat(tag, "-");
    strcat(tag, str);
  }
  if(lock == 'x')
    strcat(tag, "-none");

  listhead = initializeHead();

  listArray = malloc(threads * iterations * sizeof(SortedListElement_t));

  init_elem();
  
  pthread_t thread[threads];
  
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  int* val = malloc(threads*sizeof(int));
  for(int i = 0; i < threads; i++){
    val[i] = i*iterations;
    int ret2 = pthread_create(&thread[i], NULL, (void*)&thread_worker, &val[i]);
    if(ret2 == 1){
      perror("Error creating threads");
      exit(2);
    }
  }
  for(int i = 0; i < threads; i++){
    int ret2 = pthread_join(thread[i], NULL);
     if(ret2 == 1){
      perror("Error in joing threads");
      exit(2);
     }
  }
  clock_gettime(CLOCK_MONOTONIC, &end);

  long long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000;
  elapsed_time += end.tv_nsec;
  elapsed_time -= start.tv_nsec;

  int operations =  threads*iterations*3;

  printf("%s,%d,%d,1,%d,%lld,%lld\n", tag, threads, iterations, operations, elapsed_time, elapsed_time/operations);
  fflush(stdout);

  free(val);
  free(listArray);
  free(listhead);

  exit(0);

  
}

