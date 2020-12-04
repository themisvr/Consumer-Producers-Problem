#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "error_handling.h"


void usage(char *executable) { 

    fprintf(stderr, "Usage: %s [-n n_processes] [-s array_size]\nExiting...\n", executable);
    exit(EXIT_FAILURE);
}


void parse_args(int argc, char **argv, cmd_args *args) {

    int opt;
    while ((opt = getopt(argc, argv, "n:s:")) != -1) {
        switch(opt) {
            case 'n':
                args->n_processes = atoi(optarg);
                if (args->n_processes < 1) {
                    fprintf(stderr, "Error: Give a number > 0\nExiting...\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                args->array_size = atoi(optarg);
                if (args->array_size < 3000) {
                    fprintf(stderr, "Error: Give a number >= 3000\nExiting...\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                usage(argv[0]);
                break;
        }
    }

}