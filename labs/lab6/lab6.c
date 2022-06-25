/* 
Bruno Zarjitsky
Computação concorrente lab 6
DRE: 115117384
Implementacao do problema dos leitores e escritores, dando prioridade para escritores
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define L 5 //numero de threads leitoras
#define E 5 //numero de threads escritoras

//variaveis do problema
int leit=0; //contador de threads lendo
int escr=0; //contador de threads escrevendo
int quer_escrever = 0; // Contador de threads na espera pra escrever

// Variaveis usadas apenas para melhor visualizacao da fila de espera
int quer_ler = 0; // Contador de threads na espera pra ler
int id_operando = 0; // Id da thread que esta operando
char * tipo_operacao; // Tipo da operacao da thread ativa

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;


// Funcao criada para melhor visualizacao da fila de espera
void imprime_locks(int id_bloqueado, char * E_ou_L){
	printf("\n ________________________________________________ \n");
    printf("| -%s[%d] bloqueou --------------------------------|\n", E_ou_L, id_bloqueado);
    printf("| -aguardando %s[%d] terminar ---------------------|\n", tipo_operacao, id_operando);
	printf("| -Lendo | Quer_ler | Escrevendo | Quer_Escrever |\n");
    printf("| -  %d   |    %d     |     %d      |       %d       |\n", leit, quer_ler, escr, quer_escrever);
    printf("|________________________________________________|\n\n");
}

//entrada leitura
void InicLeit (int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);
    while(escr > 0 || quer_escrever > 0) {
        quer_ler ++;
        imprime_locks(id, "L");
        pthread_cond_wait(&cond_leit, &mutex);
        quer_ler --;
        printf("L[%d] desbloqueou\n", id);
    }
    leit++;
    id_operando = id;
    tipo_operacao = "L";
    pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", id);
    leit--;
    if(leit==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    while((leit>0) || (escr>0)) {
        quer_escrever ++;
        imprime_locks(id, "E");
        pthread_cond_wait(&cond_escr, &mutex);
        quer_escrever --;
        printf("E[%d] desbloqueou\n", id);
    }
    escr++;
    id_operando = id;
    tipo_operacao = "E";
    pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);
    escr--;
    if (quer_escrever > 0){
        pthread_cond_signal(&cond_escr);
    }
    else{
        pthread_cond_broadcast(&cond_leit);
    }
    pthread_mutex_unlock(&mutex);
}

//thread leitora
void * leitor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        InicLeit(*id);
        printf("Leitora %d esta lendo\n", *id);
        FimLeit(*id);
        sleep(1);
        int boba1, boba2;
        boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;
    } 
    free(arg);
    pthread_exit(NULL);
}

//thread leitora
void * escritor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        InicEscr(*id);
        printf("Escritora %d esta escrevendo\n", *id);
        FimEscr(*id);
        sleep(1);
        int boba1, boba2;
        boba1=1000; boba2=-1000; while (boba2 < boba1) boba2++;
    } 
    free(arg);
    pthread_exit(NULL);
}

//funcao principal
int main(void) {
    //identificadores das threads
    pthread_t tid[L+E];
    int id[L+E];

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    //cria as threads escritoras
    for(int i=0; i<E; i++) {
        id[i+L] = i+1;
        if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
    } 
    

    //cria as threads leitoras
    for(int i=0; i<L; i++) {
        id[i] = i+1;
        if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
    } 

    pthread_exit(NULL);
    return 0;
}
