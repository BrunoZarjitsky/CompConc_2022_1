// Bruno Zarjitsky
// Monitor que implementa a logica do padrao leitores/escritores
class LE {
    private int leit, escr, control_var;  
    
    // Construtor
    LE() { 
       this.leit = 0; //leitores lendo (0 ou mais)
       this.escr = 0; //escritor escrevendo (0 ou 1)
       this.control_var = 0;
    } 
    
    // Entrada para leitores
    public synchronized void EntraLeitor (int id, int thread_type) {
        try { 
            while (this.escr > 0) {
            //if (this.escr > 0) {
                System.out.println ("self.le.leitorBloqueado("+id+", " + thread_type + ", " + this.control_var + ")");
                wait();  //bloqueia pela condicao logica da aplicacao 
            }
            this.leit++;  //registra que ha mais um leitor lendo
            System.out.println ("self.le.leitorLendo("+id+", " + thread_type + ", " + this.control_var + ")");
        } catch (InterruptedException e) { }
    }
    
    // Saida para leitores
    public synchronized void SaiLeitor (int id, int thread_type) {
        this.leit--; //registra que um leitor saiu
        if (this.leit == 0) 
            this.notify(); //libera escritor (caso exista escritor bloqueado)
        System.out.println ("self.le.leitorSaindo("+id+", " + thread_type + ", " + this.control_var + ")");
    }
    
    // Entrada para escritores
    public synchronized void EntraEscritor (int id, int thread_type) {
        try { 
            while ((this.leit > 0) || (this.escr > 0)) {
                //if ((this.leit > 0) || (this.escr > 0)) {
                System.out.println ("self.le.escritorBloqueado("+id+", " + thread_type + ", " + this.control_var + ")");
                wait();  //bloqueia pela condicao logica da aplicacao 
            }
            this.escr++; //registra que ha um escritor escrevendo
            System.out.println ("self.le.escritorEscrevendo("+id+", " + thread_type + ", " + this.control_var + ")");
        } catch (InterruptedException e) { }
    }
    
    // Saida para escritores
    public synchronized void SaiEscritor (int id, int thread_type) {
       this.escr--; //registra que o escritor saiu
       notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
       System.out.println ("self.le.escritorSaindo("+id+", " + thread_type + ", " + this.control_var + ")");
    }

    // Funcao que incrementa o valor de control_var
    public void increase_control_var(){
        this.control_var++;
    }

    public Integer get_control_var(){
        return this.control_var;
    }

    public boolean is_even(){
        if (this.control_var % 2 == 0){
            return true;
        }
        return false;
    }

    // Funcao que retorna o valor de um elemento, e
    // incrementa a posicao do vetor
    public void set_control_var(int thread_id){
        this.control_var = thread_id;
    }
}


// -------------------------------------------------------
// Thread 1
class Thread1 extends Thread {
    private int id;
    private int delay;
    private LE monitor;

    // Construtor
    Thread1 (int id, int delayTime, LE monitor){
        this.id = id;
        this.delay = delayTime;
        this.monitor = monitor;
    }

    // Metodo executado pela thread
    public void run (){
        // Declaracao de variavel
        // Try
        try {
            for (;;) {
                this.monitor.EntraEscritor(this.id, 1);
                this.monitor.increase_control_var();
                this.monitor.SaiEscritor(this.id, 1);
                sleep(delay);
            }
        } catch (InterruptedException e) {return;}
    }
}

// -------------------------------------------------------
// Thread 2
class Thread2 extends Thread {
    private int id;
    private int delay;
    private LE monitor;


    // Construtor
    Thread2 (int id, int delayTime, LE monitor){
        this.id = id;
        this.delay = delayTime;
        this.monitor = monitor;
    }

    // Metodo executado pela thread
    public void run (){
        // Try
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id, 2);
                if (this.monitor.is_even()){
                    System.out.println("---PAR---");
                }
                else {
                    System.out.println("---IMPAR---");
                }
                this.monitor.SaiLeitor(this.id, 2);
                sleep(this.delay);
            }
        } catch (InterruptedException e) {return;}
    }

    // Funcao que avalia se o numero e par
    public boolean is_even(int value){
        if (value % 2 == 0){
            return true;
        }
        return false;
    }
}

// -------------------------------------------------------
// Thread 3
class Thread3 extends Thread {
    private int id;
    private int delay;
    private LE monitor;


    // Construtor
    Thread3 (int id, int delayTime, LE monitor){
        this.id = id;
        this.delay = delayTime;
        this.monitor = monitor;
    }

    // Metodo executado pela thread
    public void run (){
        // Declaracao de variavel
        double j=777777777.7, i;
        int control_var;
        // Try
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id, 3);
                control_var = this.monitor.get_control_var();
                System.out.println(control_var);
                this.monitor.SaiLeitor(this.id, 3);
                for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de leitura
                this.monitor.EntraEscritor(this.id, 3);
                this.monitor.set_control_var(this.id);
                this.monitor.SaiEscritor(this.id, 3);
                sleep(this.delay); 
            }
        } catch (InterruptedException e) {return;}
    }
}
  

//--------------------------------------------------------
// Classe principal
class Lab8 {
    static final int T1_AMOUNT = 10, T2_AMOUNT = 20, T3_AMOUNT = 30;
    static final int TOTAL_THREADS_AMOUNT = T1_AMOUNT + T2_AMOUNT + T3_AMOUNT;
    
    public static void main (String [] args){
        Thread[] threads = new Thread[TOTAL_THREADS_AMOUNT];
        int i = 0;
        LE monitor = new LE();

        for (i = 0; i < T1_AMOUNT; i++){
            threads[i] = new Thread1(i+1, (i+1)*500, monitor);
            threads[i].start();
            // System.out.println("Thread1 com id: " + i + "iniciada com sucesso!");
        }

        for (i = T1_AMOUNT; i < T1_AMOUNT + T2_AMOUNT; i++){
            threads[i] = new Thread2(i+1, (i+1)*500, monitor);
            threads[i].start();
            // System.out.println("Thread2 com id: " + i + "iniciada com sucesso!");
        }

        for (i = T1_AMOUNT + T2_AMOUNT; i < T1_AMOUNT + T2_AMOUNT + T3_AMOUNT; i++){
            threads[i] = new Thread3(i+1, (i+1)*500, monitor);
            threads[i].start();
            // System.out.println("Thread3 com id: " + i + "iniciada com sucesso!");
        }
    }
}