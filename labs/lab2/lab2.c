/* 
Bruno Zarjitsky
Computação concorrente lab 2
DRE: 115117384
Multiplicacao de matriz-matriz (considerando matrizes quadradas) 
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"
#include<time.h>

int *mat1;   // Matriz de entrada
int *mat2;   // Matriz de entrada
int *saida; // Matriz de saida
int nthreads; // Numero de threads
int somaSeq = 0; // Soma dos elementos da matriz calculada sequencialmente
int somaCon = 0; // Soma dos elementos da matriz calculada concorrentemente

typedef struct{
    int id; // Identificador do elemento que a thread ira processar
    int dim; // Dimensao das estruturas de entrada
} tArgs;

// Percorre a matriz e imprime ela na ordem certa
void imprime_matriz(int mat[], int dim){
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            printf("%d ", mat[i * dim + j]);
        }
        printf("\n");
    }
}

// Função para fazer a multiplicacao de matrizes de forma concorrente
void * multiplica_matriz_concorrente(void *arg){
    tArgs *args = (tArgs*) arg;
    int somaprod;
    for (int i = args->id; i < args->dim; i += nthreads){
        for (int j = 0; j < args->dim; j++){
            somaprod = 0;
            for(int k = 0; k < args->dim; k++) {
                somaprod += mat1[i * (args->dim) + k] * mat2[k * (args->dim) + j];
            }
            saida[i * args->dim + j] = somaprod;
        }
    }
    pthread_exit(NULL);
}

// Função para fazer a multiplicacao de matrizes de forma sequencial
void multiplica_matriz_sequencial(int matA[], int matB[], int dim){
    int somaprod, somathread = 0;
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            somaprod = 0;
            for(int k = 0; k < dim; k++) {
                somaprod += matA[i * dim + k] * matB[k * dim + j];
            }
            somaSeq += somaprod;
            somathread += somaprod;
        }
    }
    somaCon += somathread;
}

int main(int argc, char* argv[]){
    // Definicao de variaveis
    int mostraMat1 = 0; // Mudar para 1 caso queira ver a matriz 1
    int mostraMat2 = 0; // Mudar para 1 caso queira ver a matriz 2
    int mostraSaida= 0; // Mudar para 1 caso queira ver a matriz de saida
    int dim; // Dimensao da matriz de entrada
    pthread_t *tid; // Identificadores das threads no sistema
    tArgs *args; // Identificadores locais das threads e dimensao
    double inicio, fim, tempoParteSeq, tempoMatrizCon, tempoMatrizSeq, desempenho; // Variaveis para controle de tempo

    // Inicializacao de valores (Aqui inicia tambem a parte sequencial da função main)
    srand(time(0));   // Inicia a lib do rand()

    // Inicia o contador de tempo inicial
    GET_TIME(inicio);

    // Leitura e avaliacao dos parametros de entrada
    if (argc<3) {
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim){
        nthreads = dim;
    }

    // Alocacao de memoria para as estruturas de dados
    mat1 = (int *) malloc(sizeof(int) * dim * dim);
    mat2 = (int *) malloc(sizeof(int) * dim * dim);
    saida = (int *) malloc(sizeof(int) * dim * dim);
    if (mat1 == NULL && mat2 == NULL && saida == NULL) {
        printf("ERRO--malloc\n");
        return 2;
    }

    // Inicializacao das estruturas de dados
    // Cada posicao das matrizez é iniciada com um valor aleatorio < 100
    for (int linha = 0; linha < dim; linha++){
        for (int coluna = 0; coluna < dim; coluna++){
            mat1[linha * dim + coluna] = rand() % 100; 
            mat2[coluna * dim + linha] = rand() % 100;
            saida[linha * dim + coluna] = 0;
        }
    }

    GET_TIME(fim);
    tempoParteSeq = fim-inicio;

    // Aqui comeca a o tempo de multiplicacao das matrizes concorrente
    GET_TIME(inicio);
    
    // Alocacao das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
    if (tid == NULL && args == NULL) {
        puts("ERRO--malloc\n");
        return 3;
    }

    // Criacao das threads
    for (int i = 0; i < nthreads; i++){
        (args + i)->id = i; // aloca i em args[i].id
        (args + i)->dim = dim; // aloca dim em args[i].dim
        // cria as threads chamando a funcao multiplica_matriz_concorrente
        if (pthread_create(tid+i, NULL, multiplica_matriz_concorrente, (void*) (args + i))){
            puts("ERRO--pthread_create\n");
            break;
        }
    }

    // Espera pelo termino das threads
    for (int i = 0; i < nthreads; i++){
        pthread_join(*(tid + i), NULL);
    }
    GET_TIME(fim);
    tempoMatrizCon = fim-inicio;

    // Aqui comeca a o tempo de multiplicacao das matrizes sequencialmente
    GET_TIME(inicio);

    multiplica_matriz_sequencial(mat1, mat2, dim);

    GET_TIME(fim);
    tempoMatrizSeq = fim-inicio;
    
    // Aqui volta a ser adicionado tempo a parte sequencial da main
    GET_TIME(inicio);

    GET_TIME(fim);
    tempoParteSeq += fim-inicio;

    // Calcula o desempenho
    desempenho = tempoMatrizSeq / tempoMatrizCon;

    // Mostra as informacoes na tela
    printf("\n");
    printf("Soma Seq = %d\n", somaSeq);
    printf("Soma Con = %d\n", somaCon);
    printf("Tempo da parte sequencial: ------ : %lf\n", tempoParteSeq);
    printf("Tempo da multiplicação sequencial : %lf\n", tempoMatrizSeq);
    printf("Tempo da multiplicação concorrente: %lf\n", tempoMatrizCon);
    printf("Desempenho ----------------------- : %lf\n", desempenho);

    if (mostraMat1 == 1){
        printf("\nMatriz 1\n");
        imprime_matriz(mat1, dim);
    }
    if (mostraMat2 == 1){
        printf("\nMatriz 2\n");
        imprime_matriz(mat2, dim);
    }
    if (mostraSaida == 1){
        printf("\nMatriz de saida\n");
        imprime_matriz(saida, dim);
    }
    if (somaCon == somaSeq){
        printf("\nMatrizes corretas!\n\n");
    }

    // Liberacao da memoria
    free(mat1);
    free(mat2);
    free(saida);
    free(args);
    free(tid);

    return 0;
}