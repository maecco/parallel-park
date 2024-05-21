#ifndef __SHARED_H__
#define __SHARED_H__

// Muda a implementaçao da funçao get_client
// Se os ids dos clientes sempre forem sequenciais = i - 1
// E possivel usar a versao "unsafe"
// #define SAFE_GET_CLIENT

#include <defs.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

// GLOBAL VARS
extern client_args* g_cliArgs;
extern int no_clients;

// GLOBAL FUNCTIONS

// Funçao usada para encontrar o cliente correto
// baseado no seu id
client_t* getClient(int id);

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif