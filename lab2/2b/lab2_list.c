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
#define LISTS 'l'

#define LENGTH 5

struct SubList{
  SortedList_t* sublist;
  pthread_mutex_t mutexlock;
  int spinlock;
};
    
int list_flag = 0;
int numLists = 1;
int threads = 1;
int iterations = 1;

int opt_yield;

char lock = 'x';

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int spin_lock = 0;


SortedList_t* listhead;
struct SubList* subListsHead;
SortedListElement_t *listArray;

int lockOperations = 0;
//long long *wait_time;
//struct timespec start_time, end_time;
long long totalLockTime = 0;

long long calc_diff(struct timespec start, struct timespec end){
 long long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000000;
 elapsed_time += end.tv_nsec;
 elapsed_time -= start.tv_nsec;
 totalLockTime += elapsed_time;
 lockOperations++;
 return elapsed_time;
 
 }

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

void initializeSubLists(){
  for(int i = 0; i < numLists; i++){
     subListsHead[i].sublist = initializeHead();
     pthread_mutex_init(&subListsHead[i].mutexlock, NULL);
     subListsHead[i].spinlock = 0;
  }
  
}

void thread_worker(void *threadNum){
  int iter = *((int*) threadNum);
  for(int i = iter; i < iterations + iter; i++){
    struct timespec start_time, end_time;
    switch(lock){
    case 'm':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      pthread_mutex_lock(&mutex);
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      SortedList_insert(listhead, &listArray[i]);
      pthread_mutex_unlock(&mutex);
      calc_diff(start_time, end_time);
      break;

    case 's':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      while(__sync_lock_test_and_set(&spin_lock, 1))
	continue;
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      SortedList_insert(listhead, &listArray[i]);
      __sync_lock_release(&spin_lock);
      calc_diff(start_time, end_time);
      break;

    default:
      SortedList_insert(listhead, &listArray[i]);
      break;
    
    }
  }

  struct timespec start_time2, end_time2;
  //LENGTH  
  switch(lock){
   case 'm':
     clock_gettime(CLOCK_MONOTONIC, &start_time2);
     pthread_mutex_lock(&mutex);
     clock_gettime(CLOCK_MONOTONIC, &end_time2);
     if(SortedList_length(listhead) < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
     }
     pthread_mutex_unlock(&mutex);
     calc_diff(start_time2, end_time2);
     break;

   case 's':
     clock_gettime(CLOCK_MONOTONIC, &start_time2);
     while(__sync_lock_test_and_set(&spin_lock, 1))
       continue;
     clock_gettime(CLOCK_MONOTONIC, &end_time2);
     if(SortedList_length(listhead) < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
     }
     __sync_lock_release(&spin_lock);
     calc_diff(start_time2, end_time2);
     break;

  default:
    if(SortedList_length(listhead) < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
     }
     break;
   
  }
  //LOOKUP AND DELETE
  for(int i = iter; i < iterations + iter; i++){
    struct timespec start_time, end_time;
    int response;
    SortedListElement_t* e;
    switch(lock){
    case 'm':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      pthread_mutex_lock(&mutex);
      clock_gettime(CLOCK_MONOTONIC, &end_time);
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
      calc_diff(start_time, end_time);
      break;

    case 's':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      while(__sync_lock_test_and_set(&spin_lock, 1))
	continue;
      clock_gettime(CLOCK_MONOTONIC, &end_time);
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
      calc_diff(start_time, end_time);
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
  
}

void thread_worker2(void *threadNum){
  
  int iter = *((int*) threadNum);
  for(int i = iter; i < iterations + iter; i++){
    int list_num = (listArray[i].key[0]) % numLists;
    struct timespec start_time, end_time;
    switch(lock){
    case 'm':
       clock_gettime(CLOCK_MONOTONIC, &start_time);
       pthread_mutex_lock(&subListsHead[list_num].mutexlock);
       clock_gettime(CLOCK_MONOTONIC, &end_time);
       SortedList_insert(subListsHead[list_num].sublist, &listArray[i]);
       calc_diff(start_time, end_time);
       pthread_mutex_unlock(&subListsHead[list_num].mutexlock);
       
      break;

    case 's':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      while(__sync_lock_test_and_set(&subListsHead[list_num].spinlock, 1))
        continue;
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      SortedList_insert(subListsHead[list_num].sublist, &listArray[i]);
      __sync_lock_release(&subListsHead[list_num].spinlock);
      calc_diff(start_time, end_time);
      break;

    default:
      SortedList_insert(subListsHead[list_num].sublist, &listArray[i]);
      break;

    }
    

  }

  //LENGTH
  for(int i = 0; i < numLists; i++){
    struct timespec start_time, end_time;
    switch(lock){
    case 'm':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      pthread_mutex_lock(&subListsHead[i].mutexlock);
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      calc_diff(start_time, end_time);
      break;
    case 's':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      while(__sync_lock_test_and_set(&subListsHead[i].spinlock, 1));
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      calc_diff(start_time, end_time);
      break;
    default:
      break;
    }
  }

  int length = 0;
  int lengthresult = 0;
  
  for(int i = 0; i < numLists; i++){
    lengthresult = SortedList_length(subListsHead[i].sublist);
    if(lengthresult < 0){
       fprintf(stderr, "List corrupted\n");
       exit(2);
    }
    else
      length += lengthresult;
  }

  for(int i = 0; i < numLists; i++){
      switch(lock){
    case 'm':
      pthread_mutex_unlock(&subListsHead[i].mutexlock);
      break;
    case 's':
      __sync_lock_release(&subListsHead[i].spinlock);
      break;

    default:
      break;
    }
  }
     

  
    
  
  for(int i = iter; i < iterations + iter; i++){
    struct timespec start_time, end_time;
    int list_num = (listArray[i].key)[0] % numLists;
    int response;
    SortedListElement_t* e;
    switch(lock){
    case 'm':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      pthread_mutex_lock(&subListsHead[list_num].mutexlock);
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      e = SortedList_lookup(subListsHead[list_num].sublist, listArray[i].key);
      if(e == NULL){
        fprintf(stderr, "Element not found\n");
        exit(2);
      }
      response = SortedList_delete(e);
      if(response != 0){
        fprintf(stderr, "Error with delete\n");
        exit(2);
      }
      calc_diff(start_time, end_time);
      pthread_mutex_unlock(&subListsHead[list_num].mutexlock);
      
      break;

    case 's':
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      while(__sync_lock_test_and_set(&subListsHead[list_num].spinlock, 1))
        continue;
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      e = SortedList_lookup(subListsHead[list_num].sublist, listArray[i].key);
      if(e == NULL){
        fprintf(stderr, "Element not found\n");
        exit(2);
      }
      response = SortedList_delete(e);
      if(response != 0){
         fprintf(stderr, "Element not found\n");
         exit(2);
      }
      calc_diff(start_time, end_time);
      __sync_lock_release(&subListsHead[list_num].spinlock);
      break;

    default:
      e = SortedList_lookup(subListsHead[list_num].sublist, listArray[i].key);
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
}

int main(int argc, char* argv[]){
  char yieldopts[20];

  struct option long_options[] =
  {
   {"threads", 1, NULL, THREADS},
   {"iterations", 1, NULL, ITERATIONS},
   {"yield", 1, NULL, YIELD},
   {"sync", 1, NULL, SYNC},
   {"lists", 1, NULL, LISTS},
   {0, 0, 0, 0}
  };
  
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

    case LISTS:
      list_flag = 1;
      numLists = atoi(optarg);
      subListsHead = malloc(numLists * sizeof(struct SubList));
      
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

  if(list_flag){
    initializeSubLists();
  }

  pthread_t thread[threads];
  
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);

  int ret2 = 0;
  int* val = malloc(threads*sizeof(int));
  for(int i = 0; i < threads; i++){
    val[i] = i*iterations;
    if(list_flag)
      ret2 = pthread_create(&thread[i], NULL, (void*)&thread_worker2, &val[i]);
    else
      ret2 = pthread_create(&thread[i], NULL, (void*)&thread_worker, &val[i]);
    if(ret2 == 1){
      perror("Error creating threads");
      exit(2);
    }
  }
  for(int i = 0; i < threads; i++){
    ret2 = pthread_join(thread[i], NULL);
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
  long long lockWait = 0;
  if(lock != 'x')
    lockWait = totalLockTime/lockOperations;
  
  printf("%s,%d,%d,%d,%d,%lld,%lld,%lld\n", tag, threads, iterations, numLists, operations, elapsed_time, elapsed_time/operations, lockWait);
  fflush(stdout);

  free(val);
  free(listArray);
  free(listhead);
  free(subListsHead);

  exit(0);

  
}

