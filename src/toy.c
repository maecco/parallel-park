/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"


// Toy_args
toy_t **toys;
int n_toys;

// ######################### THREAD FUNCTION ############################

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    toy_t *self = (toy_t*) args;

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", self->id); // Altere para o id do brinquedo
    const char* str = "TOYSPEC | ID %d | Capacidade [%d] |\
Tempo de espera [%d ms] | Tempo de percurso [%d ms] \n";
    debug(str, self->id, self->capacity, self->msWait, self->msRide);

    // Seta as condiçoes iniciais
    pthread_mutex_lock(&self->clientAccess);
    for ( int i = 0; i < self->capacity; i++ ) {
        sem_post(&self->hasSpace);
    }

    while ( TRUE )
    {
        // Espera por turistas
        wait_crowd(self);
        // Se nao houver mais turistas, desliga o brinquedo
        if ( no_clients ) { break; }
        // Inicia o brinquedo
        startRide(self);
        // Libera os turistas e reseta o brinquedo
        freeRide(self);
    }
    debug("[OFF] - O brinquedo [%d] foi desligado.\n", self->id); // Altere para o id do brinquedo
    pthread_exit(NULL);
}

// ######################### AUX FUNCTIONS ############################


void wait_crowd(toy_t *self){
    
    debug("{TOY %d} - Esperando por turistas.\n", self->id);
    // Enquanto houver espaço no brinquedo e houver clientes no parque
    while ( self->onboard_n < self->capacity && !no_clients ) {
        // Libera a entrada de clientes
        pthread_mutex_unlock(&self->clientAccess);
        // Espera o sinal do primeiro cliente para iniciar o timer de espera
        sem_wait(&self->startTimer);
        setClock(&self->ts, self->msWait);
        // Print apenas para debug...
        if ( no_clients == FALSE ) {
            debug("{TOY %d} - Contando tempo de espera [%d ms]\n", self->id, self->msWait);
        }
        // Espera um sinal de que o brinquedo esteja cheio
        // ou que o tempo limite tenha sido atingido
        pthread_mutex_lock(&self->startLock);
        int ret = pthread_cond_timedwait(&self->full, &self->startLock, &self->ts);
        pthread_mutex_unlock(&self->startLock);
        // Bloqueia a comunicaçao de clientes com o brinquedo
        pthread_mutex_lock(&self->clientAccess);
        // Se a condiçao abaixo for falsa, significa que todos os clientes
        // ja sairam do parque e o brinquedo deve ser desligado
        // Se for verdadeira o brinquedo deve começar
        if ( no_clients == FALSE ) { 
            if ( ret == ETIMEDOUT ) {
                debug("{TOY %d} - Iniciando por excesso de espera.\n", self->id);
                break;
            } else {
                debug("{TOY %d} - Está cheio.\n", self->id);
            }
        }
    }
}

// Funçao simbolica, apenas para simular o brinquedo andando
void startRide(toy_t *self){
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = self->msRide * MILLION;
    debug("{TOY %d} - Começou a andar. [%d ms]\n", self->id, self->msRide);
    nanosleep(&ts, NULL);
    debug("{TOY %d} - Terminou de andar.\n", self->id);
}

void freeRide(toy_t *self){
    // Libera os clientes que estavam no brinquedo
    for ( int i = 0; i < self->onboard_n; i++ ) {
        // Busca o cliente pelo id
        client_t* cli = getClient(self->onboardID[i]);
        // Deduz uma moeda do cliente
        cli->coins--;
        // Aval para o cliente prosseguir
        sem_post(&cli->canProcede);
        // Remove o cliente do brinquedo
        self->onboardID[i] = -1;
        // Libera a entrada de um novo cliente
        sem_post(&self->hasSpace);
    }
    self->onboard_n = 0;
}


void setClock(struct timespec *ts, int ms){
    // Pega o tempo atual
    clock_gettime(CLOCK_REALTIME, ts);
    // Adiciona o tempo em ms
    ts->tv_nsec += ms * MILLION;
    // Se ts->tv_nsec for maior que 1 segundo (1 bilhao de nanosegundos)
    // Incrementa o segundo de tv_sec e diminui 1 segundo de ts->tv_nsec
    // Caso contrario as operaçoes abaixo nao tem efeito
    ts->tv_sec += ts->tv_nsec / BILLION;
    ts->tv_nsec = ts->tv_nsec % BILLION;
}


// ########################### WRAPPER FUNCTIONS ############################


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){

    // guarda os argumentos de toy_args
    // para serem acesciveis em todo arquivo
    toys = args->toys;
    n_toys = args->n;

    //inicia a thread para cada brinquedo
    for(int i = 0; i < n_toys; i++){
        toys[i]->msWait = rand() % (MAX_WAIT_TIME - MIN_WAIT_TIME + 1) + MIN_WAIT_TIME;
        toys[i]->msRide = rand() % (MAX_RIDE_TIME - MIN_RIDE_TIME + 1) + MIN_RIDE_TIME;
        pthread_mutex_init(&toys[i]->clientAccess, NULL);
        pthread_mutex_init(&toys[i]->startLock, NULL);
        sem_init(&toys[i]->hasSpace, 0, 0);
        pthread_cond_init(&toys[i]->full, NULL);
        toys[i]->onboardID = (int*) malloc(toys[i]->capacity * sizeof(int));
        pthread_create(&toys[i]->thread, NULL, turn_on, (void *) toys[i]);
    }
}

// Desligando os brinquedos
void close_toys(){

    for (int i = 0; i < n_toys; i++){
        toy_t* toy = toys[i];
        toy->msWait = 0;
        sem_post(&toy->startTimer);
    }

    // Espera as threads dos brinquedos terminarem
    for(int i = 0; i < n_toys; i++){
        pthread_mutex_destroy(&toys[i]->clientAccess);
        pthread_mutex_destroy(&toys[i]->startLock);
        sem_destroy(&toys[i]->hasSpace);
        pthread_cond_destroy(&toys[i]->full);
        free(toys[i]->onboardID);
        pthread_join(toys[i]->thread, NULL);
    }
}