/* 
Bruno Zarjitsky
Computação concorrente lab 4
DRE: 115117384
Calcular a raiz quadrada de todos elementos primos de um vetor
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"
#include<time.h>
#include<math.h>

int *vetor_entrada;
float *vetor_saida_sequencial;
float *vetor_saida_concorrente;
int *total_primos_thread;
unsigned long long int soma_primos_sequencial = 0;
unsigned long long int soma_primos_concorrente = 0;


int posicao_vetor_saida_concorrente = 0; //variavel compartilhada entre as threads
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

typedef struct {
    int inicio_do_vetor;
    int dimensao_do_vetor;
    int tamanho_do_passo;
} CaracteristicasVetor;

// Funcao para imprimir o vetor
void imprime_vetor_entrada(long long int dimensao_do_vetor){
    int dimensao_maxima = 20;
    printf("\nVetor de entrada: \n");
    // Evita de ficar mostrando muitos valores
    if (dimensao_do_vetor > dimensao_maxima){
        dimensao_do_vetor = dimensao_maxima;
    }
    for (int i = 0; i < dimensao_do_vetor; i++){
        printf("%d ", vetor_entrada[i]);
    }
    printf("\n\n");
}

int ehPrimo (long long int numero){
    if (numero <= 1){
        return 0;
    }
    if (numero == 2){
        return 1;
    }
    if (numero%2 == 0){
        return 0;
    }
    for (int i = 3; i < sqrt(numero)+1; i += 2){
        if (numero%i == 0){
            return 0;
        }
    }
    return 1;
}

void raizDosPrimos(int vetor_de_entrada[], float vetor_de_saida[], CaracteristicasVetor *arg){
    int posicao_vetor_saida = 0;
    for (int i = arg->inicio_do_vetor; i < arg->dimensao_do_vetor; i += arg->tamanho_do_passo) {
        if (ehPrimo(vetor_de_entrada[i])){
            vetor_de_saida[posicao_vetor_saida] = sqrt(vetor_de_entrada[i]);
            posicao_vetor_saida++;
            soma_primos_sequencial += vetor_de_entrada[i];
        }
    }
}

int contaPrimos(int vetor_de_entrada[], CaracteristicasVetor *arg){
    int total_de_primos = 0;

    for (int i = arg->inicio_do_vetor; i < arg->dimensao_do_vetor; i += arg->tamanho_do_passo){
        if (ehPrimo(vetor_de_entrada[i])){
            total_de_primos++;
        }
    }
    return total_de_primos;
}

void * contaPrimosConcorrente (void * arg){
    CaracteristicasVetor *car_vet_thread = (CaracteristicasVetor*) arg;
    total_primos_thread[car_vet_thread->inicio_do_vetor] = contaPrimos(vetor_entrada, car_vet_thread);
    pthread_exit(NULL);
}

void * raizDosPrimosConcorrente (void * arg){
    CaracteristicasVetor *car_vet_thread = (CaracteristicasVetor*) arg;
    for (int i = car_vet_thread->inicio_do_vetor; i < car_vet_thread->dimensao_do_vetor; i += car_vet_thread->tamanho_do_passo){
        if (ehPrimo(vetor_entrada[i])){
            pthread_mutex_lock(&mutex);
            vetor_saida_concorrente[posicao_vetor_saida_concorrente] = sqrt(vetor_entrada[i]);
            posicao_vetor_saida_concorrente++;
            soma_primos_sequencial += vetor_entrada[i];
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){

    // Declaracao de variaveis
    long long int dimensao_do_vetor;
    int numero_de_threads;
    int mostra_vetor = 0;
    int total_de_primos_sequencial = 0;
    int total_de_primos_concorrente = 0;
    double inicio;
    double fim;
    double tempo_concorrente;
    double tempo_sequencial;
    double desempenho;
    CaracteristicasVetor car_vet_sequencial;
    CaracteristicasVetor *car_vet_thread;
    pthread_t *id_thread;

    // Inicia a lib do rand()
    srand(time(0));
    // Inicia o mutex
    pthread_mutex_init(&mutex, NULL);

    // Leitura dos parametros de entrada
    if (argc<3){
        fprintf(stderr, "Digite %s <dimesao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dimensao_do_vetor = atoi(argv[1]);
    numero_de_threads = atoi(argv[2]);

    // Aloca memoria
    id_thread = (pthread_t*) malloc(sizeof(pthread_t) * numero_de_threads);
    vetor_entrada = (int*) malloc(sizeof(int) * dimensao_do_vetor);
    car_vet_thread = (CaracteristicasVetor*) malloc(sizeof(CaracteristicasVetor) * numero_de_threads);
    total_primos_thread = (int*) malloc(sizeof(int) * numero_de_threads);

    // Inicializacao do vetor
    for (int i = 0; i < dimensao_do_vetor; i++){
        vetor_entrada[i] = rand();
    }

    // Mostra o vetor se necessario

    if (mostra_vetor == 1){
        imprime_vetor_entrada(dimensao_do_vetor);
    }

    // Parte sequencial do programa
    printf("Iniciando parte sequencial do programa.\n");
    GET_TIME(inicio);
    car_vet_sequencial.inicio_do_vetor = 0;
    car_vet_sequencial.dimensao_do_vetor = dimensao_do_vetor;
    car_vet_sequencial.tamanho_do_passo = 1;
    total_de_primos_sequencial = contaPrimos(vetor_entrada, &car_vet_sequencial);

    vetor_saida_sequencial = (float*) malloc(sizeof(float) * total_de_primos_sequencial);
    raizDosPrimos(vetor_entrada, vetor_saida_sequencial, &car_vet_sequencial);

    GET_TIME(fim);
    tempo_sequencial = fim-inicio;

    // Parte concorrente do programa
    printf("Iniciando parte concorrente do programa.\n");
    GET_TIME(inicio);
    for (int i = 0; i < numero_de_threads; i++){
        (car_vet_thread + i)->inicio_do_vetor = i;
        (car_vet_thread + i)->dimensao_do_vetor = dimensao_do_vetor;
        (car_vet_thread + i)->tamanho_do_passo = numero_de_threads;
        total_primos_thread[i] = 0;
        if (pthread_create(id_thread + i, NULL, contaPrimosConcorrente, (void *) (car_vet_thread + i))){
            fprintf(stderr, "ERRO--pthread_create\n");
            break;
        }
    }
    total_de_primos_concorrente = 0;
    for (int i = 0; i < numero_de_threads; i++){
        pthread_join(*(id_thread + i), NULL);
        total_de_primos_concorrente += total_primos_thread[i];
    }
    vetor_saida_concorrente = (float*) malloc(sizeof(float) * total_de_primos_concorrente);
    for (int i = 0; i < numero_de_threads; i++){
        (car_vet_thread + i)->inicio_do_vetor = i;
        (car_vet_thread + i)->dimensao_do_vetor = dimensao_do_vetor;
        (car_vet_thread + i)->tamanho_do_passo = numero_de_threads;
        if (pthread_create(id_thread + i, NULL, raizDosPrimosConcorrente, (void *) (car_vet_thread + i))){
            fprintf(stderr, "ERRO--pthread_create\n");
            break;
        }
    }
    for (int i = 0; i < numero_de_threads; i++){
        pthread_join(*(id_thread + i), NULL);
    }

    GET_TIME(fim);
    tempo_concorrente = fim-inicio;

    desempenho = tempo_sequencial/tempo_concorrente;

    // Exibe resultados
    if (total_de_primos_concorrente == total_de_primos_sequencial || soma_primos_concorrente == soma_primos_sequencial){
        printf("Validado !\n");
    }
    else{
        printf("Invalidado !\n");
    }
    printf("Total de primos: %d\n", total_de_primos_concorrente);
    
    printf("\n");
    printf("Tempo Sequencial : %f\n", tempo_sequencial);
    printf("Tempo Concorrente: %f\n", tempo_concorrente);

    // Libera memoria
    pthread_mutex_destroy(&mutex);
    free(vetor_saida_sequencial);
    free(vetor_saida_concorrente);
    free(vetor_entrada);
    free(id_thread);
    free(car_vet_thread);
    free(total_primos_thread);

    return 0;
}