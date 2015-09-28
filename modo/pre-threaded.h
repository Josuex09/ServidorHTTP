//
// Created by josue on 9/28/15.
//

#ifndef SERVIDORHTTP_PRE_THREADED_H
#define SERVIDORHTTP_PRE_THREADED_H


#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "netinet/in.h"
#include "string.h"
#include "fcntl.h"
#include "pthread.h"

#include "../consulta/procesarConsulta.h"

void prethreaded(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr,int k);
void *procesarThread(void *args);

#endif //SERVIDORHTTP_PRE_THREADED_H
