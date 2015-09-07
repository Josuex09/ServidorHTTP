#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "cola.h"
#include "netinet/in.h"
#include "string.h"
#include "fcntl.h"

#define BUFSIZE 8096

int main(int argc, char ** argv){
    //arg1 puerto
    //arg2 direccion de la carpeta wwww
    //arg3 numero del modo
    // si arg3 = 4 o 5 , arg4 k procesos que van a correr

    int modo = atoi(argv[3]);

    if((modo == 4 || modo ==5) && argc != 5){
        printf("\nLa forma correcta de correrlo es: ServidorHTTP puerto ruta modo\nSi el modo es 3(Pre-forked) o 4(Pre-threaded) se agrega un cuarto de los k procesos que van a correr \nEjemplo: ServidorHTTP 8081 temp/wwwroot 2\n");
        exit(0);
    }
    else if(modo <0 || modo > 3 || argc !=4){
        printf("\nLa forma correcta de correrlo es: ServidorHTTP puerto ruta modo\nSi el modo es 3(Pre-forked) o 4(Pre-threaded) se agrega un cuarto de los k procesos que van a correr \nEjemplo: ServidorHTTP 8081 temp/wwwroot 2\n");
        exit(0);
    }
    if(chdir(argv[2]) == -1){
        printf("\nError con la ruta %s verifiquela y vuelva a intentarlo\n",argv[2]);
    }

    int puerto = atoi(argv[1]);
    int listenfd,socketfd;

    if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0) {
        printf("\nOcurrio un error creado el socket\n");
        exit(0);
    }
    socklen_t length;
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



    //FIFO EMPIEZA AQUI
    create(); //crear la cola

    for(;;){
        length = sizeof(cli_addr);
        if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0) {
            printf("\nOcurrio un error mientras se escuchaban conexiones");
        }
        else{

            printf("\nSe encolo: %d\n",socketfd);
            enq(socketfd);
            int fd = deq();
            int file_fd;
            printf("\nSe desencolo: %d\n",fd);
            long i, ret, len;
            char * fstr;
            static char buffer[BUFSIZE+1];
            ret = read(fd,buffer,BUFSIZE);

            printf("\nConsulta : %s\n",buffer);


            if(ret > 0 && ret < BUFSIZE)
                buffer[ret]=0;
            else buffer[0]=0;
            for(i=0;i<ret;i++)
                if(buffer[i] == '\r' || buffer[i] == '\n')
                    buffer[i]='*';
            for(i=4;i<BUFSIZE;i++) {
                if(buffer[i] == ' ') {
                    buffer[i] = 0;
                    break;
                }
            }

            if ((file_fd = open(&buffer[5], O_RDONLY)) == -1) {
                (void) write(fd,
                             "HTTP/1.1 404 Not Found\nContent-Length: 136\nContent-Type: text/html\n\n<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe requested URL was not found on this server.\n</body></html>\n",
                             224);
                close(fd);
                exit(0);
            }

            printf("\nConsulta despues : %s\n",buffer);

            fstr=" application/octet-stream";
            len = (long)lseek(file_fd, (off_t)0, SEEK_END); /* lseek to the file end to find the length */
            (void)lseek(file_fd, (off_t)0, SEEK_SET); /* lseek back to the file start ready for reading */
            (void)sprintf(buffer,"HTTP/1.1 200 OK\nServer: "
                    "ServidorHTTP/1.0\nContent-Length: %ld\n"
                    "\nContent-Type: %s\n\n", len, fstr);
            (void)write(fd,buffer,strlen(buffer));



            while (	(ret = read(file_fd, buffer, BUFSIZE)) > 0 ) {
                (void)write(fd,buffer,ret);
            }
            sleep(1);	/* allow socket to drain before signalling the socket is closed */
            close(fd);

        }
    }

    return 0;
}
