//
// Created by josue on 9/28/15.
//

#ifndef SERVIDORHTTP_FORKED_H
#define SERVIDORHTTP_FORKED_H

#include "netinet/in.h"
#include "unistd.h"
#include "stdio.h"
#include "signal.h"
#include "stdlib.h"
#include "../consulta/procesarConsulta.h"
void forked(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr);
#endif //SERVIDORHTTP_FORKED_H
