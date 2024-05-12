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

// VARS
client_t **clients;
int n_clients;


// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    //Sua lógica aqui


    debug("[EXIT] - O turista saiu do parque.\n");
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    // Sua lógica aqui
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Sua lógica aqui
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Sua lógica aqui.
    // coloca os clientes na fila
    enqueue(gate_queue, self->id);
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);
    // cliente deve esperar ser chamado por uma thread ticket
    // para comprar moedas
    wait_ticket(self);
    // compra as moedas
    buy_coins(self);
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);

    // cria a thread que representara o clinete no parque
    pthread_create(&self->thread, NULL, enjoy, (void *)self);

    // Sua lógica aqui.
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // guarda as variaveis dos clientes para uso nesse arquivo
    clients = args->clients;
    n_clients = args->n;

    // chama os clientes para entrar na fila
    for (int i = 0; i < args->n; i++)
        queue_enter(args->clients[i]);
}

// Essa função deve finalizar os clientes
void close_gate(){
   // espera os clientes acabarem
    for (int i = 0; i < n_clients; i++)
        pthread_join(clients[i]->thread, NULL);
    debug("[FINISHED] - Todos os turistas sairam do parque.\n");
}
