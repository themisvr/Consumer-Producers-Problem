#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "error_handling.h"
#include "shared_memory.h"

#define PERMS 0666


static inline int shm_get(key_t key, size_t size) {
	
    assert(size > 0);

    int shm_id;
    
    shm_id = shmget(key, size, IPC_CREAT | PERMS);
	if (shm_id == -1) {
        handle_error("Error: shmget");
    }
    return shm_id;
}

static inline void *shm_attach(int shm_id) {
	
    return shmat(shm_id, NULL, 0);
}

static inline int shm_dettach(const void *memory) {
	
    assert(memory != NULL);

	int shm_id;
	if ((shm_id = shmdt(memory)) == -1) {
		handle_error("Error: shmdt");
	}
	return shm_id;
}

static inline int shm_control(int shm_id) {
	
    return shmctl(shm_id, IPC_RMID, 0);
}


shm_t *shared_memory_create(key_t key) {

    int shm_id;
    shm_t *shared_memory;

    shm_id = shm_get(key, sizeof(shm_t));
    shared_memory = shm_attach(shm_id);
    shared_memory->number = 0;
    
    return shared_memory;
}

int shared_memory_dettach(shm_t *shared_memory) {

    int shm_id;

    if ((shm_id = shm_dettach(shared_memory)) == -1)
        handle_error("Error: shmdt");
    return shm_id;
}

int shared_memory_deallocate(int shm_id) {

    return shm_control(shm_id);
}


