#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>


#define MAX 100

// creating semaphores
sem_t mutex,empty,full;

// no of threads
int p,c,pc,cc;

// creating stack buffer which takes in values from produces and places on top
// of buffer 
int stackbuffer[MAX];
int top_index = 0;


// producer function
void* producer(void* arg)
{
    int indexp = *(int*)arg;
    int product;
    //produce step
    for(int i=0;i<pc;i++)
    {
        product = rand()%500;

        //sleep(5);

        // wait for empty
        sem_wait(&empty);
        // wait for mutex
        sem_wait(&mutex);

        // add item to buffer
        stackbuffer[top_index] = product;
        top_index++;
        printf("Producer %d produced item %d\n",indexp,product);

        // signal mutex
        sem_post(&mutex);
        // signal full
        sem_post(&full);
    }
}

void* consumer(void* arg)
{
    int indexc = *(int*)arg;
    // consume step
    int product;
    for(int i=0;i<cc;i++)
    {
        // wait for full
        sem_wait(&full);
        // wait for mutex
        sem_wait(&mutex);

        // remove item from buffer
        top_index--;
        product = stackbuffer[top_index];
        printf("Consumer %d consumed item %d\n",indexc,product);

        // signal mutex
        sem_post(&mutex);
        // signal empty
        sem_post(&empty);
    }
}

int main(int argc,char* argv[])
{
    //taking in no of producers and consumers
    p = atoi(argv[2]);
    pc = atoi(argv[4]);
    c = atoi(argv[6]);
    cc = atoi(argv[8]);

    //srand(time(NULL));
    // initialize semaphores
    sem_init(&mutex,0,1);
    sem_init(&empty,0,MAX);
    sem_init(&full,0,0);
    
    // create producer threads
    pthread_t prod[p],cons[c];
    for(int i=1;i<=p;i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        pthread_create(&prod[i-1],NULL,&producer,a);
    }
    for(int i=1;i<=c;i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;        
        pthread_create(&cons[i-1],NULL,&consumer,a);
    }
    for(int i=0;i<p;i++)
    {
        pthread_join(prod[i],NULL);
    }
    for(int i=0;i<c;i++)
    {
        pthread_join(cons[i],NULL);
    }
    return 0;
}