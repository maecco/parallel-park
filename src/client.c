/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"


// ######################### THREAD FUNCTION ############################

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){    
    client_t* self = (client_t *) arg;
    // Espera na fila para comprar moedas
    queue_enter(self);
    // Entra no parque
    // Main loop client
    while ( self->coins > 0 ){
        // Seleciona um brinquedo aleatoriamente
        int toy_id = rand() % self->number_toys;
        toy_t* toy = self->toys[toy_id];
        // Entra na fila do brinquedo
        go_ride(self, toy);
        debug("{CLIENT %d} - Possui [%d] moedas.\n", self->id, self->coins);
    }

    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}


// ######################### AUX FUNCTIONS ############################


// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    self->coins = rand() % MAX_COINS + 1;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    sem_wait(&self->canProcede);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // entra na fila principal para comprar moedas
    enqueue(gate_queue, self->id);
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);
    // espera sua vez
    wait_ticket(self);
    // compra as moedas
    buy_coins(self);
    debug("{CLIENT %d} - Comprou [%d] moedas.\n", self->id, self->coins);
}


void go_ride(client_t* self, toy_t* toy) {
    debug("{CLIENT %d} - Na fila do brinquedo [%d].\n", self->id, toy->id);
    // Espera ter espaço no brinquedo
    sem_wait(&toy->hasSpace);
    // Espera o brinquedo parar para entrar
    sem_wait(&toy->canEnter);
    // Espera sua vez para entrar
    pthread_mutex_lock(&toy->clientAccess);
    // Entra no brinquedo e registra sua entrada
    // ID do cliente no brinquedo
    toy->onboardID[toy->onboard_n] = self->id;
    // Incrementa o numero de pessoas no brinquedo
    toy->onboard_n++;
    debug("{CLIENT %d} - Entrou no brinquedo [%d].\n", self->id, toy->id);
    // Primeiro cliente
    if (toy->onboard_n == 1 ) { 
        // Inicia o timer do brinquedo 
        toy->waitSeconds = MAX_TIME;
        sem_post(&toy->startTimer);
    }
    // Se o brinquedo estiver cheio, sinaliza para iniciar
    if (toy->onboard_n == toy->capacity) {
        pthread_cond_signal(&toy->full);
    }
    // Do contrario apenas libera a entrada
    else {
        sem_post(&toy->canEnter);
    }
    // Libera a comunicaçao com o brinquedo
    pthread_mutex_unlock(&toy->clientAccess);
    // Espera o brinquedo acabar de rodar para seguir no parque
    sem_wait(&self->canProcede);
    debug("{CLIENT %d} - Saiu do brinquedo [%d].\n", self->id, toy->id);
}



// ########################### WRAPPER FUNCTIONS ############################

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // guarda a cli_args globalmente
    g_cliArgs = args;

    // inicializa as threads e estruturas de sincronizaçao dos clientes
    for (int i = 0; i < args->n; i++){
        sem_init(&args->clients[i]->canProcede, 0, 0);
        pthread_create(&args->clients[i]->thread, NULL, enjoy, (void *)args->clients[i]);
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
   // espera os clientes acabarem e finaliza as threads e semaforos
    for (int i = 0; i < g_cliArgs->n; i++){
        pthread_join(g_cliArgs->clients[i]->thread, NULL);
        sem_destroy(&g_cliArgs->clients[i]->canProcede);
    }
    debug("[FINISHED] - Todos os turistas sairam do parque.\n");
    no_clients = TRUE;
}
