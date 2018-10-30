#include "log.h"

#include <stdarg.h>
#include <cstdio>

void log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int result = vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
}
