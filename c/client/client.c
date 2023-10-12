#include <stdio.h>

#include "../lib/md5.h"
#include "../lib/logging.h"
#include "../lib/config.h"
#include "../lib/net.h"

struct cracker {
    unsigned char digest[MD5_DIGEST_LEN+1];
    unsigned char digest_hex[(MD5_DIGEST_LEN*2)+1];
    char passwd[4096];
    char *filename;
    FILE *file;
};

/*
 * Try each password in `c->filename` against `c->digest`
 * Read from `stdin` if `c->filename` is 0
 */
int
crack(struct cracker *c) {
    if (!md5_digest_from_hex(c->digest_hex, c->digest))
        die("Couldnt convert from hex_digest to bytes_digest");

    c->file = c->filename ? fopen(c->filename, "r") : stdin;
    if (!c->file)
        die("Couldnt open %s for reading :(", c->filename);

    unsigned char buff[MD5_DIGEST_LEN+1] = {0};
    unsigned char buff_hex[(MD5_DIGEST_LEN*2)+1] = {0};
    while(fgets(c->passwd, sizeof(c->passwd), c->file)) {
        c->passwd[strlen(c->passwd)-1] = 0;

        md5_from_str(c->passwd, buff);
        md5_digest_hex(buff, buff_hex);
        log_info("Trying:   %s  %s", buff_hex, c->passwd);
        log_info("Against:  %s", c->digest_hex);

        if(memcmp(buff, c->digest, MD5_DIGEST_LEN) == 0)
            return log_info("%s MATCH !", c->passwd), 1;
    }

    if (c->file != stdin)
        fclose(c->file);
    return 0;
}

int
handle(struct config *config) {
    struct cracker cracker = { .filename = config->filename };

    if (read(config->connfd, cracker.digest_hex, sizeof(cracker.digest_hex)) == 0)
        die("Couldnt receive digest from server :(");
    cracker.digest_hex[strlen((char*)cracker.digest_hex)-1] = 0;
    log_info("Received: %s", cracker.digest_hex);

    if (!crack(&cracker)) {
        dprintf(config->connfd, "%s\n", "NOP");
        die("Couldnt crack %s :(", cracker.digest_hex);
    }

    dprintf(config->connfd, "%s\n", cracker.passwd);
    close(config->connfd);
    return 1;
}

int
main(int argc, char **argv) {
    struct config config = { .max_client = 5 };
    if (!parse_opt(argc, argv, &config))
        return EXIT_FAILURE;

    if (!tcp_connect(&config))
        return EXIT_FAILURE;
    log_info("Connected to %s:%d", config.ip, config.port);

    if (!handle(&config))
        return EXIT_FAILURE;
}
