#include "modo/fifo.h"
#include "modo/forked.h"
#include "modo/threaded.h"
#include "modo/pre-forked.h"
#include "modo/pre-threaded.h"


int modo;
int puerto;
int k;


int main(int argc, char ** argv){
    //arg1 puerto
    //arg2 direccion de la carpeta wwww
    //arg3 numero del modo
    // si arg3 = 4 o 5 , arg4 k procesos que van a correr

    modo = atoi(argv[3]);
    if( ((modo == 4 || modo ==5) && argc != 5) || ( (modo ==1 || modo ==2 || modo ==3 ) && argc !=4)){
        printf("\nLa forma correcta de correrlo es: ServidorHTTP puerto ruta modo\nSi el modo es 3(Pre-forked) o 4(Pre-threaded) se agrega un cuarto de los k procesos que van a correr \nEjemplo: ServidorHTTP 8081 temp/wwwroot 2\n");
        exit(0);
    }
    if(chdir(argv[2]) == -1){
        printf("\nError con la ruta %s verifiquela y vuelva a intentarlo\n",argv[2]);
    }

    puerto = atoi(argv[1]);
    int listenfd,socketfd = 0;

    if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0) {
        printf("\nOcurrio un error creado el socket\n");
        exit(0);
    }
    socklen_t length =NULL;
    static struct sockaddr_in cli_addr,serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(puerto);


    if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0) {
        printf("\nOcurrio un error en el metodo bind\n");
        exit(0);
    }
    if( listen(listenfd,64) <0) {
        printf("\nOcurrio un error en el metodo listen\n");
        exit(0);
    }

    //FIFO
    if(modo == 1) {
        fifo(length,socketfd,listenfd,cli_addr);
    }
        //FORKED
    else if(modo ==2 ){
        forked(length,socketfd,listenfd,cli_addr);
    }
        //THREADED
    else if(modo == 3){
        threaded(length,socketfd,listenfd,cli_addr);
    }
    // PRE-FORKED
    else if(modo == 4) {
        k = atoi(argv[4]);
        preforked(length,socketfd,listenfd,cli_addr,k);
    }
        //PRE-THREADED
    else if(modo == 5){
        int k = atoi(argv[4]); // cant de hilos
        prethreaded(length,socketfd,listenfd,cli_addr,k);
    }
    close(listenfd);

    return 0;
}
