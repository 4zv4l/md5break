#ifndef NET
#define NET

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "config.h"
#include "logging.h"

int tcp_listen(struct config *c);
int tcp_connect(struct config *c);

#endif
