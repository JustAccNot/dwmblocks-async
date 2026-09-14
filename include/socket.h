#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/un.h>

typedef struct {
    int sockfd;
    struct sockaddr_un dst;
    socklen_t dst_len;
} socket_connection;

socket_connection *status_socket_init(void);
void status_socket_close(socket_connection* const socket_conn);
int status_socket_write(socket_connection* const socket_conn, const char* const name);

#endif  // SOCKET_H
