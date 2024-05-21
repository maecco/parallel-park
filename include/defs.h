#ifndef __DEFS_H__
#define __DEFS_H__

/* Essa biblioteca implementa definicoes que sao usadas pelo programa. */
/* ATENÇÃO: NÃO APAGUE OU EDITE O NOMES FORNECIDOS */

#include <pthread.h>
#include <semaphore.h>
#include <errno.h> // ETIMEDOUT
#include <time.h> // time_t

#define TRUE 1              // Em C nao temos True ou False (como no Python). Usamos macro TRUE para True (como no Python). 
#define FALSE 0             // e FALSE para False (como no Python).

#define MILLION 1000000     // 1 milhao. Usado para converter segundos em milisegundos.
#define BILLION 1000000000  // 1 bilhao. 

#define MAX_CAPACITY_TOY    3  // Capacidade maxima dos brinquedos.
#define MIN_CAPACITY_TOY    1  // Capacidade minima dos brinquedos.
#define MAX_COINS           5 // Maximo de moedas que um cliente pode comprar
// TIMEDEFS
#define MAX_WAIT_TIME            300  // Tempo limite de espera para um brnquedo (em ms)
#define MIN_WAIT_TIME            100  // Tempo minimo de espera para um brinquedo (em ms)
#define MAX_RIDE_TIME            200  // Tempo maximo de um brinquedo (em ms)
#define MIN_RIDE_TIME            50   // Tempo minimo de um brinquedo (em ms)

#define DEBUG               1   //  Alterne (0 or 1) essa macro se voce espera desabilitar todas as mensagens de debug.


/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct ticket{
  pthread_t thread;         // A thread do funcionário que fica na bilheteria.
  int id;                   // O id do funcionario que fica na bilheteria.
} ticket_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct toy{
  int id;                   // O id de um brinquedo.
  int capacity;             // A capacidade total de um brinquedo.
  pthread_t thread;         // A thread de um brinquedo.
  // Controle
  int onboard_n;            // Numero de pessoas no brinquedo.
  int* onboardID;           // Array de clientes(ID) no brinquedo.
  int msRide;               // Tempo de duração de um brinquedo.
  int msWait;               // Tempo de espera maximo para um brinquedo.
  struct timespec ts;       // Timespec auxiliar para o timer.
  // Sync
  sem_t hasSpace;               // Semáforo para indicar quantos lugares vagos ha ainda
  sem_t canEnter;               // Semáforo que indica se o brinquedo esta parado ou nao
  sem_t startTimer;             // Semáforo para iniciar o timer na hora certa.
  pthread_mutex_t clientAccess; // Mutex para controlar a quem esta falando com o brinquedo.
  // Cond Sync
  pthread_cond_t full;      // Condiçao para indicar que o brinquedo esta cheio.(deve iniciar)
  pthread_mutex_t startLock; // Mutex para controlar o inicio do brinquedo.
} toy_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct client{
  int id;                   // O id do cliente.
  int coins;                // Quantidade de moedas do cliente.
  int number_toys;          // Numero de brinquedos disponiveis.
  toy_t **toys;             // (Copy) Array de brinquedos.
  pthread_t thread;         // A thread do cliente.
  // Sync
  sem_t canProcede;         // Semáforo para o cliente poder prosseguir com o que esta requisitando.
} client_t;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  ticket_t **tickets;        // Array de funcionarios.                       
  int n;                    // Quantidade de funcionários da bilheteria.
} tickets_args;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  toy_t **toys;             // Array de brinquedos.
  int n;                    // Quantidade de brinquedos.
} toy_args;

/* Adicione as estruturas de sincronização que achar necessárias */
typedef struct{
  client_t **clients;       // Array de clientes.
  int n;                    // Quantidade de clientes.
} client_args;


/**************************************************
 *                    ATENÇÃO                     *
 *   NÃO EDITAR ESSE ARQUIVO A PARTIR DESTE PONTO *
 *                    ATENÇÃO                     *
 *************************************************/

// Estrutura da fila
typedef struct node {
  int data;                 // Dado da fila.
  struct node *next;        // Objeto do proximo item da fila.
} Node;

// Fila
typedef struct queue {
  Node *front;              // Primeiro elemento da fila.
  Node *rear;               // Ultimo elemento da fila.
} Queue;

#if DEBUG
#define debug(...) printf(__VA_ARGS__);
#else
#define debug(...);
#endif


#endif