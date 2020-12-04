#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>


typedef struct cmd_args {
    uint32_t n_processes;
    size_t array_size;
} cmd_args;


void usage(char *executable) __attribute__((nonnull (1)));
void parse_args(int argc, char **argv, cmd_args *args) __attribute__((nonnull (2, 3)));



#endif // UTILS_H
