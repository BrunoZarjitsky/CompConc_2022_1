# Computação Concorrente 2022.1
**Bruno Zarjitsky**<br>
**115117384**

***

## LAB 4

**Objetivo:** Implementar um metodo sequencial e um concorrente para encontrar os numeros primos de um vetor, calcular suas raizes, e armazenar em vetores de saida apenas as **raizes dos numeros primos**.

### 10^5

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 1.750892 | 1.736151 | 1.008490 
2 | 1.856958 | 0.964454 | 1.925398 
4 | 1.780045 | 0.516644 | 3.445396 
8 | 1.812759 | 0.449639 | 4.031586 

***

### 10^7

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 177.543795 | 177.762807 | 0.998768 
2 | 176.036875 | 96.749343 | 1.819515 
4 | 175.084112 | 57.335915 | 3.053655 
8 | 175.322447 | 50.181216 | 3.493786 

***

### 10^8

Para um vetor com 10^8 rodei apenas o concorrente para 8 nucleos, e custou 509.455990 segundos.<br>
Se o desempenho concorrente fosse numero_de_threads vezes melhor que o sequencial, o programa sequencial levaria pelo menos 67 minutos para chegar ao final da execução. Por isso não rodei para vetores deste tamanho.


***

## LAB 3

**Objetivo:** Implementar uma solução sequencial e uma concorrente para o problema de encontrar o *menor* e o *maior* valor de um vetor de números reais.


### 10^5

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.000433 | 0.000930 | 0.465365
2 | 0.001329 | 0.000522 | 2.547926
4 | 0.001528 | 0.001259 | 1.213483

***

### 10^7

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.036949 | 0.039273 | 0.940815
2 | 0.035819 | 0.018898 | 1.895408
4 | 0.039782 | 0.018458 | 2.155214

***

### 10^8

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.366762 | 0.360898 | 1.016249
2 | 0.373755 | 0.182388 | 2.049229
4 | 0.360600 | 0.130908 | 2.754598

