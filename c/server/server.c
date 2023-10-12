#include <stdio.h>

#include "../lib/md5.h"
#include "../lib/logging.h"
#include "../lib/config.h"
#include "../lib/net.h"

int
handle(int clientfd, unsigned char digest_hex[static 32]) {
    dprintf(clientfd, "%s\n", digest_hex);

    char resp[1024] = {0};
    int l_resp = read(clientfd, resp, sizeof(resp));

    if (strcmp(resp, "NOP\n") == 0 || l_resp == 0) {
        log_info("%s  NOT FOUND :(", digest_hex);
        close(clientfd);
        return 0;
    }

    // check if its the right passwd
    resp[strlen(resp)-1] = 0;
    unsigned char buff_digest[MD5_DIGEST_LEN+1] = {0};
    unsigned char buff_digest_hex[(MD5_DIGEST_LEN*2)+1] = {0};
    md5_from_str(resp, buff_digest);
    md5_digest_hex(buff_digest, buff_digest_hex);
    if (memcmp(buff_digest_hex, digest_hex, MD5_DIGEST_LEN*2) != 0) {
        log_info("client sent bad info !");
        log_info("recv: %s", resp);
        log_info("for : %s", digest_hex);
        log_info("but : %s", buff_digest_hex);
        close(clientfd);
        return 0;
    }

    FILE *f = fopen("md5server.out", "a+");
    if (!f)
        die("couldnt open output file: %s", strerror(errno));
    printf("%s  %s\n", digest_hex, resp);
    fprintf(f, "%s  %s\n", digest_hex, resp);

    fclose(f);
    close(clientfd);
    return 1;
}

/*
 * read every line being 32 chars long (digest_hex)
 * in a char** buffer (needs to be freed afterward)
 */
char
**loadFile(char filename[static 1], int *counter) {
    FILE *f = fopen(filename, "r");
    if (!f)
        die("%s", strerror(errno));

    char line[1024];
    int n_line = 0;
    while(fgets(line, sizeof(line), f))
        n_line++;
    rewind(f);

    char **lines = malloc(sizeof(lines)*n_line+1);
    if (!lines)
        die("couldnt allocate memory to slurp the file");

    for (int i = 0; i < n_line; i++) {
        char *line = malloc(1024);
        if (!line)
            die("couldnt allocate memory for a line in the file");

        fgets(line, 1024, f);
        line[strlen(line)-1] = 0;
        int length = strlen(line);
        if (length != 32) {
            printf("skipping: %s\n", line);
            free(line);
            continue;
        }
        lines[(*counter)++] = line;
    }

    lines[n_line] = 0;
    return lines;
}

/*
 * no need to free `hashes` since it wont grow
 * and be auto free at the end
 */
int
main(int argc, char **argv) {
    struct config config = { .max_client = 5 };
    if (!parse_opt(argc, argv, &config))
        return EXIT_FAILURE;
    if (!config.filename)
        die("filename option is required");

    int n_hash = 0;
    char **hashes = loadFile(config.filename, &n_hash);
    if (!hashes)
        die("couldnt load file :(");

    printf("loaded(%d):\n", n_hash);
    for (int i = 0; hashes[i] != 0; i++) {
        printf("%s\n", hashes[i]);
    }

    if (!tcp_listen(&config))
        return EXIT_FAILURE;
    log_info("Listening on %s:%d", config.ip, config.port);

    for (;;) {
        struct sockaddr_in client_addr = {0};
        unsigned int client_len = 0;
        int clientfd = accept(config.connfd, (struct sockaddr*)&client_addr, &client_len);
        if (clientfd == -1) {
            log_warn("accept(): %s\n", strerror(errno));
            continue;
        }

        log_info("New Client");
        if (handle(clientfd, (unsigned char*)hashes[n_hash-1]))
            n_hash--;

        if (!n_hash)
            break;
    }

    printf("Enough work for today :)\n");
}
