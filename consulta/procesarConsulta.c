//
// Created by josue on 9/28/15.
//

#include "procesarConsulta.h"
#include "unistd.h"
#include "fcntl.h"
#include "string.h"
#include "stdio.h"

#define BUFSIZE 8096

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
                     207);
        printf("\nHa llegado una nueva consulta solicitando el recurso %s pero no se encontro\n",&buffer[5]);
        goto final; // IR AL FINAL, SI ES MODO 5 PUEDEN HABER HILOS ESPERANDO
    }

    printf("\nHa llegado una nueva consulta solicitando el recurso %s\n",&buffer[5]);
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


    final: // VENIR AQUI CUANDO ENCUENTRA O NO EL ARCHIVO

    sleep(1);
    close(fd);
    return NULL;
}
