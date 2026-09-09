#include "socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

socket_connection *status_socket_init(void) {
    int fd  = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0 ) {
        return NULL;
    }

    socket_connection *const socket_conn = (socket_connection *)malloc(sizeof(socket_connection));
    if (!socket_conn) {
        close(fd);
        return NULL;
    }

    socket_conn->dst = (struct sockaddr_un *)malloc(sizeof(struct sockaddr_un));
    if (!socket_conn->dst) {
        close(fd);
	    free(socket_conn);
    	return NULL;
    }

    socket_conn->sockfd = fd;
    socket_conn->dst->sun_family = AF_UNIX;
    snprintf(socket_conn->dst->sun_path, sizeof(socket_conn->dst->sun_path),
        "%s/dwl-status.sock", getenv("XDG_RUNTIME_DIR"));

    if (access(socket_conn->dst->sun_path, F_OK) < 0) {
        status_socket_close(socket_conn);
        return NULL;
    }

    return socket_conn;
}

void status_socket_close(socket_connection *const socket_conn) {
    close(socket_conn->sockfd);
    free(socket_conn->dst);
    free(socket_conn);
}

int status_socket_write(socket_connection *const socket_conn, const char* const name) {
    if (sendto(socket_conn->sockfd, name, strlen(name), 0,
        (struct sockaddr *)socket_conn->dst, sizeof(*socket_conn->dst)) < 0) {
        return 1;
    }
    return 0;
}
