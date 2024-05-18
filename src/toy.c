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

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

    pthread_exit(NULL);
}

// ######################### AUX FUNCTIONS ############################


void wait_crowd(toy_t *self){
}

void startRide(toy_t *self){
}

void freeRide(toy_t *self){
}


// ########################### WRAPPER FUNCTIONS ############################


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    // Sua lógica aqui

    // guarda os argumentos de toy_args
    // para serem acesciveis em todo arquivo
    toys = args->toys;
    n_toys = args->n;

    //inicia a thread para cada brinquedo
    for(int i = 0; i < n_toys; i++){
        pthread_create(&toys[i]->thread, NULL, turn_on, (void *) toys[i]);
    }
}

// Desligando os brinquedos
void close_toys(){
    // Sua lógica aqui

    // Espera as threads dos brinquedos terminarem
    for(int i = 0; i < n_toys; i++){
        pthread_join(toys[i]->thread, NULL);
    }
}