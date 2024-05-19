#ifndef __SHARED_H__
#define __SHARED_H__

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