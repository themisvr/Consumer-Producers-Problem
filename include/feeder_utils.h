#ifndef FEEDER_UTILS_H
#define FEEDER__UTILS_H

#include <stdint.h>
#include "shared_memory.h"

void write_number_to_shm(shm_t *shared_memory, size_t number, size_t n_readers) __attribute__ ((nonnull (1)));
void read_number_from_shm(shm_t *shared_memery, size_t *integer_array, size_t index, 
                                    size_t ith_reader, double *running_average) __attribute__ ((nonnull (1, 2)));

void create_log(size_t *integer_array_copy, size_t size, double running_average) __attribute__ ((nonnull (1)));


#endif // FEEDER_UTILS_H