#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int isprime(unsigned long long num){
  unsigned long long r = 2;
  if(num == 1)
    return 0;
  if(num == 0)
   return 0;
  while(num % r && r < num){
    r++;
  }
  if(r == num){
    return 1;
  }
  else
    return 0;
}

int main(){
  /* Definir flags de protecao e visibilidade de memoria */
  int prot = PROT_READ | PROT_WRITE;
  int visi = MAP_PRIVATE | MAP_ANON;


  unsigned long long* entrada = mmap(NULL, 5000 * sizeof(unsigned long long), prot, visi ,0,0);
  unsigned int x = 0, i = 0;
  pid_t pid;
  pid_t teste;
  unsigned int* prim = mmap(NULL, sizeof(unsigned int), prot, MAP_SHARED | MAP_ANON ,0,0);

  scanf("%llu", &(entrada[x]));
  while(scanf("%llu", &(entrada[++x])) == 1){}

  for(i=x;i<5000;i++)
      entrada[i] = 0;

  for(i=0;i<=x;i++){ //o i tem que ser uma variável compartilhada
    pid = fork();
    if(!pid){ //apenas o filho executa esse if
      if(isprime(entrada[i]))
        ++*prim; //escreva um int que seja apontado por isso
      i++;
      exit(0);
    }
  }

  for(i=0;i<=x;i++)
    waitpid(-1, NULL, 0);

  printf("%d\n", *prim);
  munmap(entrada, 5000 * sizeof(int));
  return 0;
}
