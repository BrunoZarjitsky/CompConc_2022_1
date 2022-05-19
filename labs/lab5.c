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

#define numero_de_threads 5

int contador_de_threads = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_inicial, cond_intermediaria;

void espera_cond_inicial(){
    pthread_mutex_lock(&mutex);
    while (contador_de_threads < 1){
        pthread_cond_wait(&cond_inicial, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

// Sempre a ultima thread a ser executada
void volte_sempre(){
    pthread_mutex_lock(&mutex);
    if (contador_de_threads < 4){
        pthread_cond_wait(&cond_intermediaria, &mutex);
    }
    contador_de_threads++;
    pthread_mutex_unlock(&mutex);
    printf("Volte sempre!\n");
}

void fique_a_vontade(){
    espera_cond_inicial();
    printf("Fique a vontade.\n");
    pthread_mutex_lock(&mutex);
    contador_de_threads++;
    if (contador_de_threads == 4){
        pthread_cond_signal(&cond_intermediaria);
    }
    pthread_mutex_unlock(&mutex);
}

void sente_se_por_favor(){
    espera_cond_inicial();
    printf("Sente-se por favor.\n");
    pthread_mutex_lock(&mutex);
    contador_de_threads++;
    if (contador_de_threads == 4){
        pthread_cond_signal(&cond_intermediaria);
    }
    pthread_mutex_unlock(&mutex);
}

void copo_de_agua(){
    espera_cond_inicial();
    printf("Aceita um copo de agua?\n");
    pthread_mutex_lock(&mutex);
    contador_de_threads++;
    if (contador_de_threads == 4){
        pthread_cond_signal(&cond_intermediaria);
    }
    pthread_mutex_unlock(&mutex);
}

// Sempre a primeira thread a ser executada
void seja_bem_vindo(){
    pthread_mutex_lock(&mutex);
    if (contador_de_threads == 0){
        contador_de_threads++;
        pthread_cond_broadcast(&cond_inicial);
    }
    pthread_mutex_unlock(&mutex);
    printf("Seja bem vindo\n");
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

    // Inicializacao do mutex e variaves condicionais
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_inicial, NULL);
    pthread_cond_init(&cond_intermediaria, NULL);

    // Inicializacao das threads
    for (int i = 0; i < numero_de_threads; i++){
        pthread_create(&threads[i], NULL, task_handler, (void *) (intptr_t) i);
    }

    // Aguarda a finalizacao das threads
    for (int i = 0; i < numero_de_threads; i++){
        pthread_join(threads[i], NULL);
    }


    // Liberacao do mutex e variaveis condicionais
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_inicial);
    pthread_cond_destroy(&cond_intermediaria);

    return 0;
}