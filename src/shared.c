#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui


client_args* g_cliArgs = NULL;

client_t* getClient(int id){
    for (int i = 0; i < g_cliArgs->n; i++){
        if (g_cliArgs->clients[i]->id == id)
            return g_cliArgs->clients[i];
    }
    return NULL;
}



/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;