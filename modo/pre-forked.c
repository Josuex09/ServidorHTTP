//
// Created by josue on 9/28/15.
//

#include "pre-forked.h"


static pid_t *pids;
int cantHijos;
void preforked(socklen_t length,int socketfd,int listenfd,struct sockaddr_in cli_addr, int k) {
    cantHijos = k;
    pids = calloc(cantHijos, sizeof(pid_t));
    pid_t pidAct;
    int i;
    for (i = 0; i < k; i++) {
        pidAct = fork();
        switch (pidAct) {
            case -1:
                printf("\nError creando el proceso hijo\n");
                break;
            case 0:
                while (1) {
                    length = sizeof(cli_addr);
                    if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
                        printf("\nOcurrio un error mientras se escuchaban conexiones");
                        exit(0);
                    }
                    else {
                        procesarConsulta((void *) &socketfd);
                        close(socketfd);
                    }
                }
                //break;
            default:
                printf("\nSe creo el proceso #%d con el pid %d\n", i, pidAct);
                pids[i] = pidAct;
                break;
        }
    }
//signal(SIGCHLD,sig_chld);
    signal(SIGINT, sig_int);
    close(listenfd);
    for (;;)pause();

}

void sig_int(int signo){
    int i;
    for (i = 0; i < cantHijos; i++)
        kill(pids[i], SIGTERM);
    while (wait(NULL) > 0);
    exit(0);
}