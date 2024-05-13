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


// Thread que implementa uma bilheteria
void *sell(void *args){

    debug("[INFO] - Bilheteria Abriu!\n");
    while ( is_queue_empty(gate_queue) == 0){        
        int client_id = dequeue(gate_queue);
        client_t *client = getClient(client_id);
        sem_post(&client->canProcede);
    }
    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    // Sua lógica aqui

    // guarda os argumentos de tickets_args
    // para serem acesciveis em todo arquivo
    tickets = args->tickets;
    n_tickets = args->n;
    
    // inicia a thread para cada bilheteria
    for (int i = 0; i < n_tickets; i++){
        pthread_create(&tickets[i]->thread, NULL, sell, (void *) tickets[i]);
    }    
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    //Sua lógica aqui
    // espera as threads dos atendentes terminarem
    for (int i = 0; i < n_tickets; i++){
        pthread_join(tickets[i]->thread, NULL);
    }
}