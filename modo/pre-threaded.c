//
// Created by josue on 9/28/15.
//

#include "pre-threaded.h"

typedef struct {  // Tipo para mantener los hilos
    pthread_t id;
} Hilo;

Hilo * hilos; // arreglo de hilos

int	obtenida, procesada;
int consultas[32];

pthread_mutex_t	mutex;
pthread_cond_t	cond;


void prethreaded(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr,int k){

    int *hiloActual;
    hilos = calloc(k,sizeof(pthread_t));
    int i;
    for (i = 0; i < k; i++) {
        hiloActual = malloc(sizeof(int));
        *hiloActual = i;
        if (pthread_create(&hilos[0].id, NULL, &procesarThread, (void *) hiloActual))
            printf("Error creando el pthread");
    }

    obtenida = procesada = 0;

    while(1){
        length = sizeof(cli_addr);
        if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
            printf("\nOcurrio un error mientras se escuchaban conexiones");
        }
        pthread_mutex_lock(&mutex);
        consultas[procesada] = socketfd;
        if (++procesada == 32)
            procesada = 0;
        if (procesada == obtenida)
            printf("\nError \n");
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

    }
}

void *procesarThread(void *args){
    int socket;
    free(args);

    pthread_detach(pthread_self());

    while(1){
        pthread_mutex_lock(&mutex);
        while (obtenida == procesada)
            pthread_cond_wait(&cond, &mutex);
        socket = consultas[obtenida];	/* connected socket to service */
        if (++obtenida == 32)
            obtenida = 0;
        pthread_mutex_unlock(&mutex);
        procesarConsulta((void*)&socket);
        close(socket);

    }
    return 0;
}