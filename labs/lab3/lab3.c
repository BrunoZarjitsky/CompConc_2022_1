/* 
Bruno Zarjitsky
Computação concorrente lab 3
DRE: 115117384
Acha o menor e o maior valor de um vetor
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"
#include<time.h>

// Variaveis globais
float *vetor;

typedef struct {
    int id;
    int dimensao_do_vetor;
    int tamanho_passo;
    float maior_valor;
    float menor_valor;
} MaiorMenorValor;

// Funcao achaMaiorMenorValor
/* 
Recebendo um objeto do tipo MaiorMenorValor acha o menor e o maior valor de um pedaco do vetor 
*/
void achaMaiorMenorValor(MaiorMenorValor *arg){
    for (int i = arg->id; i < arg->dimensao_do_vetor; i += arg->tamanho_passo){
        if (i == arg->id){
            arg->maior_valor = vetor[i];
            arg->menor_valor = vetor[i];
        }
        if (vetor[i] < arg->menor_valor){
            arg->menor_valor = vetor[i];
        }
        if (vetor[i] > arg->maior_valor){
            arg->maior_valor = vetor[i];
        }
    }
}


void * funcaoConcorrente(void * arg){
    MaiorMenorValor *maior_e_menor = (MaiorMenorValor*) arg;
    achaMaiorMenorValor(maior_e_menor);
    pthread_exit(NULL);
}

// Funcao para imprimir o vetor
void imprime_vetor(long long int dimensao_do_vetor){
    int dimensao_maxima = 20;
    printf("\nVetor de entrada: \n");
    // Evita de ficar mostrando muitos valores
    if (dimensao_do_vetor > dimensao_maxima){
        dimensao_do_vetor = dimensao_maxima;
    }
    for (int i = 0; i < dimensao_do_vetor; i++){
        printf("%.2f ", vetor[i]);
    }
    printf("\n\n");
}

int main(int argc, char *argv[]){

    // Definicao de variaveis
    int mostra_vetor = 0;
    int numero_de_threads;
    long long int dimensao_do_vetor;
    double inicio;
    double fim;
    double tempo_concorrente;
    double tempo_sequencial;
    double desempenho;
    pthread_t *id_thread;
    MaiorMenorValor maior_menor_sequencial;
    MaiorMenorValor maior_menor_concorrente;
    MaiorMenorValor *maior_menor_thread;

    // Inicia a lib do rand()
    srand(time(0));

    // Verificacao da entrada do programa
    if (argc<3){
        fprintf(stderr, "Digite %s <dimesao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dimensao_do_vetor = atoi(argv[1]);
    numero_de_threads = atoi(argv[2]);

    // Alocacao de memoria
    vetor = (float*) malloc(sizeof(float) * dimensao_do_vetor);
    id_thread = (pthread_t*) malloc(sizeof(pthread_t) * numero_de_threads);
    maior_menor_thread = (MaiorMenorValor*) malloc(sizeof(MaiorMenorValor) * numero_de_threads);
    if (vetor == NULL || id_thread == NULL || maior_menor_thread == NULL){
        fprintf(stderr, "ERRO--malloc\n");
    }

    // Inicializacao do vetor
    for (int i = 0; i < dimensao_do_vetor; i++){
        vetor[i] = rand() * 1.1;
    }

    // Mostra o vetor se necessario
    if (mostra_vetor == 1){
        imprime_vetor(dimensao_do_vetor);
    }

    // Parte sequencial
    GET_TIME(inicio);
    maior_menor_sequencial.dimensao_do_vetor = dimensao_do_vetor;
    maior_menor_sequencial.id = 0;
    maior_menor_sequencial.tamanho_passo = 1;

    achaMaiorMenorValor(&maior_menor_sequencial);
    GET_TIME(fim);
    tempo_sequencial = fim-inicio;

    // Parte concorrente
    GET_TIME(inicio);

    for (int i = 0; i < numero_de_threads; i++){
        (maior_menor_thread + i)->id = i;
        (maior_menor_thread + i)->dimensao_do_vetor = dimensao_do_vetor;
        (maior_menor_thread + i)->tamanho_passo = numero_de_threads;
        // cria as threads chamando a funcaoConcorrente
        if (pthread_create(id_thread+i, NULL, funcaoConcorrente, (void*) (maior_menor_thread + i))){
            puts("ERRO--pthread_create\n");
            break;
        }
    }

    // Define um valor inicial para as posicoes maior_valor e menor_valor do objeto maior_menor_concorrente
    maior_menor_concorrente.maior_valor = vetor[0];
    maior_menor_concorrente.menor_valor = vetor[0];

    // Espera as threads terminarem de executar e compara os valores
    for (int i = 0; i < numero_de_threads; i++){
        pthread_join(*(id_thread + i), NULL);
        if ((maior_menor_thread + i)->maior_valor > maior_menor_concorrente.maior_valor){
            maior_menor_concorrente.maior_valor = (maior_menor_thread + i)->maior_valor;
        }
        if ((maior_menor_thread + i)->menor_valor < maior_menor_concorrente.menor_valor){
            maior_menor_concorrente.menor_valor = (maior_menor_thread + i)->menor_valor;
        }
    }

    GET_TIME(fim);
    tempo_concorrente = fim-inicio;

    // Exibicao de resultados
    printf("Menor numero do vetor (sequencial) : %.2f\n", maior_menor_sequencial.menor_valor);
    printf("Menor numero do vetor (concorrente): %.2f\n", maior_menor_concorrente.menor_valor);
    printf("Maior numero do vetor (sequencial) : %.2f\n", maior_menor_sequencial.maior_valor);
    printf("Maior numero do vetor (concorrente): %.2f\n", maior_menor_concorrente.maior_valor);

    // Calcula o desempenho e exibe os tempos
    desempenho = tempo_sequencial / tempo_concorrente;
    printf("\n%lf / %lf\n%lf\n", tempo_sequencial, tempo_concorrente, desempenho);

    // Liberacao de memoria
    free(vetor);
    free(maior_menor_thread);
    free(id_thread);

    return 0;
}