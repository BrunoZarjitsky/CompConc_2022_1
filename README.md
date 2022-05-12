# Computação Concorrente 2022.1
**Bruno Zarjitsky**<br>
**115117384**

***

## LAB 4

**Objetivo:** Implementar um metodo sequencial e um concorrente para encontrar os numeros primos de um vetor, calcular suas raizes, e armazenar em vetores de saida apenas as **raizes dos numeros primos**.

### 10^5

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.554412 | 0.536575 | 1.033242
2 | 0.552029 | 0.272191 | 2.028094
4 | 0.543418 | 0.155139 | 3.502780

***

### 10^7

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 53.486894 | 51.987066 | 1.028850
2 | 52.476357 | 26.363255 | 1.990511
4 | 52.494540 | 15.686372 | 3.346506

***

### 10^8

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 535.481880 | 589.780860 | 0.907934
2 | 535.464544 | 329.257393 | 1.626279
4 | 523.239131 | 175.577120 | 2.980110


***

## LAB 3

**Objetivo:** Implementar uma solução sequencial e uma concorrente para o problema de encontrar o **menor** e o **maior** valor de um vetor de números reais.


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

