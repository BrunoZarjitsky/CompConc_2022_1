/* 
Bruno Zarjitsky
Computação concorrente lab 7
DRE: 115117384
Conta elementos pares em um vetor de inteiros
*/

import java.util.ArrayList;
import java.util.Random;

// Classe da estrutura de dados 
// Essa classe sera util para gerenciar o acesso
// das variaveis compartilhadas
class DataStructure {
    private int vector_index;
    private ArrayList<Integer> vector_array;

    public DataStructure(ArrayList<Integer> data) {
        vector_index = 0;
        vector_array = data;
    }

    // Funcao que retorna o valor de um elemento, e
    // incrementa a posicao do vetor
    public synchronized Integer get_value_and_increase_index() {
        // Necessario para evitar condicao de corrida 
        if (vector_index >= vector_array.size()) {
            return null;
        }
        return vector_array.get(vector_index++);
    }

    // public synchronized Integer obtemElemento(int vector_index) {
    //     // Necessario para evitar condicao de corrida 
    //     if (vector_index >= vector_array.size()) {
    //         return null;
    //     }
    //     return vector_array.get(vector_index);
    // }

    // Verifica se todos os valores do vetor foram percorridos
    public synchronized boolean process_ended() {
        return vector_index >= vector_array.size();
    }

}

// Classe que estende Thread e implementa a tarefa de cada thread do programa
class ThreadClass extends Thread {

    // Identificador da thread
    private int id_thread;

    // Variaveis compartilhados com outras threads
    private DataStructure s;
    private int even_count;

    public ThreadClass(int id_thread, DataStructure data) {
        this.id_thread = id_thread;
        this.data = data;
        this.even_count = 0;
    }

    // Metodo main da thread
    public void run() {
        while (!this.data.process_ended()) {
            Integer vector_element = this.data.get_value_and_increase_index();
            if (vector_element != null) { // Necessario para evitar condicao de corrida 
                this.even_count + this.is_even(vector_element); // Se par soma 1, se não par soma 0 na contagem de pares
            }
        }
    }

    // Funcao que avalia se o numero e par
    // Retorna 1 caso seja, e 0 caso nao seja
    public int is_even(int vector_element){
        if (vector_element % 2 == 0){
            return 1;
        }
        return 0;
    }

    // Retorna a contagem de numeros pares da thread
    public int get_even_count() {
        return this.even_count;
    }
}

// Classe da aplicacao
class Lab7 {
    static final int THREADS_AMOUNT = 2;
    static final int VECTOR_SIZE = 1000;

    public static void main (String[] args) {
        Thread[] threads = new Thread[THREADS_AMOUNT]; // Reserva espaco para o vetor de threads
        ArrayList<Integer> vector_array = new ArrayList<Integer>(); // Reserva espaco para o vetor de inteiros
        int even_count_sequential = 0; // Contador de pares sequencial
        int even_count_concurrent = 0; // Contador de pares concorrente

        // Inicializacao do vetor de inteiros
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            Random random_generator = new Random();
            int vector_element = random_generator.nextInt();
            if (vector_element % 2 == 0) { // Verifica se e par
                ++even_count_sequential;
            };
            vector_array.add(vector_element);
        }

        // Cria uma instancia de DataStructure para ser compartilhado pelas threads
        DataStructure data = new DataStructure(vector_array);

        // Cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
            threads[i] = new ThreadClass(i, data);
        }

        // Inicia as threads
        for (int i=0; i<threads.length; i++) {
            threads[i].start();
        }

        // Espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { 
                    threads[i].join(); 
                    even_count_concurrent += ((int)((ThreadClass) threads[0]).get_even_count())
                } 
            catch (InterruptedException e) {
                    return; 
                }
        }
        // Soma os totais de pares encontrados em cada thread
        even_count_concurrent = ((int) ((ThreadClass) threads[0]).get_even_count() + ((ThreadClass) threads[1]).get_even_count());

        // Imprime os resultados
        System.out.println("\nPares sequencial = " + even_count_sequential);
        System.out.println("\nPares concorrente = " + even_count_concurrent);
    }
}
