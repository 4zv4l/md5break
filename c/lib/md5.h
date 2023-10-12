#ifndef MD5
#define MD5
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define MD5_DIGEST_LEN 16

int md5_digest_from_hex(const unsigned char digest[static 32], unsigned char buff[static 16]);
void md5_digest_hex(const unsigned char digest[static 16], unsigned char buff[static 32]);
void md5_digest(const unsigned char digest[static 16]);
void md5_digest_ex(const unsigned char digest[static 16], const char str[static 1]);
int md5_from_file(const char filename[static 1], unsigned char[static 16]);
int md5_from_str(const char str[static 1], unsigned char[static 16]);
#endif
