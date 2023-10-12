#include "net.h"

/*
 * set `config->connfd` to the server fd
 * use `config->ip` and `config->port`
 * also use `config->max_client` for the backlog
 */
int
tcp_listen(struct config *c) {
    c->connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (c->connfd == -1)
        die("socket(): %s", strerror(errno));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(c->port),
        .sin_addr.s_addr = inet_addr(c->ip)
    };

    if (setsockopt(c->connfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1)
        die("setsockopt(): %s", strerror(errno));
    log_info("setsockopt(): success !");

    if (bind(c->connfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        die("bind(): %s", strerror(errno));
    log_info("bind(): success !");

    if (listen(c->connfd, c->max_client) == -1)
        die("listen(): %s", strerror(errno));

    return 1;
}

/*
 * set `config->connfd` to the client fd
 * use `config->ip` and `config->port`
 */

int
tcp_connect(struct config *c) {
    c->connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (c->connfd == -1)
        die("socket(): %s", strerror(errno));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(c->port),
        .sin_addr.s_addr = inet_addr(c->ip)
    };

    if (connect(c->connfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        die("connect(): %s", strerror(errno));
    log_info("connect(): success !");

    return 1;
}
