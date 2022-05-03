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


float *vetor; // Vetor de entrada
int nthreads; // Numero de threads
// float maior; // Maior numero do vetor
// float menor; // Menor numero do vetor

void * tarefa(void * arg){
    
}



int main(int argc, char *argv[]){
    // Definecao de variaveis
    long int dim;
    int nthreads;
    float menor, maior;

    // Iniciacao de valores
    srand(time(0));   // Inicia a lib do rand()

    // Verificacao da entrada do programa
    if (argc<3){
        fprintf(stderr, "Digite %s <dimesao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    // Alocacao de memoria
    vetor = (float*) malloc(sizeof(float) * dim);
    if (vetor == NULL){
        fprintf(stderr, "ERRO--malloc\n");
    }

    // Inicializacao do vetor
    for (int i = 0; i < dim; i++){
        vetor[i] = ((rand() % 100) * 1.1)/(i + 1);
        if (i == 0){
            maior = vetor[i];
            menor = vetor[i];
        }
    }

    // Parte sequencial
    for (int i = 0; i < dim; i++){
        if (vetor[i] < menor){
            menor = vetor[i];
        }
        if (vetor[i] > maior){
            maior = vetor[i];
        }
    }

    // Parte concorrente

    // Exibicao de resultados
    printf("Menor numero do vetor: %f\n", menor);
    printf("Maior numero do vetor: %f\n", maior);

    // Liberacao de memoria

    return 0;
}