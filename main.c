#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "cola.h"
#include "netinet/in.h"
#include "string.h"
#include "fcntl.h"
#include "signal.h"
#include "pthread.h"


#define BUFSIZE 8096
void* procesarConsulta(void* fd);
pthread_t  *threads = NULL;

pthread_t  *threadsEnqueued = NULL;

int *busyThreads  = NULL;
int threadLenght;
int available=0;
int indexAvailable=0;
int indexThreads =0;
int modo;
int socketfd;
int main(int argc, char ** argv) {
    //arg1 puerto
    //arg2 direccion de la carpeta wwww
    //arg3 numero del modo
    // si arg3 = 4 o 5 , arg4 k procesos que van a correr

    modo = atoi(argv[3]);


    /*if ((modo == 4 || modo == 5) && argc != 5) {
        printf("\nLa forma correcta de correrlo es: ServidorHTTP puerto ruta modo\nSi el modo es 3(Pre-forked) o 4(Pre-threaded) se agrega un cuarto de los k procesos que van a correr \nEjemplo: ServidorHTTP 8081 temp/wwwroot 2\n");
        exit(0);
    }
    else if (modo < 0 || modo > 3 || argc != 4) {
        printf("\nLa forma correcta de correrlo es: ServidorHTTP puerto ruta modo\nSi el modo es 3(Pre-forked) o 4(Pre-threaded) se agrega un cuarto de los k procesos que van a correr \nEjemplo: ServidorHTTP 8081 temp/wwwroot 2\n");
        exit(0);
    }*/
    if (chdir(argv[2]) == -1) {
        printf("\nError con la ruta %s verifiquela y vuelva a intentarlo\n", argv[2]);
    }

    int puerto = atoi(argv[1]);
    int listenfd; //fd;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nOcurrio un error creado el socket\n");
        exit(0);
    }
    socklen_t length;
    static struct sockaddr_in cli_addr, serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(puerto);


    if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nOcurrio un error en el metodo bind\n");
        exit(0);
    }
    if (listen(listenfd, 64) < 0) {
        printf("\nOcurrio un error en el metodo listen\n");
        exit(0);
    }

//
//    //FIFO
    if (modo == 1) {
        create(); //crear la cola

        for (; ;) {
            length = sizeof(cli_addr);
            if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
                printf("\nOcurrio un error mientras se escuchaban conexiones");
            }
            else {

                // PROCESO DE FIFO
                printf("\nSe agrego una consulta en la cola de espera");
                enq(socketfd);
                fd = deq();
                printf("\nSe desencolo una consulta en la cola de espera");
                // FINAL PROCESO DE FIFO
                procesarConsulta((void *) &fd);
                close(socketfd);

            }
        }
    }
//        //FORKED
    if (modo == 2) {
        int pid;
        (void) signal(SIGCLD, SIG_IGN); /* ignore child death */
        (void) signal(SIGHUP, SIG_IGN); /* ignore terminal hangups */
        for (;;) {
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
                    procesarConsulta((void *) &socketfd);
                    exit(0);
                }
                else {
                    close(socketfd);
                }
            }
        }
    }
    //THREADED
     if (modo == 3) {
        for (;;) {
            length = sizeof(cli_addr);
            if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
                printf("\nOcurrio un error mientras se escuchaban conexiones");
            }
            else {
                pthread_t hilo;
                pthread_create(&hilo, NULL, procesarConsulta, (void *) &socketfd);
            }
        }
    }
    else if(modo == 5){
        create(); //crear la cola
        threadLenght = atoi(argv[4]);

        printf("%d\n",threadLenght);
        threads = malloc(sizeof(pthread_t)*threadLenght);
        busyThreads = malloc(sizeof(int)*threadLenght);
        threadsEnqueued = malloc(sizeof(pthread_t)*(threadLenght*2));

        int k;

        for (k=0; k< threadLenght; k++) {
            busyThreads[k] =0;
        }

        for(;;){
            length = sizeof(cli_addr);
            if ((socketfd = accept(listenfd, (struct sockaddr *) &cli_addr, &length)) < 0) {
                printf("\nOcurrio un error mientras se escuchaban conexiones");
            }
            else{
                available=-1;
                int u;
                for (u = 0; u < threadLenght && available==-1; u++) {
                    if (busyThreads[u] == 0) {
                        available = u;
                        indexAvailable = u;
                        busyThreads[u]=1;
                        break;
                    }
                }
                if (available == -1) {
                    printf("\nNo hay hilos disponibles\n");
                    enq(threadsEnqueued[indexThreads]);
                    indexThreads++;
                    if(queuesize()>0){
                        printf("Se encolo y el indexThreads es: %d\n",indexThreads);
                    }
//                  close(socketfd);
                }
                else{
                    printf("\nSe empezo a utilizar el hilo: %d\n", indexAvailable+1);
                    pthread_create(&threads[indexAvailable],NULL,procesarConsulta,(void*)&socketfd);
                }
            }
        }
    }
    return 0;
}

void* procesarConsulta(void* fd_t) {
    int fd = *((int *) fd_t);
    long i, ret, len;
    char *fstr;
    int file_fd;
    static char buffer[BUFSIZE + 1];
    ret = read(fd, buffer, BUFSIZE);

    if (ret > 0 && ret < BUFSIZE)
        buffer[ret] = 0;
    else buffer[0] = 0;
    for (i = 0; i < ret; i++)
        if (buffer[i] == '\r' || buffer[i] == '\n')
            buffer[i] = '*';
    for (i = 4; i < BUFSIZE; i++) {
        if (buffer[i] == ' ') {
            buffer[i] = 0;
            break;
        }
    }

    if ((file_fd = open(&buffer[5], O_RDONLY)) == -1) {
        (void) write(fd,
                     "HTTP/1.1 404 Not Found\nContent-Length: 136\nContent-Type: text/html\n\n<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe requested URL was not found on this server.\n</body></html>\n",
                     224);
        close(fd);
    }


    fstr = "binary";//" application/octet-stream";
    len = (long) lseek(file_fd, (off_t) 0, SEEK_END); // ir al final del archivo para saber el tamano
    (void) lseek(file_fd, (off_t) 0, SEEK_SET);     // volver al inicio
    (void) sprintf(buffer, "HTTP/1.1 200 OK\nServer: "
            "ServidorHTTP/1.0\nContent-Length: %ld\n"
            "Content-Type: %s\n\n", len, fstr);
    (void) write(fd, buffer, strlen(buffer));


    while ((ret = read(file_fd, buffer, BUFSIZE)) > 0) {
        (void) write(fd, buffer, ret);
    }
    if(modo==5){
        printf("Disponible: %d\n",indexAvailable);
        busyThreads[indexAvailable]=0;
        printf("Se libero el hilo %d\n",indexAvailable+1);
        indexAvailable--;
    }
    sleep(1);
    close(fd);
    if(!empty()&&modo==5){
        printf("No esta vacia");
        indexAvailable++;
        threads[indexAvailable]=deq();
        indexThreads--;
        pthread_create(&threads[indexAvailable],NULL,procesarConsulta,(void*)&socketfd);
        printf("\nSe empezo a utilizar el hilo: %d\n", indexAvailable+1);
    }
    return NULL;



}