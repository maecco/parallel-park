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
    toy_t *toy = (toy_t*) args;

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo

    while ( TRUE )
    {
        wait_crowd(toy);
        if ( no_clients ) { break; }
        startRide(toy);
        freeRide(toy);
    }
    

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

    pthread_exit(NULL);
}

// ######################### AUX FUNCTIONS ############################


void wait_crowd(toy_t *self){
    
    debug("[WAIT] - O brinquedo [%d] está esperando por turistas.\n", self->id);
    pthread_mutex_lock(&self->startLock);
    while ( self->onboard_n < self->capacity )
    {
        // Permite a entrada
        sem_post(&self->canEnter);
        // Espera um sinal para começar
        pthread_cond_wait(&self->full, &self->startLock);
    }
}

void startRide(toy_t *self){
    debug("[START] - O brinquedo [%d] começou a andar.\n", self->id);
    sleep(1);
    debug("[END] - O brinquedo [%d] terminou a andar.\n", self->id);
}

void freeRide(toy_t *self){
    pthread_mutex_lock(&self->clientAccess);
    for ( int i = 0; i < self->onboard_n; i++ ) {
        client_t* cli = getClient(self->onboardID[i]);
        cli->coins--;
        sem_post(&cli->canProcede);
        self->onboardID[i] = -1;
    }
    self->onboard_n = 0;
    pthread_mutex_unlock(&self->clientAccess);
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
        pthread_mutex_init(&toys[i]->clientAccess, NULL);
        pthread_mutex_init(&toys[i]->startLock, NULL);
        sem_init(&toys[i]->hasSpace, 0, toys[i]->capacity);
        sem_init(&toys[i]->canEnter, 0, 0);
        pthread_cond_init(&toys[i]->full, NULL);
        toys[i]->onboardID = (int*) malloc(toys[i]->capacity * sizeof(int));
        pthread_create(&toys[i]->thread, NULL, turn_on, (void *) toys[i]);
    }
}

// Desligando os brinquedos
void close_toys(){

    // Espera as threads dos brinquedos terminarem
    for(int i = 0; i < n_toys; i++){
        pthread_mutex_destroy(&toys[i]->clientAccess);
        pthread_mutex_destroy(&toys[i]->startLock);
        sem_destroy(&toys[i]->hasSpace);
        sem_destroy(&toys[i]->canEnter);
        pthread_cond_destroy(&toys[i]->full);
        free(toys[i]->onboardID);
        pthread_join(toys[i]->thread, NULL);
    }
}