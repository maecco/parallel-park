#ifndef __TOY_H__
#define __TOY_H__

#include <defs.h>


// Você pode declarar novas funções aqui


void wait_crowd(toy_t *self);
void startRide(toy_t *self);
void freeRide(toy_t *self);
void setClock(struct timespec *ts, int ms);


/********************************
 *          ATENÇÃO             *
 *   NÃO EDITAR ESSAS FUNÇÕES   *
 *          ATENÇÃO             *
 *******************************/
void open_toys(toy_args *args);
void close_toys();

#endif