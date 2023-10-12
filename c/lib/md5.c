#include "md5.h"


/*
 * convert digest from hex and store it in buff (must be at least 16 char long)
 */
int
md5_digest_from_hex(const unsigned char digest_hex[static 32], unsigned char buff[static 16]) {
    for (int i = 0; i < 16; i++) {
        sscanf((char*)digest_hex, "%2hhx", &buff[i]);
        digest_hex += 2;
    }

    return 1;
}

/*
 * convert digest to hex and store it in buff (must be at least 32 char long)
 */
void
md5_digest_hex(const unsigned char digest[static 16], unsigned char buff[static 32]) {
    for (int i = 0; i < 16; i++)
        buff += sprintf((char *)buff, "%02x", digest[i]);
}

/*
 * show the md5 digest in hex
 */
void
md5_digest(const unsigned char digest[static 16]) {
    for (int i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("\n");
}

/*
 * show the md5 digest in hex followed by the string
 */
void
md5_digest_ex(const unsigned char digest[static 16], const char str[static 1]) {
    for (int i = 0; i < 16; i++)
        printf("%02x", digest[i]);
    printf("  %s\n", str);
}

/*
 * compute md5 from given filename
 * return 1 if ok
 * print error to stderr and return error code otherwise
 */
int
md5_from_file(const char filename[static 1], unsigned char digest[static 16]) {
    FILE *file = fopen(filename, "rb");
    if (!file)
        return fprintf(stderr, "%s: %s\n", filename, strerror(errno)), 0;

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex(ctx, EVP_md5(), NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    char buff[1024] = {0};
    int length = 0;
    while((length = fread(buff, 1, 1024, file)) > 0) {
        if (EVP_DigestUpdate(ctx, buff, length) == 0)
            return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;
    }

    if (EVP_DigestFinal_ex(ctx, digest, NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    fclose(file);
    EVP_MD_CTX_free(ctx);

    return 1;
}

/*
 * compute md5 of string (MUST be 0 terminated)
 * return 1 if ok
 * print error to stderr and return error code otherwise
 */
int
md5_from_str(const char str[static 1], unsigned char digest[static 16]) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex(ctx, EVP_md5(), NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    if (EVP_DigestUpdate(ctx, str, strlen(str)) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    if (EVP_DigestFinal_ex(ctx, digest, NULL) == 0)
        return fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL)), 0;

    EVP_MD_CTX_free(ctx);

    return 1;
}
