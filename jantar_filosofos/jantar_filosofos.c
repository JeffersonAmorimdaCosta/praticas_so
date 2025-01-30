#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_FILOSOFOS 5

pthread_mutex_t garfos[NUM_FILOSOFOS];

void* filosofo(void* arg) {
    int id = *((int*)arg); // Id filósofo
    int garfo_esquerdo = id;
    int garfo_direito = (id + 1) % NUM_FILOSOFOS;

    while (1) {
        printf("Filosofo %d esta pensando.\n", id);
        sleep(1 + rand() % 3); // Pensando

        // Pegando os garfos em ordem para evitar deadlock
        printf("O filosofo %d esta com fome.\n", id);
        if (garfo_esquerdo < garfo_direito) {
            pthread_mutex_lock(&garfos[garfo_esquerdo]);
            pthread_mutex_lock(&garfos[garfo_direito]);
        } else {
            pthread_mutex_lock(&garfos[garfo_direito]);
            pthread_mutex_lock(&garfos[garfo_esquerdo]);
        }

        // Comendo
        printf("Filosofo %d esta comendo.\n", id);
        sleep(1 + rand() % 3); // Simula o tempo de alimentação

        // Soltando os garfos
        pthread_mutex_unlock(&garfos[garfo_esquerdo]);
        pthread_mutex_unlock(&garfos[garfo_direito]);

        printf("Filosofo %d terminou de comer e esta pensando novamente.\n", id);
    }

    return NULL;
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    srand(time(NULL));

    // Inicializando os mutexes
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&garfos[i], NULL);
    }

    // Criando as threads para cada filósofo
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    // Aguardando as threads (NUNCA VAI ACONTECER)
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Destruindo os mutexes (NUNCA VAI ACONTECER)
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&garfos[i]);
    }

    return 0;
}
