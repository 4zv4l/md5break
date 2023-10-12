#include "../lib/md5.h"

/*
 * test program to test the md5 lib
 * print the `digest_hex` from input str
 */
int
main(void) {
    printf("> ");
    fflush(stdout);

    char buff[1024] = {0};
    fgets(buff, sizeof(buff), stdin);
    buff[strlen(buff)-1] = 0;

    unsigned char digest[MD5_DIGEST_LEN] = {0};
    unsigned char digest_hex[(MD5_DIGEST_LEN*2)] = {0};
    md5_from_str(buff, digest);
    md5_digest_hex(digest, digest_hex);

    printf("%s  %s\n", digest_hex, buff);
}
