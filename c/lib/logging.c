#include "logging.h"

/*
 * log to stderr and works like printf
 * you can use the macros:
 * `log_info`
 * `log_warn`
 * `log_error`
 *
 * to avoid the tag
 */
void
log_format(const char tag[static 4], const char msg[static 1], ...) {
    va_list args;
    va_start(args, msg);

    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now);

    char date[256] = {0};
    strftime(date, sizeof(date), "%FT%T", timeinfo);

    fprintf(stderr, "%s [%s] ", date, tag);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");

    va_end(args);
}
