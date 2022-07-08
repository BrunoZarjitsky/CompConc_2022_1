/* 
Bruno Zarjitsky
Computação concorrente lab 10
DRE: 115117384
Implementacao produtores/consumidores
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<stdint.h>
#include<semaphore.h>

sem_t exc_mut_escr, exc_mut_leit, escr, leit; //semaforos
int e=0, l=0; //globais

const int total_escritores = 5, total_leitores = 25;

void delay (){
    sleep(5);
}

void mini_delay(){
    sleep(1);
}

void * thread_leitora (void *arg){
    int id_da_thread = (intptr_t)arg;
    while(1) {
        printf("Leitora %d quer ler\n", id_da_thread);
        sem_wait(&leit);
        sem_wait(&exc_mut_leit);
        l++;
        if(l==1) {
            sem_wait(&escr);
        }
        sem_post(&exc_mut_leit);
        sem_post(&leit);
        printf("Leitura liberada para thread %d\n", id_da_thread);
        mini_delay();
        //le...
        printf("Leitora %d terminou de ler\n", id_da_thread);
        sem_wait(&exc_mut_leit);
        l--;
        if(l==0) {
            printf("Leitora %d libera os escritores\n\n", id_da_thread);
            sem_post(&escr);
        }
        sem_post(&exc_mut_leit);
        mini_delay();
    }
}

void * thread_escritora (void *arg){
    int id_da_thread = (intptr_t)arg;
    while(1) {
        printf("Escritora %d quer escrever\n", id_da_thread);
        sem_wait(&exc_mut_escr);
        e++;
        if(e==1) {
            sem_wait(&leit);
        }
        sem_post(&exc_mut_escr);
        sem_wait(&escr);
        printf("Escrita liberada para thread %d\n", id_da_thread);
        mini_delay();
        //escreve...
        printf("Escritora %d terminou de escrever\n", id_da_thread);
        sem_post(&escr);
        sem_wait(&exc_mut_escr);
        e--;
        if(e==0) {
            printf("Escritora %d libera as leitoras\n\n", id_da_thread);
            sem_post(&leit);
        }
        sem_post(&exc_mut_escr);
        delay();
    }   
}

int main (int argc, char * argv[]){
    pthread_t threads_escritoras[total_escritores];
    pthread_t threads_leitores[total_leitores];

    // Inicializacao de variavel
    sem_init(&exc_mut_escr, 0, 1);
    sem_init(&exc_mut_leit, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);

    // Inicializacao das threads
    for (int i = 0; i < total_leitores; i++){
        pthread_create(&threads_leitores[i], NULL, thread_leitora, (void *) (intptr_t) i);
    }
    for (int i = 0; i < total_escritores; i++){
        pthread_create(&threads_escritoras[i], NULL, thread_escritora, (void *) (intptr_t) i);
    }

    // Aguarda a finalizacao das threads
    for (int i = 0; i < total_escritores; i++){
        pthread_join(threads_escritoras[i], NULL);
    }
    for (int i = 0; i < total_leitores; i++){
        pthread_join(threads_leitores[i], NULL);
    }

    // Liberacao de memoria
    sem_close(&exc_mut_escr);
    sem_close(&exc_mut_leit);
    sem_close(&escr);
    sem_close(&leit);
}