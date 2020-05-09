#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#include "SortedList.h"

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
  if(list == NULL || element == NULL){
    return;
  }

  SortedList_t *p = list;

  if(p->key != NULL){
    return;
  }

  SortedList_t *c = p->next;
  while(c != list){
    if(strcmp(c->key, element->key) < 0)
      break;
    p = c;
    c = c->next;
    c->prev = p;
  }

  if (opt_yield & INSERT_YIELD)
     sched_yield();
  
  // element->prev = p;
  element->next = p->next;
  element->prev = p;
  p->next = element;
  element->next->prev = element;

  return;
}

int SortedList_delete( SortedListElement_t *element){
  if(element == NULL || element->key == NULL)
    return 1;

  SortedListElement_t *n = element->next;
  SortedListElement_t *p = element->prev;

  if(p->next != element || n->prev != element)
    return 1;
  
  if (opt_yield & DELETE_YIELD)
    sched_yield();

  n->prev = p;
  p->next = n;
  //element->next = NULL;
  //element->prev = NULL;
  //element->prev->next = element->next;
  //element->next->prev = element->prev;
  return 0;

}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key){
  if(list == NULL)
    return NULL;

  SortedList_t *p = list;

  if(p->key != NULL)
    return NULL;

  SortedListElement_t *c = p->next;

  if (opt_yield & LOOKUP_YIELD)
    sched_yield();

  while(c != list){
    if(!strcmp(c->key, key))
      return c;
    /*if (opt_yield & LOOKUP_YIELD)
      sched_yield();*/
    c = c->next;
   }

  return NULL;
  
}

int SortedList_length(SortedList_t *list){
  //int counter = 0;

  if(list == NULL)
    return -1;

  if(list->key != NULL)
    return -1;


  int counter = 0;

  SortedListElement_t *c = list->next;
  // SortedListElement_t *n = c->next;
  //SortedListElement_t *p = c->prev;

  while(c != list){
    if(c->prev->next != c || c->next->prev != c)
      return -1;
    //counter++;
    if (opt_yield & LOOKUP_YIELD)
      sched_yield();
    c = c->next;
     //n = c->next;
     //p = c->prev;
    counter++;
  }

  return counter;
  
}
