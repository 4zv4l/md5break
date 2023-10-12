#include "config.h"
#include "logging.h"

/*
 * accept:
 * -m max client
 * -f input file (hashes for server, wordlist for client)
 */
int
parse_opt(int argc, char **argv, struct config *c) {
    int opt;
    while((opt = getopt(argc, argv, ":m:f:")) != -1) {
        switch(opt) {
            case 'm': {
                          int arg = atoi(optarg);
                          if (arg != 0)
                              c->max_client = arg;
                          else
                              log_warn("bad value for option 'm': %s\n", optarg);
                          break;
                      }
            case 'f': 
                      c->filename = optarg;
                      break;

            case ':':
                      fprintf(stderr, "value required: %c\n", optopt);
                      return 0;
            case '?':
                      fprintf(stderr, "unknown option: %c\n", optopt);
                      return 0;
        }
    }

    argc -= optind;
    if (argc != 2)
        return fprintf(stderr, "usage: %s [OPTION] [ip] [port]\n", argv[0]), 0;

    c->ip = argv[optind];
    c->port = atoi(argv[optind+1]);
    return 1;
}
