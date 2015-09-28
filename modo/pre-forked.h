//
// Created by josue on 9/28/15.
//

#ifndef SERVIDORHTTP_PRE_FORKED_H
#define SERVIDORHTTP_PRE_FORKED_H

#include "netinet/in.h"
#include "unistd.h"
#include "stdio.h"
#include "signal.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "../consulta/procesarConsulta.h"

void preforked(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr,int k);
void sig_int(int signo);

#endif //SERVIDORHTTP_PRE_FORKED_H
