#include<stdio.h>
#include<pthread.h>

#define NTHREADS 10

int vetor[NTHREADS];

//Definir a tarefa que a thread vai fazer
void * tarefa (void * arg) {
	int ident = *(int *)arg;
	vetor[ident-1] = ident;
	printf("Ola, sou a thread %d!\n", ident);
	pthread_exit(NULL);
}

// Funcao principal
int main(void) {
	pthread_t tid[NTHREADS]; // identificador da thread no sistema
	int ident[NTHREADS]; // identificador local da thread

	// cria as threads
	for (int i = 1; i <= NTHREADS; i++){
		ident[i-1] = i;
		if (pthread_create(&tid[i-1], NULL, tarefa, (void *)&ident[i-1])){
			printf("ERRO -- pthread_create\n");
		};
	};

	// Espera as threads terminarem de executar
	for (int i = 0; i < NTHREADS; i++){
		// cria as threads
		if (pthread_join(tid[i], NULL)){
			printf("ERRO -- pthread_join\n");
		};
	};

	// Imprimir o vetor de identificadores
	for (int i = 0; i < NTHREADS; i++){
		printf("%d ", vetor[i]);
	};
	printf("\n");

	printf("Ola, sou a thread principal!\n");
	// Desvincula o termino da main do termino da thread
	pthread_exit(NULL);
	return 0;
}
