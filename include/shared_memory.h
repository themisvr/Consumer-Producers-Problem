#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>


typedef struct shm_t {
    size_t number;
    struct timeval tp;
} shm_t;


shm_t *shared_memory_create(key_t key);
int shared_memory_dettach(shm_t *shared_memory);
int shared_memory_deallocate(int shm_id);



#endif // SHARED_MEMORY_H
