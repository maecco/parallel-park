/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include <queue.h>
#include "shared.h"

// tickets_args
ticket_t **tickets;
int n_tickets;
// access queue mutex
sem_t queueAccess;


// ######################### THREAD FUNCTION ############################

// Thread que implementa uma bilheteria
void *sell(void *args){
    ticket_t* ticket  = (ticket_t*) args;
    debug("[INFO] - Bilheteria Abriu!\n");
    // enquanto ainda houver clientes na fila
    while ( 1 ){
        // Solicita acesso a fila
        sem_wait(&queueAccess);
        // recebe o id do proximo cliente da fila
        int client_id = dequeue(gate_queue);
        // Se a fila estiver vazia, sai do loop para encerrar as atividades
        if (client_id == -1){ 
            break; 
        } else {
            debug("{TICKET %d} - Atendendo cliente [%d]\n", ticket->id, client_id);
            // Libera o acesso da fila
            sem_post(&queueAccess);
            // Busca o objeto cliente pelo id fornecido e o libera para prosseguir
            client_t *client = getClient(client_id);
            sem_post(&client->canProcede);
        }
    }
    pthread_exit(NULL);
}

// ######################### AUX FUNCTIONS ############################
// NONE :P
// ########################### WRAPPER FUNCTIONS ############################


// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    // Guarda os argumentos de tickets_args
    // Para serem acesciveis em todo arquivo
    tickets = args->tickets;
    n_tickets = args->n;

    // Inicializa o semaforo trancado
    // Clientes nao entram ate que todas as bilheterias estejam prontas
    sem_init(&queueAccess, 0, 0);
    
    // Inicia a thread para cada bilheteria
    for (int i = 0; i < n_tickets; i++){
        pthread_create(&tickets[i]->thread, NULL, sell, (void *) tickets[i]);
    }    

    // libera o semaforo da fila
    // para que os clientes possam entrar
    sem_post(&queueAccess);
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    // Espera as threads dos atendentes terminarem
    for (int i = 0; i < n_tickets; i++){
        debug("{TICKET %d} - Fechou!\n", tickets[i]->id);
        pthread_join(tickets[i]->thread, NULL);
    }
    // Finaliza o semaforo da fila
    sem_destroy(&queueAccess);
}