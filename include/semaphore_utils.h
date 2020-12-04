#ifndef SEMAPHORE_UTILS_H
#define SEMAPHORE_UTILS_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "error_handling.h"

#define PERMS 0666


static inline void sem_init(int *sem_id, size_t n_sems) {

    *sem_id = semget((key_t) IPC_PRIVATE, n_sems, IPC_CREAT | PERMS);
    if (*sem_id == -1) handle_error("Error: semget()");

}

static inline void sem_set_value(int sem_id, size_t n_sems, int val) {

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short array[1];
    } sem_attr;

    int rv;

    sem_attr.val = val;
    for (size_t i = 0; i != n_sems; ++i) {
        if ((rv = semctl(sem_id, i, SETVAL, sem_attr)) == -1)
            handle_error("Error: semtcl()");
    }   
}


static inline void sem_destroy(int sem_id) {

    if (semctl(sem_id, 0, IPC_RMID) == -1)
        handle_error("Error: semctl()");
}


/* semaphore down */
static inline void sem_p(int sem_id, int ith_sem) {

    struct sembuf sops = {
        .sem_flg = 0,
        .sem_num = ith_sem,
        .sem_op = -1
    };

    if (semop(sem_id, &sops, 1) == -1)
        handle_error("Error: semop()");
}

/* semaphore up */
static inline void sem_v(int sem_id, int ith_sem) {

    struct sembuf sops = {
        .sem_flg = 0,
        .sem_num = ith_sem,
        .sem_op = 1
    };

    if (semop(sem_id, &sops, 1) == -1)
        handle_error("Error: semop()");
}


#endif // SEMAPHORE_UTILS_H