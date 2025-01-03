//
// Created by pknadimp on 1/2/25.
//
#include "utils.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

int run_cmd(const char *cmd, ...) {
    va_list ap;
    char buf[CMDBUFLEN];

    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);
    va_end(ap);
    printf("%s\n", buf);
    return system(buf);
}


