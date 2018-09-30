#include <stdio.h>
#include <unistd.h>
#include <omp.h>

//-fopenmp
//C:\TDM-4\bin\libgomp-1.dll
//pthreadGC2.dll

#define TAMANHO 5
#define NUMITER 26

char buffer[TAMANHO];
int prox_entrada = 0;
int prox_saida = 0;
int contador = 0;
int vazio = 1;
int cheio = 0;
int i,j;

void coloca(char item)
{
    buffer[prox_entrada] = item;
    prox_entrada = (prox_entrada + 1) % TAMANHO;
    contador++;
    if (contador == TAMANHO)
        cheio = 1;
    if (contador == 1) // buffer estava vazio
        vazio = 0;
}

void produtor(int tid)
{
    char item;
    while( i < NUMITER)
    {
        {
            item = 'A' + (i % 26);
            coloca(item);
            #pragma omp barrier ///descomente se necessário
            i++;
            printf("%d Produzindo %c ...\n",tid, item);
        }
        usleep(20000);
    }
}

char retira()
{
    char item;

    item = buffer[prox_saida];
    prox_saida = (prox_saida + 1) % TAMANHO;
    contador--;
    if (contador == 0) // buffer está vazio
        vazio = 1;
    if (contador == (TAMANHO-1))
        cheio = 0;// buffer estava cheio
    return item;
}

void consumidor(int tid)
{
    char item;
    while(j < NUMITER)
    {
        {
            j++;
            #pragma omp barrier ///descomente se necessário
            item = retira();
            printf("%d ...Consumindo %c\n",tid, item);
        }
        usleep(20000);
    }
}

int main()
{
    int tid;
    i = j = 0;
    #pragma omp parallel firstprivate(i,j) private(tid) num_threads(4)
    {
       tid = omp_get_thread_num();
       if(tid % 2 == 0)
       {
           produtor(tid);
       }
       else
       {
           consumidor(tid);
       }
    }
    return 0;
}