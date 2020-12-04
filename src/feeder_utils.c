#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "shared_memory.h"
#include "error_handling.h"
#include "feeder_utils.h"
#include "semaphore_utils.h"


extern int rd_sem_id, wrt_sem_id;


void write_number_to_shm(shm_t *shared_memory, size_t number, size_t n_readers) {

    shared_memory->number = number;
    gettimeofday(&shared_memory->tp, NULL);
    for (size_t i = 0; i != n_readers; ++i) {
        sem_v(rd_sem_id, i);
    }
    for (size_t i = 0; i != n_readers; ++i) {
        sem_p(wrt_sem_id, 0);
    }
}

void read_number_from_shm(shm_t *shared_memory, size_t *integer_array_copy, size_t index, 
                                                size_t ith_reader, double *running_average) {

    struct timeval end_t;
    uint64_t delay_t;

    sem_p(rd_sem_id, ith_reader);
    integer_array_copy[index] = shared_memory->number;
    gettimeofday(&end_t, NULL);
    delay_t = ((end_t.tv_sec * 1000000 + end_t.tv_usec) - ((shared_memory->tp).tv_sec * 1000000 + (shared_memory->tp).tv_usec));
    *running_average += delay_t;
    sem_v(wrt_sem_id, 0);
}


void create_log(size_t *integer_array_copy, size_t size, double running_average) {

    FILE *fp = NULL;

    fp = fopen("./process_logs.txt", "w");
    if (!fp) {
        handle_error("Error: fopen()");
    }
    for (size_t i = 0; i != size; ++i) {
        fprintf(fp, "%ld\n", integer_array_copy[i]);
    }
    fprintf(fp, "Last Process ID %d\n", getpid());
    fprintf(fp, "Running Average %lf\n", running_average);

    if (fclose(fp) == -1) handle_error("Error: close()");
}