#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <defs.h>

// Você pode declarar novas funções aqui


void queue_enter(client_t* client);
void go_ride(client_t* client, toy_t* toy);

/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_gate(client_args *args);
void close_gate();

#endif