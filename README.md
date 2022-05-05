# Computação Concorrente 2022.1
**Bruno Zarjitsky**<br>
**115117384**

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

