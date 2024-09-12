#include <pthread.h>    // Biblioteca para criar e gerenciar threads
#include <semaphore.h>  // Biblioteca para usar semáforos (controle de sincronização)
#include <stdio.h>      // Biblioteca padrão para entrada e saída (printf)
#include <unistd.h>     // Biblioteca para funções como sleep (espera)


// Declara dois semáforos que serão usados para sincronizar os semáforos de trânsito
sem_t semaforo1;  // Semáforo para controlar o primeiro sinal de trânsito
sem_t semaforo2;  // Semáforo para controlar o segundo sinal de trânsito

// Função que será executada pelas threads, responsável por simular o comportamento de um semáforo
void* semaforo_func(void* arg) {
    int semaforo_num = *((int*)arg);  // Converte o argumento recebido para um número inteiro, que será o número do semáforo

    // Loop infinito para manter o ciclo dos semáforos funcionando continuamente
    while (1) {
        // Se o semáforo for o número 1, controla o funcionamento do primeiro semáforo
        if (semaforo_num == 1) {
            sem_wait(&semaforo1);  // Aguarda até o semáforo 1 estar "verde" (liberado)

            // Exibe que o Semáforo 1 está verde e o Semáforo 2 está vermelho
            printf("\rSemaforo 1: VERDE     | Semaforo 2: VERMELHO");
            fflush(stdout);  // Garante que o texto seja imediatamente exibido no terminal
            sleep(3);  // Simula o tempo em que o semáforo 1 permanece verde (3 segundos)

            // Agora o Semáforo 1 está amarelo e o Semáforo 2 continua vermelho
            printf("\rSemaforo 1: AMARELO   | Semaforo 2: VERMELHO");
            fflush(stdout);  // Atualiza a saída imediatamente
            sleep(1);  // Simula o tempo do amarelo (1 segundo)

            // O Semáforo 1 fica vermelho, e o Semáforo 2 ainda está vermelho
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: VERMELHO");
            fflush(stdout);  // Atualiza a saída para exibir os dois semáforos vermelhos
            sem_post(&semaforo2);  // Libera o semáforo 2, permitindo que ele mude para verde
            sleep(3);  // Semáforo 1 fica vermelho por mais 3 segundos
        } else {
            // Caso contrário, controla o funcionamento do Semáforo 2
            sem_wait(&semaforo2);  // Aguarda até o semáforo 2 estar "verde" (liberado)

            // Exibe que o Semáforo 2 está verde e o Semáforo 1 está vermelho
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: VERDE    ");
            fflush(stdout);  // Atualiza a saída para refletir o estado atual
            sleep(3);  // Semáforo 2 fica verde por 3 segundos

            // Agora o Semáforo 2 está amarelo e o Semáforo 1 continua vermelho
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: AMARELO ");
            fflush(stdout);  // Atualiza a saída
            sleep(1);  // Semáforo 2 fica amarelo por 1 segundo

            // Ambos os semáforos estão vermelhos novamente
            printf("\rSemaforo 1: VERMELHO  | Semaforo 2: VERMELHO");
            fflush(stdout);  // Atualiza a saída com ambos vermelhos
            sem_post(&semaforo1);  // Libera o semáforo 1, permitindo que ele mude para verde
            sleep(3);  // Semáforo 2 permanece vermelho por mais 3 segundos
        }
    }

    return NULL;  // A função retorna NULL, mas esse retorno nunca será alcançado por causa do loop infinito
}

int main() {
    pthread_t thread1, thread2;  // Declara duas threads para controlar cada semáforo
    int semaforo_num1 = 1;  // Número da primeira thread (semáforo 1)
    int semaforo_num2 = 2;  // Número da segunda thread (semáforo 2)

    // Inicializa os semáforos
    // O semáforo 1 começa com valor 1, ou seja, ele inicia "verde"
    sem_init(&semaforo1, 0, 1);  // Inicializa semáforo 1 com valor inicial de 1 (liberado)
    // O semáforo 2 começa com valor 0, ou seja, ele inicia "vermelho"
    sem_init(&semaforo2, 0, 0);  // Inicializa semáforo 2 com valor inicial de 0 (bloqueado)

    // Cria as duas threads que irão executar a função dos semáforos
    pthread_create(&thread1, NULL, semaforo_func, &semaforo_num1);  // Thread para o semáforo 1
    pthread_create(&thread2, NULL, semaforo_func, &semaforo_num2);  // Thread para o semáforo 2

    // Aguarda a conclusão das threads (o que não acontece aqui, pois o loop é infinito)
    pthread_join(thread1, NULL);  // Aguarda a thread 1 terminar (nunca termina)
    pthread_join(thread2, NULL);  // Aguarda a thread 2 terminar (nunca termina)

    // Destroi os semáforos ao final da execução (nunca será alcançado devido ao loop infinito)
    sem_destroy(&semaforo1);  // Libera os recursos alocados pelo semáforo 1
    sem_destroy(&semaforo2);  // Libera os recursos alocados pelo semáforo 2

    return 0;  // Retorna 0 para indicar que o programa terminou sem erros
}


