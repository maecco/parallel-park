#include "shared.h"

#include <queue.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

// lista com os clientes
client_args* g_cliArgs = NULL;
// variavel globas para indicar se ainda ha clients
int no_clients = FALSE;


// Funçao usada para encontrar os clientes baseado no seu id
#ifdef SAFE_GET_CLIENT // definido em shared.h
client_t* getClient(int id){
    for (int i = 0; i < g_cliArgs->n; i++){
        if (g_cliArgs->clients[i]->id == id)
            return g_cliArgs->clients[i];
    }
    return NULL;
}
#else 
client_t* getClient(int id){
    return g_cliArgs->clients[id-1];
}
#endif


/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;