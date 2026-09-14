#include "socket.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

socket_connection *status_socket_init(void) {
    const char *const runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (runtime_dir == NULL || runtime_dir[0] == '\0') {
        (void)fprintf(stderr, "error: XDG_RUNTIME_DIR is not set\n");
        return NULL;
    }

    const int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0) {
        (void)fprintf(stderr, "error: could not create status socket\n");
        return NULL;
    }

    socket_connection *const socket_conn = malloc(sizeof(*socket_conn));
    if (socket_conn == NULL) {
        close(fd);
        return NULL;
    }

    *socket_conn = (socket_connection) {
        .sockfd = fd,
        .dst = {.sun_family = AF_UNIX},
    };

    const int path_length =
        snprintf(socket_conn->dst.sun_path, sizeof(socket_conn->dst.sun_path),
                 "%s/dwl-status.sock", runtime_dir);
    if (path_length < 0 ||
        (size_t)path_length >= sizeof(socket_conn->dst.sun_path)) {
        (void)fprintf(stderr, "error: status socket path is too long\n");
        status_socket_close(socket_conn);
        return NULL;
    }

    socket_conn->dst_len =
        (socklen_t)(offsetof(struct sockaddr_un, sun_path) +
                    (size_t)path_length + 1);

    if (access(socket_conn->dst.sun_path, F_OK) < 0) {
        (void)fprintf(stderr, "error: status socket does not exist\n");
        status_socket_close(socket_conn);
        return NULL;
    }

    return socket_conn;
}

void status_socket_close(socket_connection *const socket_conn) {
    (void)close(socket_conn->sockfd);
    free(socket_conn);
}

int status_socket_write(socket_connection *const socket_conn,
                        const char *const name) {
    if (sendto(socket_conn->sockfd, name, strlen(name), 0,
               (const struct sockaddr *)&socket_conn->dst,
               socket_conn->dst_len) < 0) {
        return 1;
    }

    return 0;
}
