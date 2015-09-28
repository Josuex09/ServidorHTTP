//
// Created by josue on 9/28/15.
//

#include "fifo.h"


void fifo(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr){
    for (;;) {
        length = sizeof(cli_addr);
        if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
            printf("\nOcurrio un error mientras se escuchaban conexiones");
            exit(0);
        }
        else {
            procesarConsulta((void *)&socketfd);
            close(socketfd);

        }
    }
}