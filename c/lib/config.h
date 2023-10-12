#ifndef CONFIG
#define CONFIG
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

struct config {
    int max_client;

    int connfd;
    char *ip;
    short int port;

    char *filename;
};

int parse_opt(int argc, char **argv, struct config *c);
#endif
