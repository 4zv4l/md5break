#ifndef LOGGING
#define LOGGING
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

/*
 * log to stderr showing the level and arguments
 * works like printf()
 *
 * ex: log_info("Hello %s !", user);
 */
#define log_info(...)  log_format("INFO",  __VA_ARGS__)
#define log_warn(...)  log_format("WARN",  __VA_ARGS__)
#define log_error(...) log_format("ERROR", __VA_ARGS__)

/*
 * works like printf
 * print to stderr
 * and exit the program with code 255
 */
#define die(...) (fprintf(stderr, __VA_ARGS__), fputc('\n',stderr), exit(255))

void log_format(const char tag[static 4], const char msg[static 1], ...);
#endif
