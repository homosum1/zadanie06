#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef lib_pt_1
#define lib_pt_1

sem_t* createMutex() {
  sem_t* mutex;
  mutex = (sem_t*) malloc(sizeof(sem_t));
  if(mutex == NULL) {
    printf("Semaphore couldn't be allocated\n");
    exit(0);
  } else
    return mutex;
}

int initializeMutex(sem_t* mutex) {
  int inicialized = sem_init(mutex, 0, 1);
  if(inicialized == -1) {
    printf("Semaphore inicialization error\n");
  }
  return inicialized;
}

int postMutex(sem_t* mutex) {
  int isGood = sem_post(mutex);
  if(isGood == -1) {
    printf("Semaphore postMutex error\n");
    exit(0);
  }

  return isGood;
}

int waitMutex(sem_t* mutex) {
  int isGood = sem_wait(mutex);
  if(isGood == -1) {
    printf("Semaphore waitMutex error\n");
    exit(0);
  }

  return isGood;
}

int getMutexValue(sem_t* mutex) {
  int value;
  int error = sem_getvalue(mutex, &value);
  if(error == -1) {
    printf("Getvalue function error\n");
    exit(0);
  }
  return value;
}


int deleteMutex(sem_t* mutex) {
  int deleted = sem_destroy(mutex);
  if(deleted == -1) {
    printf("Semaphore delete error occured\n");
  }
  free(mutex);
  return deleted;
}


#endif
