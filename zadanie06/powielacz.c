#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "my_lib.h"

int ReadFile(char* file_path);
void WriteFile(char* file_path, int numb);

int main(int argc, char* argv[])
{
  if(argc != 4) {
    printf("nieprawidłowe argumenty\n");
    return 1;
  } else {
    printf("Oto program powielacza\n");
    printf("ilość sekcji krytycznych: %s\n", argv[1]);
    printf("semaphore pointer: %s\n", argv[2]);
    printf("open.txt file path: %s\n", argv[3]);
  }

  printf("semaphore: %ld\n\n", atol(argv[2]));
  sem_t* sem = (sem_t*)atol(argv[2]);


  for(int i=0; i<atoi(argv[1]); i++) {
    sem_wait(sem);

    int number = ReadFile(argv[3]);
    int randomNum = rand() % 4;
    sleep(randomNum);
    WriteFile(argv[3], ++number);
    printf("number: %d\n", number);
    sem_post(sem);
  }

  return 0;
}


int ReadFile(char* file_path) {
  FILE *in = fopen(file_path, "r");
  if(in == NULL) {
    printf("błąd przy otwieraniu pliku numer.txt\n");
    exit(0);
  }
  char number[2];
  int it = 0;
  while(1) {
    int num = fgetc(in);
    if(feof(in))
      break;
    number[it] = num;
    it++;
  }
  int in_number = atoi(number);
  fflush(in);
  fclose(in);
  return in_number;
}

void WriteFile(char* file_path, int num) {
  FILE *out = fopen(file_path, "w");
  if(out == NULL) {
    printf("błąd przy otwieraniu pliku numer.txt\n");
    exit(0);
  }
  fprintf(out, "%d",num);
  fflush(out);
  fclose(out);
}
