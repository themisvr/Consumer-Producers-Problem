#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <errno.h>

#define handle_error_number(en, msg) \
    do { errno = en; perror(msg); strerror(errno); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


#endif // ERROR_HANDLING_H