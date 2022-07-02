/* 
Bruno Zarjitsky
Computação concorrente lab 5
DRE: 115117384
Cria 5 threads que devem seguir uma ordem de execução
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdint.h>
#include<semaphore.h>

#define numero_de_threads 5
// Variaveis globais
int contador_condicional = 0;    //variavel compartilhada entre as threads
sem_t cond_inicial, cond_intermediaria;     // semaforo para exclusao mutua entre as threads

void check_condicional(){
    if (contador_condicional == 3){
        sem_post(&cond_intermediaria);
    }
}

// Sempre a ultima thread a ser executada
void volte_sempre(){
    sem_wait(&cond_intermediaria);
    printf("Volte sempre!\n");
}

void fique_a_vontade(){
    sem_wait(&cond_inicial);
    printf("Fique a vontade.\n");
    contador_condicional++;
    check_condicional();
}

void sente_se_por_favor(){
    sem_wait(&cond_inicial);
    printf("Sente-se por favor.\n");
    contador_condicional++;
    check_condicional();
}

void copo_de_agua(){
    sem_wait(&cond_inicial);
    printf("Aceita um copo de agua?\n");
    contador_condicional++;
    check_condicional();
}

// Sempre a primeira thread a ser executada
void seja_bem_vindo(){
    printf("Seja bem vindo\n");
    for (int i = 0; i < 3; i++){
        sem_post(&cond_inicial);
    }
}

void * task_handler(void *arg){
    int id_da_thread = (intptr_t)arg;
    int boba1, boba2;
    boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;
    switch (id_da_thread){
        case 0:
            seja_bem_vindo();
            break;
        case 1:
            volte_sempre();
            break;
        case 2:
            fique_a_vontade();
            break;
        case 3:
            copo_de_agua();
            break;
        case 4:
            sente_se_por_favor();
            break;
        default:
            printf("ERROR--DEADLOCK\n");
            while(1){}
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    // Definicao de variavel
    pthread_t threads[numero_de_threads];

    // Inicializacao de variavel
    sem_init(&cond_inicial, 0, 0);
    sem_init(&cond_intermediaria, 0, 0);

    // Inicializacao das threads
    for (int i = 0; i < numero_de_threads; i++){
        pthread_create(&threads[i], NULL, task_handler, (void *) (intptr_t) i);
    }

    // Aguarda a finalizacao das threads
    for (int i = 0; i < numero_de_threads; i++){
        pthread_join(threads[i], NULL);
    }
    
    // Liberacao de memoria
    sem_close(&cond_inicial);
    sem_close(&cond_intermediaria);
}