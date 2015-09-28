//
// Created by josue on 9/28/15.
//

#ifndef SERVIDORHTTP_FIFO_H
#define SERVIDORHTTP_FIFO_H

#include "netinet/in.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "../consulta/procesarConsulta.h"


void fifo(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr);

#endif //SERVIDORHTTP_FIFO_H
