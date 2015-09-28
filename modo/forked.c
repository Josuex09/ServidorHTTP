//
// Created by josue on 9/28/15.
//

#include "forked.h"


void forked(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr){
    int pid;
    (void)signal(SIGCLD, SIG_IGN); /* ignore child death */
    (void)signal(SIGHUP, SIG_IGN); /* ignore terminal hangups */
    for(;;){
        length = sizeof(cli_addr);
        if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
            printf("\nOcurrio un error mientras se escuchaban conexiones");
        }
        else {
            pid = fork();
            if (pid < 0) {
                printf("\nHa ocurrido un error creado el proceso\n");
            }
            else if (pid == 0) {
                procesarConsulta((void *)&socketfd);
                exit(0);
            }
            else {
                close(socketfd);
            }
        }
    }
}