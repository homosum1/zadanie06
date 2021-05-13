#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include "my_lib.h"

const char FILE_PATH[] = "/Users/alexander/Desktop/zadanie06/numer.txt";
// const char FILE_PATH[] = "/home/kotarski_1175015/test/zadanie06/numer.txt";
sem_t* local_sem;

void CleaningFunc(void);
void signalHandler(void);
void CheckFileNum(char* val1, char* val2);

// ./semafory_b /home/kotarski_1175015/test/zadanie06/powielacz 3 3
// ./semafory_b /Users/alexander/Desktop/zadanie06/powielacz 3 3
// przekazywana: proces potomny, liczba procesów, liczba sekcji krytycznych każdegego procesu
int main(int argc, char* argv[]) {
  printf("przekazane argumenty: %d\n", argc);
  if(argc != 4) {
    printf("nieprawidłowe argumenty\n");
    return 1;
  }
  else
  {
    printf("ścieżka do procesu potomnego: %s\n", argv[1]);
    printf("liczba procesów: %s\n", argv[2]);
    printf("liczba sekcji krytycznych każdego procesu: %s\n\n", argv[3]);
  }

  // inicjalizacja semafory
  sem_t* sem = createMutex();
  local_sem = sem;
  initializeMutex(sem);
  char temp_add[256];
  sprintf(temp_add, "%lld", (long)sem);
  printf("Semapore value : %s\n", temp_add);

  // tworzenie nowego pliku
  FILE *out = fopen(FILE_PATH, "w");
  if(out == NULL) {
    printf("błąd przy otwieraniu pliku numer.txt\n");
    exit(0);
  }
  fprintf(out, "%s","0");
  fflush(out);
  fclose(out);

  // forkowanie
  pid_t pid_child;
  int loopCnt = atoi(argv[2]);
  printf("LoopCnt: %d\n", loopCnt);

  // obsługa sygnału (SIGINT id = 2)
  if (signal(2, signalHandler) == SIG_ERR){
    perror("Signal function error or prohibited signal\n");
    exit(EXIT_FAILURE);
  }

  for(int i=0; i<loopCnt; i++) {
    switch (pid_child = fork())
    {
        case -1:
            {
                perror("fork error");
                exit(EXIT_FAILURE);
            }
        case  0:
            {
                if(execl(argv[1], " " , argv[3], temp_add, &FILE_PATH, NULL) == -1) {
                  printf("konsument - execl function error ocurred\n");
                  exit(EXIT_FAILURE);
                }
            }
        default:
            {
                // . . .
            }
    }
  }

  sleep(10);
  CheckFileNum(argv[2], argv[3]);
  atexit(CleaningFunc);
  return 0;
}

void CleaningFunc(void) {
  printf("Sprzątanie pamięci\n");
  deleteMutex(local_sem);
}

void signalHandler(void) {
  printf("Przyjęto sygnał SIGINT\n");
  deleteMutex(local_sem);
  exit(EXIT_SUCCESS);
}

void CheckFileNum(char* val1, char* val2) {
  FILE *in = fopen(FILE_PATH, "r");
  char number[2];
  int it = 0;
  while(1) {
    int num = fgetc(in);
    if(feof(in))
      break;
    number[it] = num;
    it++;
  }
  int in_number = atoi(number), expected_val = atoi(val1)*atoi(val2);
  if(expected_val == in_number)
    printf("Końcowy numer w pliku tekstowym jest poprawny i wynosi: %d\n", in_number);
  else
    printf("Końcowy numer w pliku tekstowym NIE jest poprawny i wynosi: %d, a powinien  wynosić: %d\n", in_number, expected_val);
}
