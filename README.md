# Computação Concorrente 2022.1
**Bruno Zarjitsky**<br>
**115117384**

***

## LAB 8

Implementação de uma aplicação concorrente em Java usando o padrão básico **leitores/escritores**
<br>
A implementação é dividida em 3 threads sendo 

### T1
Incrementa a variavel de controle
### T2 
Le a variavel de controle e imprime se ela é par ou impar
### T3
Le a variavel e imprime ela na tela, faz um processamento, e define o valor da variavel de controle como o id da thread

### Observações
1 - Para verificar a corretude da aplicação, está sendo usado o codigo auxiliar verificaLE.py<br>
Esse codigo le o arquivo textScript.txt e verifica todas suas chamadas<br>
Estou disponibilizando tambem um arquivo textScript.txt de exemplo com 14620 instruções para verificação.<br>
2 - Para gerar um novo arquivo textScript.txt rode o comando <br>
java lab8 > textScript.txt <br>
3 - Acabei tendo pouco tempo essa semana para fazer o codigo e por isso ele está um pouco bagunçado, de qualquer forma estou disponivel caso seja necessario alguma explicação mais detalhada.<br>
***

## LAB 5

### Atividade 1

Alterando o numero de threads A para 1, a execução nunca vai terminar pois vai entrar em deadlock<br>

### Atividade 2

Sim, sempre foi atendida, pois o pthread_cond_broadcast vai liberar todas as threads independente de quantas estiverem bloqueadas, então para todos os casos possiveis
(0 bloqueios, 1 bloqueio, 2 bloqueio) será realizado o desbloqueio da execução.<br>

### Atividade 3

Sim, o log de execução foi sempre correto<br>
Não podemos substiuir a linha 50 pela 51, pois entre o recebimento do sinal, e o print do valor x, esse x já pode ter sido incrementado novamente, e não ser multiplo de 10<br>
Invertendo essas linhas, o log de execução não esteve sempre correto, aconteceu da thread B imprimir um valor não multiplo de 10, no caso 23.<br>

### Atividade 4

Alguns exemplos de execução:<br><br>
Seja bem vindo<br>
Aceita um copo de agua?<br>
Fique a vontade.<br>
Sente-se por favor.<br>
Volte sempre!<br>
***
Seja bem vindo<br>
Fique a vontade.<br>
Aceita um copo de agua?<br>
Sente-se por favor.<br>
Volte sempre!<br>
***
Seja bem vindo<br>
Fique a vontade.<br>
Sente-se por favor.<br>
Aceita um copo de agua?<br>
Volte sempre!<br>

***

## LAB 4

**Objetivo:** Implementar um metodo sequencial e um concorrente para encontrar os numeros primos de um vetor, calcular suas raizes.

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

