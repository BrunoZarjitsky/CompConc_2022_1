#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

#define MEIO_VETOR 5000
#define TAM_VETOR 10000
#define N_THREADS 2

// altere o limite verificador caso queira ver outros valores
#define limite_verificador 10

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads, begin_vetor, end_vetor;
} t_Args;

typedef struct {
    int valor, quadrado;
} valor_e_quadrado;

valor_e_quadrado *vetor[TAM_VETOR];

void * tarefa (void *arg){
    t_Args *args = (t_Args *) arg;
    for (int i = args->begin_vetor; i < args->end_vetor; i++){
        vetor[i]->quadrado = vetor[i]->valor * vetor[i]->valor;
    }
    free(arg);
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid[N_THREADS];
    t_Args *arg;
    int testes_aprovados = 0, testes_regeitados = 0;

    // Preenche o vetor
    for (int i = 0; i < TAM_VETOR; i++){
        vetor[i] = malloc(sizeof(valor_e_quadrado));
        vetor[i]->valor = i;
    }

    // Cria as threads
    for (int i = 0; i < N_THREADS; i++){
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = i; 
        arg->nThreads = N_THREADS;
        arg->begin_vetor = i*MEIO_VETOR;
        arg->end_vetor = i*MEIO_VETOR+MEIO_VETOR;

        if (pthread_create(&tid[i], NULL, tarefa, (void *)arg)){
            printf("ERROR\n");
        }
    }

    // Verifica se as threads terminaram de executar
    for (int i = 0; i < N_THREADS; i++){
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        }
    }

    // Verificador dos valores
    // Altere o valor do limite_verificador para ver outros valores
    for (int i = 0; i < TAM_VETOR; i++){
        if (vetor[i]->quadrado == pow(vetor[i]->valor, 2)){
            testes_aprovados ++;
        }
        else{
            testes_regeitados ++;
        }
    }
    printf("Testes aprovados: %d\nTestes regeitados: %d\n", testes_aprovados, testes_regeitados);
    pthread_exit(NULL);

    return 0;
}