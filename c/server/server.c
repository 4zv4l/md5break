#include <stdio.h>

#include "../lib/md5.h"
#include "../lib/logging.h"
#include "../lib/config.h"
#include "../lib/net.h"

int
handle(int clientfd, char digest_hex[static 32]) {
    dprintf(clientfd, "%s\n", digest_hex);

    char resp[1024] = {0};
    int l_resp = read(clientfd, resp, sizeof(resp));

    if (strcmp(resp, "NOP") == 0 || l_resp == 0) {
        close(clientfd);
        return 0;
    }

    FILE *f = fopen("md5server.out", "a+");
    if (!f)
        die("couldnt open output file: %s", strerror(errno));
    printf("%s  %s", digest_hex, resp);
    fprintf(f, "%s  %s", digest_hex, resp);

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
        int length = strlen(line)-1;
        if (length != 32) {
            printf("skipping: %s", line);
            free(line);
            continue;
        }
        line[strlen(line)-1] = 0;
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
        if (handle(clientfd, hashes[n_hash-1]))
            n_hash--;

        if (!n_hash)
            break;
    }

    printf("Enough work for today :)\n");
}
